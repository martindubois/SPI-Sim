
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

    // ===== Embedded::SPI::ISlave ==========================================
    uint8_t AD7689::OnConnect(uint16_t* aWord)
    {
        // assert(NULL != aWord);

        // TODO
        return 0;
    }

    void AD7689::OnDisconnect()
    {
        // TODO
    }

    uint8_t AD7689::OnRxWord(uint16_t* aWord)
    {
        // assert(NULL != aWord);

        // TODO
        return 0;
    }

    uint8_t AD7689::OnTxReady(uint16_t* aWord)
    {
        // assert(NULL != aWord);

        // TODO
        return 0;
    }

}
