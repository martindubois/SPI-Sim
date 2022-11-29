
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   SPI-Sim
// File      SPI-Sim-B/AnalogDevice_AD5668.cpp

#include "Component.h"

// ===== Local ==============================================================
#include "AnalogDevices_AD5668.h"

using namespace KMS;

namespace AnalogDevices
{

    // Public
    // //////////////////////////////////////////////////////////////////////

    void* AD5668::operator new(size_t, void* aPtr) { return aPtr; }

    // ===== KMS::Msg::IReceiver ============================================
    unsigned int AD5668::Receive(void* aSender, unsigned int aCode, void* aData)
    {
        unsigned int lResult = Msg::IReceiver::MSG_IGNORED;

        // TODO

        return lResult;
    }

}
