
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   SPI-Sim
// File      SPI-Sim-A/Chip.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <SPI-Sim/AnalogDevices/AD5162.h>
#include <SPI-Sim/AnalogDevices/AD5668.h>
#include <SPI-Sim/AnalogDevices/AD7689.h>

// ===== Local ==============================================================
#include "../Common/Protocol.h"

namespace SPI_Sim
{

    // Public
    // //////////////////////////////////////////////////////////////////////

    Chip* Chip::Create(uint16_t aType)
    {
        Chip* lResult = NULL;

        switch (aType)
        {
        case CHIP_ANALOG_DEVICES_AD5162: lResult = new AnalogDevices::AD5162(); break;
        case CHIP_ANALOG_DEVICES_AD5668: lResult = new AnalogDevices::AD5668(); break;
        case CHIP_ANALOG_DEVICES_AD7689: lResult = new AnalogDevices::AD7689(); break;
        }

        return lResult;
    }

    Chip::~Chip() {}

    const char* Chip::GetManufacturer() const { assert(NULL != mManufacturer); return mManufacturer; }
    const char* Chip::GetName        () const { assert(NULL != mName        ); return mName        ; }

    void Chip::Dump()
    {
        std::cout << "Manufacturer : " << mManufacturer << "\n";
        std::cout << "Name         : " << mName << "\n";
        std::cout << "Type         : 0x" << std::hex << mType << std::dec << std::endl;
    }

    void Chip::ExecuteCommand(const char* aCmd)
    {
        if (0 == _stricmp(aCmd, "Dump")) { Dump(); }
        else
        {
            KMS_EXCEPTION(APPLICATION_ERROR, "Invalid command", aCmd);
        }
    }

    // Protected
    // //////////////////////////////////////////////////////////////////////

    Chip::Chip(uint16_t aType, const char* aManufacturer, const char* aName)
        : mManufacturer(aManufacturer)
        , mName(aName)
        , mType(aType)
    {
        assert(NULL != aManufacturer);
        assert(NULL != aName);
    }

}
