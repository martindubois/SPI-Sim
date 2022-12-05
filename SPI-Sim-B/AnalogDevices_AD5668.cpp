
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   SPI-Sim
// File      SPI-Sim-B/AnalogDevice_AD5668.cpp

// Analog Devices
// Octal, 12-/140/16-Bit, SPI Voltage Output
// denseDAC with 5 ppm/C, On-Chip Reference
// Data Sheet AD5628/AD5648/AD5668

#include "Component.h"

// ===== Import/Includes ====================================================
#include <KMS/Embedded/SPI.h>

// ===== Local ==============================================================
#include "AnalogDevices_AD5668.h"

using namespace KMS;

// Constants
// //////////////////////////////////////////////////////////////////////////

// --> IDLE <--+
//      |      |
//      +--> WAIT_DATA
#define STATE_IDLE      (0)
#define STATE_WAIT_DATA (1)

namespace AnalogDevices
{

    // Public
    // //////////////////////////////////////////////////////////////////////

    AD5668::AD5668() : mState(STATE_IDLE) {}

    void* AD5668::operator new(size_t, void* aPtr) { return aPtr; }

    // ===== Embedded::SPI::ISlave ==========================================
    uint8_t AD5668::OnConnect(uint16_t*)
    {
        return FLAG_TX_WORD | FLAG_WORD_16;
    }

    void AD5668::OnDisconnect() {}

    uint8_t AD5668::OnRxWord(uint16_t* aWord)
    {
        // assert(NULL != aWord);

        uint16_t lWord = *aWord;

        switch (mState)
        {
        case STATE_IDLE:
            mAddr  = (lWord >>  4) & 0x000f;
            mValue = (lWord << 12) & 0xf000;
            mState = STATE_WAIT_DATA;
            break;

        case STATE_WAIT_DATA:
            mValue |= ((lWord >> 4) & 0x0fff);
            SetValue(mAddr, mValue);
            mState = STATE_IDLE;
            break;

        // default: assert(false);
        }

        return 0;
    }

    uint8_t AD5668::OnTxReady(uint16_t*)
    {
        return FLAG_TX_LAST_WORD | FLAG_TX_WORD | FLAG_WORD_16;
    }

}
