
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   SPI-Sim
// File      SPI-Sim-B/AnalogDevice_AD5162.cpp

// Analog Devices
// Dual, 256-Position, SPI
// Digital Potentiometer
// AD5162

#include "Component.h"

// ===== Local ==============================================================
#include "AnalogDevices_AD5162.h"

using namespace KMS;

namespace AnalogDevices
{

    // Public
    // //////////////////////////////////////////////////////////////////////

    void* AD5162::operator new(size_t, void* aPtr) { return aPtr; }

    // ===== Embedded::SPI::ISlave ==========================================

    uint8_t AD5162::OnConnect(uint16_t*)
    {
        return FLAG_TX_LAST_WORD | FLAG_TX_WORD | FLAG_WORD_09;
    }

    void AD5162::OnDisconnect() {}

    uint8_t AD5162::OnRxWord(uint16_t* aWord)
    {
        // assert(NULL != aWord);

        uint16_t lWord = *aWord;

        uint8_t lIndex = (lWord >> 8) & 0x01;
        uint8_t lValue =  lWord       & 0xff;

        SetValue(lIndex, lValue);

        return 0;
    }

    uint8_t AD5162::OnTxReady(uint16_t*) { return 0; }

}
