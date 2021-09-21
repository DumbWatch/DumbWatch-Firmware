/**
 * 
 */

#pragma once

#include <cstdint>
#include <cstdlib>
#include <drivers/device.h>

namespace NRF52
{

//
// Device Base class
//
class SerialDevice : public Device
{
public:
    SerialDevice() = delete;
    SerialDevice(uint32_t base_address) : Device(base_address){}
    virtual ~SerialDevice(){}

    virtual DeviceStatus receive(uint8_t* data, size_t count) = 0;
    virtual DeviceStatus send(const uint8_t* data, size_t count) = 0;
    virtual DeviceStatus transceive(const uint8_t* txbuf, uint8_t* rxbuf, size_t count) = 0;

protected:
};

}