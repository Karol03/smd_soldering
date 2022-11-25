#pragma once

#include "button.hpp"
#include "lcd.hpp"
#include "led.hpp"
#include "profile.hpp"


#define PREV(val, MAX) val = (val - 1 + MAX) % MAX;
#define NEXT(val, MAX) val = (val + 1) % MAX;


/*
 *    bn - button next pressed
 *    bnh - button next hold
 *    bp - button prev pressed
 *    bph - button prev hold
 *    
 *    OPT (...) - menuOption value
 *    OPT (u) - menuOption value different than selected
 *    
 * 
 * 
 * 
 *    O -> MENU TRANSITION (print menu) ->   MENU    <--_  <--
 *     /                                  /    |   \      \    \ 
 *     \                                bnh    bn   bp     \    \
 *      \                              /        \     \    /     \
 *       \                    --- MENU SELECT     --------        \
 *        \                 /      /     \                         \
 *          ---- OPT(u) ---     OPT(0)   OPT(1)                     \
 *                               /         \                         \
 *                         SET HEATING    SET PROFILE ---- bn ->      | 
 *                              |          /    |   \ \          \    |
 *                              |        bph   bnh   \ \ -- bp -->\   |
 *                          HEAT START    \      \    \___________/  /
 *                           /       \     \->____\->______________ / 
 *                     bn || bnh   bp || bph                       /        
 *                         |            \________________________ /              
 *                    START SOLDERING                              
 *                                                    
 *                                                    
  */

Profile profile;


void menu_main(void*);


inline void menu_state_machine(void(*next)(void*) = NULL, void* args = NULL)
{
    static void(*func)(void*) = NULL;

    if (next != NULL)
    {
        func = next;
    }
    else if (func != NULL)
    {
        func(args);
    }
}

inline void menu_soldering(void*)
{
    const uint8_t MAX_VIEWS = 3;
    static uint8_t view = 0;
    static uint32_t timestamp = millis();

    const ButtonPress button = getButton();
    if (button == EButton_next_pressed)
    {
        NEXT(view, MAX_VIEWS)
        lcd_print_soldering(&profile.stats(), view);
        timestamp = millis();
    }
    else if (button == EButton_prev_pressed)
    {
        PREV(view, MAX_VIEWS)
        lcd_print_soldering(&profile.stats(), view);
        timestamp = millis();
    }
    else if (button == EButton_prev_hold)
    {
        view = 255;
        lcd_print_soldering(&profile.stats(), view);
        view = 0;
        profile.stop();
        menu_state_machine(menu_main);
        delay(1700);
        return;
    }

    if (!profile.run())
    {
        view = 254;
        lcd_print_soldering(&profile.stats(), view);
        view = 0;
        profile.stop();
        menu_state_machine(menu_main);
        delay(1700);
        return;
    }

    if (millis() - timestamp > 1000)
    {
        lcd_print_soldering(&profile.stats(), view);
        timestamp = millis();
    }

    delay(50);
}

inline void menu_set_heating(void*)
{
    const ButtonPress button = getButton();
    if (button == EButton_next_pressed)
    {
        profile.start();
        lcd_print_soldering(&profile.stats());
        menu_state_machine(menu_soldering);
    }
    else if (button == EButton_prev_pressed)
    {
        menu_state_machine(menu_main);
    }
}

inline void menu_set_profile(void*)
{
    const uint8_t MAX_PROFILES = 1;
    static uint8_t profileNumber = 1;

    const ButtonPress button = getButton();
    if (button == EButton_next_pressed)
    {
        NEXT(profileNumber, MAX_PROFILES)
        if (profileNumber == 0)
        {
            profileNumber = 1;
        }
        lcd_print_profile(profileNumber);
    }
    else if (button == EButton_prev_pressed)
    {
        PREV(profileNumber, MAX_PROFILES)
        if (profileNumber == 0)
        {
            profileNumber = MAX_PROFILES;
        }
        lcd_print_profile(profileNumber);
    }
    else if (button == EButton_next_hold)
    {
        profile.init(profileNumber);
        profileNumber = 1;
        menu_state_machine(menu_main);
    }
    else if (button == EButton_prev_hold)
    {
        profileNumber = 1;
        menu_state_machine(menu_main);
    }
}

inline void menu_main(void*)
{
    const uint8_t MAX_MENU_OPTIONS = 3;
    static uint8_t option = 0;
    static uint8_t lastOption = -1;
    static bool isLedSet = false;

    if (!isLedSet && profile.is_cold())
    {
        isLedSet = true;
        led_set(COLOR_GREEN);
    }
    else if (!profile.is_stopped())
    {
        profile.stop();
        if (!profile.is_cold())
        {
            led_blink(COLOR_RED, true);
        }
    }
    else if (!profile.is_cold())
    {
        isLedSet = false;
    }

    if (option != lastOption)
    {
        lastOption = option;
        lcd_print_menu(option);
    }

    const ButtonPress button = getButton();
    if (button == EButton_next_pressed)
    {
        NEXT(option, MAX_MENU_OPTIONS)
    }
    else if (button == EButton_prev_pressed)
    {
        PREV(option, MAX_MENU_OPTIONS)
    }
    else if (button == EButton_next_hold)
    {
        isLedSet = false;
        lastOption = -1;
        if (option == 0)
        {
            lcd_print_soldering();
            menu_state_machine(menu_set_heating);
        }
        else if (option == 1)
        {
            lcd_print_profile();
            menu_state_machine(menu_set_profile);
        }
        option = 0;
    }
}
