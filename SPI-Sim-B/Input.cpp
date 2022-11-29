
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   SPI-Sim
// File      SPI-Sim-B/Input.cpp

#include "Component.h"

// ===== Local ==============================================================
#include "Input.h"

using namespace KMS;

// Public
// //////////////////////////////////////////////////////////////////////////

Input::Input() : mIOs(NULL) {}

void Input::SetIOs(DAQ::DigitalInput* aIOs) { mIOs = aIOs; }

// ===== KMS::Embedded::WorkItem ============================================

void Input::Work()
{
    uint16_t lValue = 0;

    for (unsigned int i = 0; i < 13; i++)
    {
        if (mIOs[i].Read())
        {
            lValue |= (1 << i);
        }
    }

    if (GetBits(0xffff) != lValue)
    {
        SetData(lValue);
    }
}
