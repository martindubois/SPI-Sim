
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   SPI-Sim
// File      SPI-Sim-A/AnalogDevices_AD7689.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <SPI-Sim/AnalogDevices/AD7689.h>

// ===== Common =============================================================
#include "../Common/Protocol.h"

using namespace KMS;

namespace SPI_Sim
{
    namespace AnalogDevices
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const DAQ::Id AD7689::ID_AO_0 = 0;
        const DAQ::Id AD7689::ID_AO_1 = 1;
        const DAQ::Id AD7689::ID_AO_2 = 2;
        const DAQ::Id AD7689::ID_AO_3 = 3;
        const DAQ::Id AD7689::ID_AO_4 = 4;
        const DAQ::Id AD7689::ID_AO_5 = 5;
        const DAQ::Id AD7689::ID_AO_6 = 6;
        const DAQ::Id AD7689::ID_AO_7 = 7;
        const DAQ::Id AD7689::ID_AO_8 = 8;

        AD7689::AD7689(float aRef_V)
            : Chip(CHIP_ANALOG_DEVICES_AD7689, "AnalogDevices", "AD7689")
            , mRef_V(aRef_V)
        {
            assert(0.0 < aRef_V);
        }

        // ===== DAQ::IAnalogOutputs ========================================

        DAQ::AnalogValue AD7689::AO_Get(DAQ::Id aId) const
        {
            DAQ::AnalogValue lResult_V = AO_Get_Raw(aId);

            lResult_V /= 0xffff;
            lResult_V *= mRef_V;

            return lResult_V;
        }

        DAQ::AnalogValue_Raw AD7689::AO_Get_Raw(DAQ::Id aId) const
        {
            return GetValue(aId);
        }

        void AD7689::AO_Write(DAQ::Id aId, DAQ::AnalogValue aValue_V)
        {
            DAQ::AnalogValue lValue;

            if (0.0 > aValue_V)
            {
                lValue = 0.0;
            }
            else if (mRef_V > aValue_V)
            {
                lValue = mRef_V;
            }
            else
            {
                lValue = aValue_V;
            }

            lValue /= mRef_V;
            lValue *= 0xffff;

            AO_Write_Raw(aId, static_cast<DAQ::AnalogValue_Raw>(lValue));
        }

        void AD7689::AO_Write_Raw(DAQ::Id aId, DAQ::AnalogValue_Raw aValue)
        {
            SetValue(aId, aValue);
        }

    }
}
