/**
 * @file Sprites.cpp
 * \brief
 * A class for drawing animated sprites from image and mask bitmaps.
 */

#include "Sprites.h"

void Sprites::drawExternalMask(int16_t x, int16_t y, const uint8_t *bitmap,
                               const uint8_t *mask, uint8_t frame, uint8_t mask_frame)
{
    draw(x, y, bitmap, frame, mask, mask_frame, SPRITE_MASKED);
}

void Sprites::drawOverwrite(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame)
{
    draw(x, y, bitmap, frame, NULL, 0, SPRITE_OVERWRITE);
}

void Sprites::drawErase(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame)
{
    draw(x, y, bitmap, frame, NULL, 0, SPRITE_IS_MASK_ERASE);
}

void Sprites::drawSelfMasked(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame)
{
    draw(x, y, bitmap, frame, NULL, 0, SPRITE_IS_MASK);
}

void Sprites::drawPlusMask(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame)
{
    //draw(x, y, bitmap, frame, NULL, 0, SPRITE_PLUS_MASK);
    uint8_t w = bitmap[0];
    uint8_t h = bitmap[1];

    bitmap += 2;

    for (int j = 0; j < h; j++)
    {
        for (int i = 0; i < w; i++)
        {
            int blockY = j / 8;
            int blockIndex = (w * blockY + i) * 2;
            uint8_t pixels = bitmap[blockIndex];
            uint8_t maskPixels = bitmap[blockIndex + 1];
            uint8_t bitmask = 1 << (j % 8);

            if (maskPixels & bitmask)
            {
                if (x + i >= 0 && y + j >= 0)
                {
                    if (pixels & bitmask)
                    {
                        Arduboy2Base::drawPixel(x + i, y + j, WHITE);
                    }
                    else
                    {
                        Arduboy2Base::drawPixel(x + i, y + j, BLACK);
                    }
                }
            }
        }
    }
}


//common functions
void Sprites::draw(int16_t x, int16_t y,
                   const uint8_t *bitmap, uint8_t frame,
                   const uint8_t *mask, uint8_t sprite_frame,
                   uint8_t drawMode)
{
    unsigned int frame_offset;

    if (bitmap == NULL)
        return;

    uint8_t width = pgm_read_byte(bitmap);
    uint8_t height = pgm_read_byte(++bitmap);
    bitmap++;
    if (frame > 0 || sprite_frame > 0)
    {
        frame_offset = (width * ( height / 8 + ( height % 8 == 0 ? 0 : 1)));
        // sprite plus mask uses twice as much space for each frame
        if (drawMode == SPRITE_PLUS_MASK)
        {
            frame_offset *= 2;
        }
        else if (mask != NULL)
        {
            mask += sprite_frame * frame_offset;
        }
        bitmap += frame * frame_offset;
    }

    // if we're detecting the draw mode then base it on whether a mask
    // was passed as a separate object
    if (drawMode == SPRITE_AUTO_MODE)
    {
        drawMode = mask == NULL ? SPRITE_UNMASKED : SPRITE_MASKED;
    }

    drawBitmap(x, y, bitmap, mask, width, height, drawMode);
}

