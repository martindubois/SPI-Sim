
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   SPI-Sim
// File      Includes/SPI-Sim/DigitalOutputs.h

#pragma once

// ===== Import/Includes ====================================================
#include <KMS/DAQ/IDigitalOutputs.h>
#include <KMS/WOP/BitArray.h>

namespace SPI_Sim
{

    class DigitalOutputs : public KMS::WOP::BitArray<uint16_t>, public KMS::DAQ::IDigitalOutputs
    {

    public:

        static const KMS::DAQ::Id ID_DO_00;
        static const KMS::DAQ::Id ID_DO_01;
        static const KMS::DAQ::Id ID_DO_02;
        static const KMS::DAQ::Id ID_DO_03;
        static const KMS::DAQ::Id ID_DO_04;
        static const KMS::DAQ::Id ID_DO_05;
        static const KMS::DAQ::Id ID_DO_06;
        static const KMS::DAQ::Id ID_DO_07;
        static const KMS::DAQ::Id ID_DO_08;
        static const KMS::DAQ::Id ID_DO_09;
        static const KMS::DAQ::Id ID_DO_10;
        static const KMS::DAQ::Id ID_DO_11;
        static const KMS::DAQ::Id ID_DO_12;
        static const KMS::DAQ::Id ID_DO_13;
        static const KMS::DAQ::Id ID_DO_14;
        static const KMS::DAQ::Id ID_DO_15;

        unsigned int GetCount() const;

        void Dump() const;

        // ===== KMS::DAQ::IDigitalOutputs ==================================
        virtual void DO_Clear(KMS::DAQ::Id aId);
        virtual bool DO_Get  (KMS::DAQ::Id aId) const;
        virtual void DO_Set  (KMS::DAQ::Id aId, bool aValue = true);

    };

}
