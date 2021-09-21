/**
 * 
 */

#pragma once

#include <cstdint>

typedef void (*isr_callback)(void);

namespace NRF52
{

enum class DeviceStatus : int32_t
{
    SUCCESS = 0,
    INVALID_PARAMETER = -1,
    DEVICE_BUSY = -2,
    ALREADY_INITIALIZED = -3,
    NOT_INITIALIZED = -4,
    UNIMPLEMENTED = -5,
};

//
// Device Base class
//
class Device
{
public:
    Device() = delete; // Default constructor deleted, a device requires a base address
    Device(uint32_t base_address) : m_base_address(base_address){}
    virtual ~Device(){}

    virtual DeviceStatus initialize() = 0;
    virtual DeviceStatus uninitialize() = 0;
    
    uint32_t base_address() const { return m_base_address; }
    bool initialized() const { return m_initialized; }

protected:
    uint32_t m_base_address { 0 }; // Device base address
    bool m_initialized { false };
};

}