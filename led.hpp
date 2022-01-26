#pragma once

#include "defines.hpp"


void led_blink(bool red = false, bool green = false, bool blue = false, bool enable = false)
{
    static uint32_t timestamp = millis();
    static uint8_t rgb = 0;

    if (enable)
    {
        rgb = (red << 2 | green << 1 | blue);
    }
    else if (rgb != 0 && millis() - timestamp > 500)
    {
        timestamp = millis();

        if (rgb & 0x8)
        {
            rgb &= 0x7;
            digitalWrite(LED_R, LOW);
            digitalWrite(LED_G, LOW);
            digitalWrite(LED_B, LOW);
        }
        else
        {
            rgb |= 0x8;
            digitalWrite(LED_R, (rgb & 0x4));
            digitalWrite(LED_G, (rgb & 0x2));
            digitalWrite(LED_B, (rgb & 0x1));
        }
    }
}

void led_set(bool red = false, bool green = false, bool blue = false)
{
    led_blink(0, 0, 0, true);
    digitalWrite(LED_R, red);
    digitalWrite(LED_G, green);
    digitalWrite(LED_B, blue);
}
