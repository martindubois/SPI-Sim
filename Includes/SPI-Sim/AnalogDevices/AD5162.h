
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   SPI-Sim
// File      Includes/SPI_Sim/AnalogDevices/AD5162.h

#pragma once

// ===== Import/Includes ====================================================
#include <KMS/DAQ/IAnalogInputs.h>
#include <KMS/WOP/ValueArray.h>

// ===== Includes ===========================================================
#include <SPI-Sim/Chip.h>

namespace SPI_Sim
{
    namespace AnalogDevices
    {

        class AD5162 : public Chip, public KMS::WOP::ValueArray<uint8_t, 2>, public KMS::DAQ::IAnalogInputs
        {

        public:

            static const KMS::DAQ::Id ID_AI_R_A1_W1;
            static const KMS::DAQ::Id ID_AI_R_B1_W1;
            static const KMS::DAQ::Id ID_AI_R_B2_W2;

            AD5162(float aEndToEnd_ohm = 10000.0);

            // ===== KMS::DAQ::IAnalogInputs ================================
            virtual KMS::DAQ::AnalogValue     AI_Read    (KMS::DAQ::Id aId);
            virtual KMS::DAQ::AnalogValue_Raw AI_Read_Raw(KMS::DAQ::Id aId);

            // ===== Chip ===================================================
            virtual void Dump();

        private:

            float mEndToEnd_ohm;

        };

    }
}
