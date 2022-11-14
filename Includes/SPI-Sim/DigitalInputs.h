
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   SPI-Sim
// File      Includes/SPI-Sim/DigitalInputs.h

#pragma once

// ===== Import/Includes ====================================================
#include <KMS/DAQ/IDigitalInputs.h>
#include <KMS/WOP/BitArray.h>

namespace SPI_Sim
{

    class DigitalInputs : public KMS::WOP::BitArray<uint16_t>, public KMS::DAQ::IDigitalInputs
    {

    public:

        static const KMS::DAQ::Id ID_DI_00;
        static const KMS::DAQ::Id ID_DI_01;
        static const KMS::DAQ::Id ID_DI_02;
        static const KMS::DAQ::Id ID_DI_03;
        static const KMS::DAQ::Id ID_DI_04;
        static const KMS::DAQ::Id ID_DI_05;
        static const KMS::DAQ::Id ID_DI_06;
        static const KMS::DAQ::Id ID_DI_07;
        static const KMS::DAQ::Id ID_DI_08;
        static const KMS::DAQ::Id ID_DI_09;
        static const KMS::DAQ::Id ID_DI_10;
        static const KMS::DAQ::Id ID_DI_11;
        static const KMS::DAQ::Id ID_DI_12;
        static const KMS::DAQ::Id ID_DI_13;
        static const KMS::DAQ::Id ID_DI_14;
        static const KMS::DAQ::Id ID_DI_15;

        unsigned int GetCount() const;

        void Dump() const;

        // ===== KMS::DAQ::IDigitalInputs ===================================
        virtual bool DI_Read(KMS::DAQ::Id aId);

    };

}
