
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   SPI-Sim
// File      SPI-Sim-A/AnalogDevices_AD5162.cpp

// TEST COVERAGE 2022-11-29 KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <SPI-Sim/AnalogDevices/AD5162.h>

// ===== Common =============================================================
#include "../Common/Protocol.h"

using namespace KMS;

namespace SPI_Sim
{
    namespace AnalogDevices
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const DAQ::Id AD5162::ID_AI_R_A1_W1 = 0;
        const DAQ::Id AD5162::ID_AI_R_B1_W1 = 1;
        const DAQ::Id AD5162::ID_AI_R_B2_W2 = 2;

        AD5162::AD5162(float aEndToEnd_ohm)
            : Chip(CHIP_ANALOG_DEVICES_AD5162, "AnalogDevices", "AD5162")
            , mEndToEnd_ohm(aEndToEnd_ohm)
        {}

        // ===== DAQ::IAnalogInputs =========================================

        DAQ::AnalogValue AD5162::AI_Read(DAQ::Id aId)
        {
            DAQ::AnalogValue lResult_ohm = AI_Read_Raw(aId);

            lResult_ohm /= 0xff;
            lResult_ohm *= mEndToEnd_ohm;

            return lResult_ohm;
        }

        DAQ::AnalogValue_Raw AD5162::AI_Read_Raw(DAQ::Id aId)
        {
            DAQ::AnalogValue_Raw lResult;

            switch (aId)
            {
            case ID_AI_R_A1_W1: lResult = 0xff - this->GetValue(0); break;
            case ID_AI_R_B1_W1: lResult = this->GetValue(0); break;
            case ID_AI_R_B2_W2: lResult = this->GetValue(1); break;

            default:
                // NOT TESTED
                KMS_EXCEPTION(APPLICATION_ERROR, "Invalid analog input id", "");
            }

            return lResult;
        }

        // ===== Chip =======================================================

        WOP::Object* AD5162::GetInstance() { return this; }

        void AD5162::Dump()
        {
            Chip::Dump();

            std::cout << "  R A1 to W1 = " << AI_Read(ID_AI_R_A1_W1) << " ohm\n";
            std::cout << "  R B1 to W1 = " << AI_Read(ID_AI_R_B1_W1) << " ohm\n";
            std::cout << "  R B2 to W2 = " << AI_Read(ID_AI_R_B2_W2) << " ohm" << "\n" << std::endl;
        }
    }
}
