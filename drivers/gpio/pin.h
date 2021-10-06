/**
 * 
 */

#pragma once

#include <cstdint>

namespace NRF52 {

class Pin
{
public:
    enum class Direction : uint32_t
    {
        Input = 0,
        Output = 1
    };

    enum class InputConnection : uint32_t
    {
        Connect = 0,
        Disconnect = 1
    };

    enum class DriveStrength : uint32_t
    {
        S0S1 = 0,
        H0S1 = 1,
        S0H1 = 2,
        H0H1 = 3,
        D0S1 = 4,
        D0H1 = 5,
        S0D1 = 6,
        H0D1 = 7
    };

    enum class PullMode : uint32_t
    {
        Disabled = 0,
        PullDown = 1,
        PullUp = 3
    };

    enum class Sense : uint32_t
    {
        Disabled = 0,
        High = 2,
        Low = 3
    };

public:
    Pin() = delete;
    ~Pin() = default;

    Pin(uint8_t pin) : m_pin(pin){}

    Pin(const Pin& rhs) = delete;           // We can't copy a pin
    Pin(Pin&& rhs) = delete;                // We can't move a pin
    Pin& operator=(const Pin&) = delete;    // We can't copy assign a pin
    Pin& operator=(Pin&&) = delete;         // We can't move assign a pin

    void set_direction(Direction direction);
    void set_input_buffer_mode(InputConnection mode);
    void set_drive_strength(DriveStrength strength);
    void set_pull_mode(PullMode mode);
    void set_sense_mode(Sense mode);
    void set_high();
    void set_low();

    uint8_t read() const;

    uint8_t pin_number() const { return m_pin; };
    Direction direction() const { return m_direction; }
    InputConnection input_buffer_mode() const { return m_connection; }
    DriveStrength drive_strength() const { return m_drive_strength; }
    PullMode pull_mode() const { return m_pull_mode; }
    Sense sense_mode() const { return m_sense_mode; }

private:
    uint8_t m_pin;
    Direction m_direction { Direction::Input };
    InputConnection m_connection { InputConnection::Disconnect };
    DriveStrength m_drive_strength { DriveStrength::S0S1 };
    PullMode m_pull_mode { PullMode::Disabled };
    Sense m_sense_mode { Sense::Disabled };
};

}