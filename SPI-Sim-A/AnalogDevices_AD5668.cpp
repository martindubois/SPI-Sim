
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   SPI-Sim
// File      SPI-Sim-A/AnalogDevices_AD5668.cpp

// TEST COVERAGE 2022-11-29 KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <SPI-Sim/AnalogDevices/AD5668.h>

// ===== Common =============================================================
#include "../Common/Protocol.h"

using namespace KMS;

namespace SPI_Sim
{
    namespace AnalogDevices
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const DAQ::Id AD5668::ID_AI_A = 0;
        const DAQ::Id AD5668::ID_AI_B = 1;
        const DAQ::Id AD5668::ID_AI_C = 2;
        const DAQ::Id AD5668::ID_AI_D = 3;
        const DAQ::Id AD5668::ID_AI_E = 4;
        const DAQ::Id AD5668::ID_AI_F = 5;
        const DAQ::Id AD5668::ID_AI_G = 6;
        const DAQ::Id AD5668::ID_AI_H = 7;

        AD5668::AD5668(float aRef_V)
            : Chip(CHIP_ANALOG_DEVICES_AD5668, "AnalogDevices", "AD5668")
            , mRef_V(aRef_V)
        {
            assert(0.0 < aRef_V);
        }

        // ===== DAQ::IAnalogInputs =========================================

        DAQ::AnalogValue AD5668::AI_Read(DAQ::Id aId)
        {
            DAQ::AnalogValue lResult_V = AI_Read_Raw(aId);

            lResult_V /= 0xffff;
            lResult_V *= mRef_V;

            return lResult_V;
        }

        DAQ::AnalogValue_Raw AD5668::AI_Read_Raw(DAQ::Id aId)
        {
            return GetValue(aId);
        }

        // ===== Chip =======================================================

        WOP::Object* AD5668::GetInstance() { return this; }

        void AD5668::Dump()
        {
            Chip::Dump();

            for (uint8_t i = 0; i < 8; i++)
            {
                std::cout << "  Channel " << static_cast<char>('A' + i) << " = " << AI_Read(i) << " V\t(" << AI_Read_Raw(i) << "\n";
            }

            std::cout << std::endl;
        }

    }
}
