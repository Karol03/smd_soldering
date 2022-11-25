#pragma once

#include<DFRobotHighTemperatureSensor.h>

#include "defines.hpp"
#include "led.hpp"
#include "profiles.hpp"


struct Stat_t
{
    uint16_t expectedTime    = 0;
    uint16_t workTime        = 0;
    int16_t temperature      = 0;
    int8_t profile           = 0;
    int8_t tempDifferential  = 0;
    int8_t fanSpeed          = -1;
};


class Profile
{
public:
    Profile()
    {
        m_stats.profile = 1;
    }

    inline void init(uint8_t number)
    {
        m_stats.profile = number;
    }

    inline void start()
    {
        m_isStopped = false;
      
        if (m_stats.profile == 1)
        {
            m_startTimestamp = millis();
            m_stats.expectedTime = 330;
            led_set(COLOR_RED);
        }
        else
        {
            digitalWrite(RELAY, LOW);
            led_blink(COLOR_BLUE);
        }
    }

    inline bool run()
    {
        if (m_stats.workTime >= PROFILES_SIZE[m_stats.profile] - 1)
        {
            return false;
        }
      
        const float temp_value = temperature();
        m_stats.workTime = ((millis() - m_startTimestamp) / 1000);

        const float value = PROFILES[m_stats.profile][m_stats.workTime];
        const float nextValue = PROFILES[m_stats.profile][m_stats.workTime + 1];
        const float dvalue = 0.1f * value;
        const float value_min = value - dvalue;
        const float value_max = value + dvalue;

        if (temp_value < value_min)
        {
            digitalWrite(RELAY, HIGH);
        }
        else if (temp_value > value_max)
        {
            digitalWrite(RELAY, LOW);
        }

        if (value < nextValue)
        {
            m_stats.tempDifferential = 1;
        }
        else if (value > nextValue)
        {
            m_stats.tempDifferential = -1;
        }
        else
        {
            m_stats.tempDifferential = 0;
        }
        m_stats.temperature = temp_value;

        m_stats.fanSpeed = digitalRead(FAN) * 100;
        return true;
    }

    inline void stop()
    {
        m_isStopped = true;
        digitalWrite(RELAY, LOW);
    }

    inline const Stat_t& stats() const
    {
        return m_stats;
    }

    inline bool is_cold() const
    {
        return temperature() < 50.0f;
    }

    inline bool is_stopped() const
    {
        return m_isStopped;
    }

private:
    inline float temperature() const
    {
        const float voltageRef = 5.000f;
        DFRobotHighTemperature pt100 = DFRobotHighTemperature(voltageRef);
        return pt100.readTemperature(PT100);
    }

private:
    Stat_t m_stats;
    uint32_t m_startTimestamp = {};
    bool m_isStopped = false;
};
