
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   SPI-Sim
// File      Includes/SPI_Sim/AnalogDevices/AD7689.h

#pragma once

// ===== Import/Includes ====================================================
#include <KMS/DAQ/IAnalogOutputs.h>
#include <KMS/WOP/ValueArray.h>

// ===== Includes ===========================================================
#include <SPI-Sim/Chip.h>

namespace SPI_Sim
{
    namespace AnalogDevices
    {

        class AD7689 : public Chip, public KMS::WOP::ValueArray<uint16_t, 9>, public KMS::DAQ::IAnalogOutputs
        {

        public:

            static const KMS::DAQ::Id ID_AO_0;
            static const KMS::DAQ::Id ID_AO_1;
            static const KMS::DAQ::Id ID_AO_2;
            static const KMS::DAQ::Id ID_AO_3;
            static const KMS::DAQ::Id ID_AO_4;
            static const KMS::DAQ::Id ID_AO_5;
            static const KMS::DAQ::Id ID_AO_6;
            static const KMS::DAQ::Id ID_AO_7;
            static const KMS::DAQ::Id ID_AO_8;

            AD7689(float aRef_V = 2.5);

            // ===== KMS::DAQ::IAnalogInputs ================================
            virtual KMS::DAQ::AnalogValue     AO_Get      (KMS::DAQ::Id aId) const;
            virtual KMS::DAQ::AnalogValue_Raw AO_Get_Raw  (KMS::DAQ::Id aId) const;
            virtual void                      AO_Write    (KMS::DAQ::Id aId, KMS::DAQ::AnalogValue     aValue_V);
            virtual void                      AO_Write_Raw(KMS::DAQ::Id aId, KMS::DAQ::AnalogValue_Raw aValue);

            // ===== Chip ===================================================
            virtual KMS::WOP::Object* GetInstance();
            virtual void Dump();
            virtual void ExecuteCommand(const char* aCmd);

        private:

            float mRef_V;

        };

    }
}
