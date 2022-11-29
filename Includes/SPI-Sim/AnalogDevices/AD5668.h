
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   SPI-Sim
// File      Includes/SPI_Sim/AnalogDevices/AD5668.h

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

        class AD5668 : public Chip, public KMS::WOP::ValueArray<uint16_t, 8>, public KMS::DAQ::IAnalogInputs
        {

        public:

            static const KMS::DAQ::Id ID_AI_A;
            static const KMS::DAQ::Id ID_AI_B;
            static const KMS::DAQ::Id ID_AI_C;
            static const KMS::DAQ::Id ID_AI_D;
            static const KMS::DAQ::Id ID_AI_E;
            static const KMS::DAQ::Id ID_AI_F;
            static const KMS::DAQ::Id ID_AI_G;
            static const KMS::DAQ::Id ID_AI_H;

            AD5668(float aRef_V = 2.5);

            // ===== KMS::DAQ::IAnalogInputs ================================
            virtual KMS::DAQ::AnalogValue     AI_Read    (KMS::DAQ::Id aId);
            virtual KMS::DAQ::AnalogValue_Raw AI_Read_Raw(KMS::DAQ::Id aId);

            // ===== Chip ===================================================
            virtual KMS::WOP::Object* GetInstance();
            virtual void Dump();

        private:

            float mRef_V;

        };

    }
}
