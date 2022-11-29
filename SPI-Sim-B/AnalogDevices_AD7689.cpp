
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   SPI-Sim
// File      SPI-Sim-B/AnalogDevice_AD7689.cpp

#include "Component.h"

// ===== Local ==============================================================
#include "AnalogDevices_AD7689.h"

using namespace KMS;

namespace AnalogDevices
{

    // Public
    // //////////////////////////////////////////////////////////////////////

    void* AD7689::operator new(size_t, void* aPtr) { return aPtr; }

    // ===== Msg::IReceiver =================================================

    unsigned int AD7689::Receive(void* aSender, unsigned int aCode, void* aData)
    {
        unsigned int lResult = Msg::IReceiver::MSG_IGNORED;

        // TODO

        return lResult;
    }

}
