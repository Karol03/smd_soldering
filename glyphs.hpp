#pragma once


#define RIGHT_SLASH 0
byte RIGHT_SLASH_GLYPH[] =
{
    0b00001,
    0b00010,
    0b00010,
    0b00100,
    0b00100,
    0b01000,
    0b01000,
    0b10000
};

#define LEFT_SLASH 1
byte LEFT_SLASH_GLYPH[] =
{
    0b10000,
    0b01000,
    0b01000,
    0b00100,
    0b00100,
    0b00010,
    0b00010,
    0b00001
};

#define ARROW_UP 2
byte ARROW_UP_GLYPH[] =
{
    0b00000,
    0b00100,
    0b01110,
    0b10101,
    0b00100,
    0b00100,
    0b00100,
    0b00000
};

#define ARROW_DOWN 3
byte ARROW_DOWN_GLYPH[] =
{
    0b00000,
    0b00100,
    0b00100,
    0b00100,
    0b10101,
    0b01110,
    0b00100,
    0b00000
};

#define DEGREE 4
byte DEGREE_GLYPH[] =
{
    0b00110,
    0b01001,
    0b01001,
    0b00110,
    0b00000,
    0b00000,
    0b00000,
    0b00000
};
