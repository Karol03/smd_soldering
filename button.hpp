#pragma once

#include "defines.hpp"


enum ButtonPress : uint8_t
{
    EButton_not_pressed    = 0,
    EButton_pressed        = 1,
    EButton_hold           = 2,
    EButton_next_pressed   = 3,
    EButton_next_hold      = 4,
    EButton_prev_pressed   = 5,
    EButton_prev_hold      = 6
};


inline ButtonPress getButtonAction(int button, uint32_t& timestamp)
{
    if (digitalRead(button) == LOW && timestamp == 0)
    {
        timestamp = millis();
    }
    else if (digitalRead(button) == HIGH && timestamp != 0)
    {
        const uint32_t difference = millis() - timestamp;
        if (difference > 550)
        {
            return EButton_hold;
        }
        else
        {
            return EButton_pressed;
        }
    }
    return EButton_not_pressed;
}


inline ButtonPress getButton()
{
    static uint32_t buttonNextTS = 0;
    static uint32_t buttonPrevTS = 0;

    const ButtonPress resultNext = getButtonAction(BTN_NEXT, buttonNextTS);
    const ButtonPress resultPrev = getButtonAction(BTN_PREV, buttonPrevTS);

    if (resultNext != EButton_not_pressed)
    {
        buttonNextTS = 0;
        buttonPrevTS = 0;
        if (resultNext == EButton_pressed)
        {
            return EButton_next_pressed;
        }
        else
        {
            return EButton_next_hold;
        }
    }
    else if (resultPrev != EButton_not_pressed)
    {
        buttonNextTS = 0;
        buttonPrevTS = 0;
        if (resultPrev == EButton_pressed)
        {
            return EButton_prev_pressed;
        }
        else
        {
            return EButton_prev_hold;
        }
    }
    return EButton_not_pressed;
}
