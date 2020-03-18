#include "../Arduboy.h"
#include "audio.h"

static const byte PROGMEM tune_pin_to_timer_PGM[] = { 3, 1 };
static volatile uint32_t *_tunes_timer1_pin_port;
static volatile uint32_t _tunes_timer1_pin_mask;
static volatile int32_t timer1_toggle_count;
static volatile uint32_t *_tunes_timer3_pin_port;
static volatile uint32_t _tunes_timer3_pin_mask;
static byte _tune_pins[AVAILABLE_TIMERS];
static byte _tune_num_chans = 0;
static volatile boolean tune_playing; // is the score still playing?
static volatile unsigned wait_timer_frequency2;       /* its current frequency */
static volatile boolean wait_timer_playing = false;   /* is it currently playing a note? */
static volatile boolean tonePlaying = false;
static volatile unsigned long wait_toggle_count;      /* countdown score waits */

// pointers to your musical score and your position in said score
static volatile const byte *score_start = 0;
static volatile const byte *score_cursor = 0;

#ifndef __AVR__
#  define power_timer1_enable()
#  define power_timer1_disable()
#  define power_timer3_enable()
#  define power_timer3_disable()
static uint8_t TCCR1A, TCCR3A;
static uint8_t TCCR1B, TCCR3B;
static uint8_t WGM12, WGM32;
static uint8_t CS10, CS30;
static uint8_t OCR1A, OCR3A;
static uint8_t TIMSK1, TIMSK3;
static uint8_t OCIE1A, OCIE3A;
#  define pinMode(pin,mode) 
#endif

// Table of midi note frequencies * 2
//   They are times 2 for greater accuracy, yet still fits in a word.
//   Generated from Excel by =ROUND(2*440/32*(2^((x-9)/12)),0) for 0<x<128
// The lowest notes might not work, depending on the Arduino clock frequency
// Ref: http://www.phy.mtu.edu/~suits/notefreqs.html
const uint8_t PROGMEM _midi_byte_note_frequencies[48] =
{
    16, 17, 18, 19, 21, 22, 23, 24, 26, 28, 29, 31, 33, 35, 37, 39, 41, 44, 46, 49, 52, 55, 58, 62, 65,
    69, 73, 78, 82, 87, 92, 98, 104, 110, 117, 123, 131, 139, 147, 156, 165, 175, 185, 196, 208, 220,
    233, 247
};
const unsigned int PROGMEM _midi_word_note_frequencies[80] =
{
    262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494, 523, 554, 587, 622, 659,
    698, 740, 784, 831, 880, 932, 988, 1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760,
    1865, 1976, 2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729, 3951, 4186,
    4435, 4699, 4978, 5274, 5588, 5920, 6272, 6645, 7040, 7459, 7902, 8372, 8870, 9397, 9956,
    10548, 11175, 11840, 12544, 13290, 14080, 14917, 15804, 16744, 17740, 18795, 19912, 21096,
    22351, 23680, 25088
};

/* AUDIO */

bool ArduboyAudio::audio_enabled = false;

void ArduboyAudio::on()
{
    power_timer1_enable();
    power_timer3_enable();
    audio_enabled = true;
}

bool ArduboyAudio::enabled()
{
    return audio_enabled;
}

void ArduboyAudio::off()
{
    audio_enabled = false;
    power_timer1_disable();
    power_timer3_disable();
}

void ArduboyAudio::saveOnOff()
{
//  EEPROM.write(EEPROM_AUDIO_ON_OFF, audio_enabled);
}

void ArduboyAudio::begin()
{
    tune_playing = false;
    if (1/*EEPROM.read(EEPROM_AUDIO_ON_OFF)*/)
        on();
}

/* TUNES */

