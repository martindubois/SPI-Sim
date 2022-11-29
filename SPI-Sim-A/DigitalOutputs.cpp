
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   SPI-Sim
// File      SPI-Sim-A/DigitalOutputs.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <SPI-Sim/DigitalOutputs.h>

using namespace KMS;

namespace SPI_Sim
{

    // Public
    // //////////////////////////////////////////////////////////////////////

    unsigned int DigitalOutputs::GetCount() const { return 16; }

    void DigitalOutputs::Dump() const
    {
        uint16_t lBits = GetBits(0xffff);

        for (unsigned int i = 0; i < 16; i++)
        {
            std::cout << "DO #" << i << " = " << (TestBits(1 << i) ? "true" : "false") << "\n";
        }

        std::cout << std::endl;
    }

    // ===== DAQ::IDigitalInputs ============================================

    void DigitalOutputs::DO_Clear(DAQ::Id aId) { ClearBits(1 << aId); }

    bool DigitalOutputs::DO_Get(DAQ::Id aId) const { return TestBits(1 << aId); }

    void DigitalOutputs::DO_Set(DAQ::Id aId, bool aValue)
    {
        uint16_t lBit = 1 << aId;

        if (aValue)
        {
            SetBits(lBit);
        }
        else
        {
            ClearBits(lBit);
        }
    }

}
