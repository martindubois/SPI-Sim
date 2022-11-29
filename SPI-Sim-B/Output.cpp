
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   SPI-Sim
// File      SPI-Sim-B/Output.cpp

#include "Component.h"

// ===== Local ==============================================================
#include "Output.h"

using namespace KMS;

// Public
// //////////////////////////////////////////////////////////////////////////

Output::Output() : mIOs(NULL) {}

void Output::SetIOs(DAQ::DigitalOutput* aIOs) { mIOs = aIOs; }

// ===== WOP::BitArray<uint16_t> ============================================

uint8_t Output::WriteData(const WOP::FrameBuffer* aIn)
{
    uint8_t lResult = WOP::BitArray<uint16_t>::WriteData(aIn);
    if (KMS_WOP_RESULT_OK == lResult)
    {
        uint16_t lValue = GetBits(0xffff);

        for (unsigned int i = 0; i < 16; i++)
        {
            if (0 != (lValue & 0x0001))
            {
                mIOs[i].Set();
            }
            else
            {
                mIOs[i].Clear();
            }

            lValue >>= 1;
        }
    }

    return lResult;
}
