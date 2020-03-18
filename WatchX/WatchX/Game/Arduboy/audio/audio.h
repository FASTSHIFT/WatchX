#ifndef ArduboyAudio_h
#define ArduboyAudio_h

#include <Arduino.h>
//#include <EEPROM.h>
#include <avr/pgmspace.h>
//#include <avr/power.h>

#define AVAILABLE_TIMERS 2
#define TUNE_OP_PLAYNOTE  0x90  /* play a note: low nibble is generator #, note is next byte */
#define TUNE_OP_STOPNOTE  0x80  /* stop a note: low nibble is generator # */
#define TUNE_OP_RESTART   0xe0  /* restart the score from the beginning */
#define TUNE_OP_STOP      0xf0  /* stop playing */


class ArduboyAudio
{
public:
  void static begin();
  void static on();
  void static off();
  void static saveOnOff();
  bool static enabled();

protected:
  bool static audio_enabled;
};


class ArduboyTunes
{
public:
  // Playtune Functions

  /// Assign a timer to an output pin.
  void initChannel(byte pin);

  /// Start playing a polyphonic score.
  void playScore(const byte *score);

  /// Stop playing the score.
  void stopScore();

  /// Delay in milliseconds.
  void delay(unsigned msec);

  /// Stop all timers.
  void closeChannels();

  bool playing();
  void tone(unsigned int frequency, unsigned long duration);

  // called via interrupt
  void static step();
  void static soundOutput();

private:
  void static playNote (byte chan, byte note);
  void static stopNote (byte chan);
};
#endif
