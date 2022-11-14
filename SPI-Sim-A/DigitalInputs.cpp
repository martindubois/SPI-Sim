
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   SPI-Sim
// File      SPI-Sim-A/DigitalInputs.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <SPI-Sim/DigitalInputs.h>

namespace SPI_Sim
{

    // Public
    // //////////////////////////////////////////////////////////////////////

    unsigned int DigitalInputs::GetCount() const { return 16; }

    void DigitalInputs::Dump() const
    {
        uint16_t lBits = GetBits(0xffff);

        for (unsigned int i = 0; i < 16; i++)
        {
            std::cout << "DI #" << i << " = " << (TestBits(1 << i) ? "true" : "false") << "\n";
        }

        std::cout << std::endl;
    }

    // ===== KMS::DAQ::IDigitalInputs =======================================

    bool DigitalInputs::DI_Read(KMS::DAQ::Id aId) { return TestBits(1 << aId); }

}
