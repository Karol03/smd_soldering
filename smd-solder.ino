#include "defines.hpp"
#include "lcd.hpp"
#include "menu.hpp"


void setup()
{
    pinMode(LCD_OFF, OUTPUT);

    pinMode(LED_R, OUTPUT);
    pinMode(LED_G, OUTPUT);
    pinMode(LED_B, OUTPUT);

    pinMode(BTN_NEXT, INPUT_PULLUP);
    pinMode(BTN_PREV, INPUT_PULLUP);

    pinMode(RELAY, OUTPUT);
    pinMode(FAN, INPUT);

    lcd_init();

    menu_state_machine(menu_main);
}

void loop()
{
    menu_state_machine();
    led_blink();
    delay(10);
}
