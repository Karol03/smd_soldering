#pragma once

#include <LiquidCrystal.h>

#include "defines.hpp"
#include "glyphs.hpp"
#include "profile.hpp"


LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);


inline void lcd_init()
{
    digitalWrite(LCD_OFF, HIGH);
    lcd.begin(16, 2);
    lcd.display();
    lcd.clear();
    lcd.createChar(RIGHT_SLASH, RIGHT_SLASH_GLYPH);
    lcd.createChar(LEFT_SLASH, LEFT_SLASH_GLYPH);
    lcd.createChar(ARROW_UP, ARROW_UP_GLYPH);
    lcd.createChar(ARROW_DOWN, ARROW_DOWN_GLYPH);
    lcd.createChar(DEGREE, DEGREE_GLYPH);
}

inline void lcd_print_menu(const uint8_t option = 0)
{
    if (option == 0)
    {
        lcd.setCursor(0, 0);
        lcd.write(byte(RIGHT_SLASH));
        lcd.print("     Start    ");
        lcd.write(byte(LEFT_SLASH));
        lcd.setCursor(0, 1);
        lcd.write(byte(LEFT_SLASH));
        lcd.print("              ");
        lcd.write(byte(RIGHT_SLASH));
    }
    else if (option == 1)
    {
        lcd.setCursor(0, 0);
        lcd.write(byte(RIGHT_SLASH));
        lcd.print("    Thermal   ");
        lcd.write(byte(LEFT_SLASH));
        lcd.setCursor(0, 1);
        lcd.write(byte(LEFT_SLASH));
        lcd.print("    Profile   "); 
        lcd.write(byte(RIGHT_SLASH));
    }
}

inline void lcd_print_profile(const uint8_t number = 0)
{
    if (number == 0)
    {
        lcd.setCursor(0, 0);
        lcd.write(byte(RIGHT_SLASH));
        lcd.print("    Profile   ");
        lcd.write(byte(LEFT_SLASH));
        lcd.setCursor(0, 1);
        lcd.write(byte(LEFT_SLASH));
        lcd.print("              ");
        lcd.write(byte(RIGHT_SLASH));
    }
    else if (number == 1)
    {
        const char prof1[15] = " 1. Sn63Pb37  ";
        lcd.setCursor(1, 1);
        lcd.print(prof1);
    }
}

inline byte difference_char(int8_t val)
{
    if (val < 0)
    {
        return ARROW_DOWN;
    }
    else if (val == 0)
    {
        return '-';
    }
    else
    {
        return ARROW_UP;
    }
}

inline void print_row(char row[], uint8_t opt, const Stat_t* value = NULL)
{
    /**
    * lcd.print("Time 3600(3000)s");
    * lcd.print("Temp.  300°C (↗)");
    * lcd.print("Profile       1.");
    *  **
    * lcd.print("Temp.  300°C (-)");
    * lcd.print("Temp.  300°C (↘)");
    **/

    if (opt == 0)
    {
        if (value->expectedTime > 1000)
        {
            sprintf(row, "Time %4d(%4d)s", value->workTime, value->expectedTime);
        }
        else if (value->expectedTime > 100)
        {
            sprintf(row, "Time %5d(%3d)s", value->workTime, value->expectedTime);
        }
        else if (value->expectedTime > 10)
        {
            sprintf(row, "Time %6d(%2d)s", value->workTime, value->expectedTime);
        }
    }
    else if (opt == 1)
    {
        sprintf(row, "Temp.:%4d*C (-)", value->temperature);
        row[10] = DEGREE;
        row[14] = difference_char(value->tempDifferential);
    }
    else if (opt == 2)
    {
        sprintf(row, "Profile%8d.", value->profile);
    }
    else if (opt == 3)
    {
        if (value->fanSpeed < 0)
        {
            sprintf(row, "Fan disconnected");
        }
        else
        {
            const uint8_t fanSpeed = (value->fanSpeed > 100 ? 100 : value->fanSpeed);
            sprintf(row, "Fan%12d%%", fanSpeed);
        }
    }
    else
    {
      row[0] = '\0';
    }
}

inline void lcd_print_soldering(const Stat_t* stats = NULL, const uint8_t view = 0)
{
    const uint8_t MAX_VIEW_ROW_OPT = 3;
  
    char row1[17];
    char row2[17];

    if (stats == NULL)
    {
        sprintf(row1, "Start soldering?");
        sprintf(row2, " <no>     <yes> ");
    }
    else if (view == 255)
    {
        sprintf(row1, "    Operation   ");
        sprintf(row2, "     stopped    ");
    }
    else if (view == 254)
    {
        sprintf(row1, "    Soldering   ");
        sprintf(row2, "    finished    ");
    }
    else
    {
        print_row(row1, view % MAX_VIEW_ROW_OPT, stats);
        print_row(row2, (view + 1) % MAX_VIEW_ROW_OPT, stats);
    }

    lcd.setCursor(0, 0);
    lcd.print(row1);
    lcd.setCursor(0, 1);
    lcd.print(row2);
}