void Sprites::drawBitmap(int16_t x, int16_t y,
                         const uint8_t *bitmap, const uint8_t *mask,
                         uint8_t w, uint8_t h, uint8_t draw_mode)
{
    // no need to draw at all of we're offscreen
    if (x + w <= 0 || x > WIDTH - 1 || y + h <= 0 || y > HEIGHT - 1)
        return;

    if (bitmap == NULL)
        return;

    // xOffset technically doesn't need to be 16 bit but the math operations
    // are measurably faster if it is
    uint16_t xOffset, ofs;
    int8_t yOffset = y & 7;
    int8_t sRow = y / 8;
    uint8_t loop_h, start_h, rendered_width;

    if (y < 0 && yOffset > 0)
    {
        sRow--;
    }

    // if the left side of the render is offscreen skip those loops
    if (x < 0)
    {
        xOffset = abs(x);
    }
    else
    {
        xOffset = 0;
    }

    // if the right side of the render is offscreen skip those loops
    if (x + w > WIDTH - 1)
    {
        rendered_width = ((WIDTH - x) - xOffset);
    }
    else
    {
        rendered_width = (w - xOffset);
    }

    // if the top side of the render is offscreen skip those loops
    if (sRow < -1)
    {
        start_h = abs(sRow) - 1;
    }
    else
    {
        start_h = 0;
    }

    loop_h = h / 8 + (h % 8 > 0 ? 1 : 0); // divide, then round up

    // if (sRow + loop_h - 1 > (HEIGHT/8)-1)
    if (sRow + loop_h > (HEIGHT / 8))
    {
        loop_h = (HEIGHT / 8) - sRow;
    }

    // prepare variables for loops later so we can compare with 0
    // instead of comparing two variables
    loop_h -= start_h;

    sRow += start_h;
    ofs = (sRow * WIDTH) + x + xOffset;
    uint8_t *bofs = (uint8_t *)bitmap + (start_h * w) + xOffset;
    uint8_t data;

    uint8_t mul_amt = 1 << yOffset;
    uint16_t mask_data;
    uint16_t bitmap_data;

    switch (draw_mode)
    {
    case SPRITE_UNMASKED:
        // we only want to mask the 8 bits of our own sprite, so we can
        // calculate the mask before the start of the loop
        mask_data = ~(0xFF * mul_amt);
        // really if yOffset = 0 you have a faster case here that could be
        // optimized
        for (uint8_t a = 0; a < loop_h; a++)
        {
            for (uint8_t iCol = 0; iCol < rendered_width; iCol++)
            {
                bitmap_data = pgm_read_byte(bofs) * mul_amt;

                if (sRow >= 0)
                {
                    data = Arduboy2Base::sBuffer[ofs];
                    data &= (uint8_t)(mask_data);
                    data |= (uint8_t)(bitmap_data);
                    Arduboy2Base::sBuffer[ofs] = data;
                }
                if (yOffset != 0 && sRow < 7)
                {
                    data = Arduboy2Base::sBuffer[ofs + WIDTH];
                    data &= (*((unsigned char *) (&mask_data) + 1));
                    data |= (*((unsigned char *) (&bitmap_data) + 1));
                    Arduboy2Base::sBuffer[ofs + WIDTH] = data;
                }
                ofs++;
                bofs++;
            }
            sRow++;
            bofs += w - rendered_width;
            ofs += WIDTH - rendered_width;
        }
        break;

    case SPRITE_IS_MASK:
        for (uint8_t a = 0; a < loop_h; a++)
        {
            for (uint8_t iCol = 0; iCol < rendered_width; iCol++)
            {
                bitmap_data = pgm_read_byte(bofs) * mul_amt;
                if (sRow >= 0)
                {
                    Arduboy2Base::sBuffer[ofs] |= (uint8_t)(bitmap_data);
                }
                if (yOffset != 0 && sRow < 7)
                {
                    Arduboy2Base::sBuffer[ofs + WIDTH] |= (*((unsigned char *) (&bitmap_data) + 1));
                }
                ofs++;
                bofs++;
            }
            sRow++;
            bofs += w - rendered_width;
            ofs += WIDTH - rendered_width;
        }
        break;

    case SPRITE_IS_MASK_ERASE:
        for (uint8_t a = 0; a < loop_h; a++)
        {
            for (uint8_t iCol = 0; iCol < rendered_width; iCol++)
            {
                bitmap_data = pgm_read_byte(bofs) * mul_amt;
                if (sRow >= 0)
                {
                    Arduboy2Base::sBuffer[ofs]  &= ~(uint8_t)(bitmap_data);
                }
                if (yOffset != 0 && sRow < 7)
                {
                    Arduboy2Base::sBuffer[ofs + WIDTH] &= ~(*((unsigned char *) (&bitmap_data) + 1));
                }
                ofs++;
                bofs++;
            }
            sRow++;
            bofs += w - rendered_width;
            ofs += WIDTH - rendered_width;
        }
        break;

    case SPRITE_MASKED:
        uint8_t *mask_ofs;
        mask_ofs = (uint8_t *)mask + (start_h * w) + xOffset;
        for (uint8_t a = 0; a < loop_h; a++)
        {
            for (uint8_t iCol = 0; iCol < rendered_width; iCol++)
            {
                // NOTE: you might think in the yOffset==0 case that this results
                // in more effort, but in all my testing the compiler was forcing
                // 16-bit math to happen here anyways, so this isn't actually
                // compiling to more code than it otherwise would. If the offset
                // is 0 the high part of the word will just never be used.

                // load data and bit shift
                // mask needs to be bit flipped
                mask_data = ~(pgm_read_byte(mask_ofs) * mul_amt);
                bitmap_data = pgm_read_byte(bofs) * mul_amt;

                if (sRow >= 0)
                {
                    data = Arduboy2Base::sBuffer[ofs];
                    data &= (uint8_t)(mask_data);
                    data |= (uint8_t)(bitmap_data);
                    Arduboy2Base::sBuffer[ofs] = data;
                }
                if (yOffset != 0 && sRow < 7)
                {
                    data = Arduboy2Base::sBuffer[ofs + WIDTH];
                    data &= (*((unsigned char *) (&mask_data) + 1));
                    data |= (*((unsigned char *) (&bitmap_data) + 1));
                    Arduboy2Base::sBuffer[ofs + WIDTH] = data;
                }
                ofs++;
                mask_ofs++;
                bofs++;
            }
            sRow++;
            bofs += w - rendered_width;
            mask_ofs += w - rendered_width;
            ofs += WIDTH - rendered_width;
        }
        break;

    case SPRITE_PLUS_MASK:
        
        break;
    }
}