void ArduboyTunes::initChannel(byte pin)
{
    byte timer_num;

    // we are all out of timers
    if (_tune_num_chans == AVAILABLE_TIMERS)
        return;

    timer_num = pgm_read_byte(tune_pin_to_timer_PGM + _tune_num_chans);
    _tune_pins[_tune_num_chans] = pin;
    _tune_num_chans++;
    pinMode(pin, OUTPUT);
    switch (timer_num)
    {
    case 1: // 16 bit timer
        TCCR1A = 0;
        TCCR1B = 0;
        bitWrite(TCCR1B, WGM12, 1);
        bitWrite(TCCR1B, CS10, 1);
        _tunes_timer1_pin_port = portOutputRegister(digitalPinToPort(pin));
        _tunes_timer1_pin_mask = digitalPinToBitMask(pin);
        break;
    case 3: // 16 bit timer
        TCCR3A = 0;
        TCCR3B = 0;
        bitWrite(TCCR3B, WGM32, 1);
        bitWrite(TCCR3B, CS30, 1);
        _tunes_timer3_pin_port = portOutputRegister(digitalPinToPort(pin));
        _tunes_timer3_pin_mask = digitalPinToBitMask(pin);
        playNote(0, 60);  /* start and stop channel 0 (timer 3) on middle C so wait/delay works */
        stopNote(0);
        break;
    }
}

void ArduboyTunes::playNote(byte chan, byte note)
{
    byte timer_num;
    byte prescalar_bits;
    unsigned int frequency2; /* frequency times 2 */
    unsigned long ocr;

    // we can't plan on a channel that does not exist
    if (chan >= _tune_num_chans)
        return;

    // we only have frequencies for 128 notes
    if (note > 127)
    {
        return;
    }

    timer_num = pgm_read_byte(tune_pin_to_timer_PGM + chan);
    if (note < 48)
    {
        frequency2 = pgm_read_byte(_midi_byte_note_frequencies + note);
    }
    else
    {
        frequency2 = pgm_read_word(_midi_word_note_frequencies + note - 48);
    }

    //******  16-bit timer  *********
    // two choices for the 16 bit timers: ck/1 or ck/64
    ocr = F_CPU / frequency2 - 1;
    prescalar_bits = 0b001;
    if (ocr > 0xffff)
    {
        ocr = F_CPU / frequency2 / 64 - 1;
        prescalar_bits = 0b011;
    }
    // Set the OCR for the given timer, then turn on the interrupts
    switch (timer_num)
    {
    case 1:
        TCCR1B = (TCCR1B & 0b11111000) | prescalar_bits;
        OCR1A = ocr;
        bitWrite(TIMSK1, OCIE1A, 1);
        break;
    case 3:
        TCCR3B = (TCCR3B & 0b11111000) | prescalar_bits;
        OCR3A = ocr;
        wait_timer_frequency2 = frequency2;  // for "tune_delay" function
        wait_timer_playing = true;
        bitWrite(TIMSK3, OCIE3A, 1);
        break;
    }
}

void ArduboyTunes::stopNote(byte chan)
{
    byte timer_num;
    timer_num = pgm_read_byte(tune_pin_to_timer_PGM + chan);
    switch (timer_num)
    {
    case 1:
        TIMSK1 &= ~(1 << OCIE1A);                 // disable the interrupt
        *_tunes_timer1_pin_port &= ~(_tunes_timer1_pin_mask);   // keep pin low after stop
        break;
    case 3:
        wait_timer_playing = false;
        *_tunes_timer3_pin_port &= ~(_tunes_timer3_pin_mask);   // keep pin low after stop
        break;
    }
}

void ArduboyTunes::playScore(const byte *score)
{
    score_start = score;
    score_cursor = score_start;
    step();  /* execute initial commands */
    tune_playing = true;  /* release the interrupt routine */
}

void ArduboyTunes::stopScore (void)
{
    for (uint8_t i = 0; i < _tune_num_chans; i++)
        stopNote(i);
    tune_playing = false;
}

bool ArduboyTunes::playing()
{
    return tune_playing;
}

/* Do score commands until a "wait" is found, or the score is stopped.
This is called initially from tune_playcore, but then is called
from the interrupt routine when waits expire.
*/
/* if CMD < 0x80, then the other 7 bits and the next byte are a 15-bit big-endian number of msec to wait */
void ArduboyTunes::step()
{
    byte command, opcode, chan;
    unsigned duration;

    while (1)
    {
        command = pgm_read_byte(score_cursor++);
        opcode = command & 0xf0;
        chan = command & 0x0f;
        if (opcode == TUNE_OP_STOPNOTE)   /* stop note */
        {
            stopNote(chan);
        }
        else if (opcode == TUNE_OP_PLAYNOTE)   /* play note */
        {
            playNote(chan, pgm_read_byte(score_cursor++));
        }
        else if (opcode == TUNE_OP_RESTART)   /* restart score */
        {
            score_cursor = score_start;
        }
        else if (opcode == TUNE_OP_STOP)   /* stop score */
        {
            tune_playing = false;
            break;
        }
        else if (opcode < 0x80)   /* wait count in msec. */
        {
            duration = ((unsigned)command << 8) | (pgm_read_byte(score_cursor++));
            wait_toggle_count = ((unsigned long) wait_timer_frequency2 * duration + 500) / 1000;
            if (wait_toggle_count == 0) wait_toggle_count = 1;
            break;
        }
    }
}

void ArduboyTunes::closeChannels(void)
{
    byte timer_num;
    for (uint8_t chan = 0; chan < _tune_num_chans; chan++)
    {
        timer_num = pgm_read_byte(tune_pin_to_timer_PGM + chan);
        switch (timer_num)
        {
        case 1:
            TIMSK1 &= ~(1 << OCIE1A);
            break;
        case 3:
            TIMSK3 &= ~(1 << OCIE3A);
            break;
        }
        digitalWrite(_tune_pins[chan], 0);
    }
    _tune_num_chans = 0;
    tune_playing = false;
}

void ArduboyTunes::soundOutput()
{
    if (wait_timer_playing)   // toggle the pin if we're sounding a note
    {
        *_tunes_timer3_pin_port ^= _tunes_timer3_pin_mask;
    }
    if (tune_playing && wait_toggle_count && --wait_toggle_count == 0)
    {
        // end of a score wait, so execute more score commands
        ArduboyTunes::step();  // execute commands
    }
}

void ArduboyTunes::tone(unsigned int frequency, unsigned long duration)
{
    tonePlaying = true;
    uint8_t prescalarbits = 0b001;
    int32_t toggle_count = 0;
    uint32_t ocr = 0;

    // two choices for the 16 bit timers: ck/1 or ck/64
    ocr = F_CPU / frequency / 2 - 1;
    prescalarbits = 0b001;
    if (ocr > 0xffff)
    {
        ocr = F_CPU / frequency / 2 / 64 - 1;
        prescalarbits = 0b011;
    }
    TCCR1B = (TCCR1B & 0b11111000) | prescalarbits;

    // Calculate the toggle count
    if (duration > 0)
    {
        toggle_count = 2 * frequency * duration / 1000;
    }
    else
    {
        toggle_count = -1;
    }
    // Set the OCR for the given timer,
    // set the toggle count,
    // then turn on the interrupts
    OCR1A = ocr;
    timer1_toggle_count = toggle_count;
    bitWrite(TIMSK1, OCIE1A, 1);
}

// TIMER 1
static void ISR_TIMER1_COMPA_vect()
{
    if (tonePlaying)
    {
        if (timer1_toggle_count != 0)
        {
            // toggle the pin
            *_tunes_timer1_pin_port ^= _tunes_timer1_pin_mask;
            if (timer1_toggle_count > 0) timer1_toggle_count--;
        }
        else
        {
            tonePlaying = false;
            TIMSK1 &= ~(1 << OCIE1A);                 // disable the interrupt
            *_tunes_timer1_pin_port &= ~(_tunes_timer1_pin_mask);   // keep pin low after stop
        }
    }
    else
    {
        *_tunes_timer1_pin_port ^= _tunes_timer1_pin_mask;  // toggle the pin
    }
}

// TIMER 3
static void ISR_TIMER3_COMPA_vect()
{
    // Timer 3 is the one assigned first, so we keep it running always
    // and use it to time score waits, whether or not it is playing a note.
    ArduboyTunes::soundOutput();
}

