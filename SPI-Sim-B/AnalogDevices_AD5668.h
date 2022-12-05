
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   SPI-Sim
// File      SPI-Sim-B/AnalogDevice_AD5668.h

#pragma once

// ===== Import/Includes ====================================================
#include <KMS/Embedded/SPI.h>
#include <KMS/WOP/ValueArray.h>

namespace AnalogDevices
{

    class AD5668 : public KMS::WOP::ValueArray<uint16_t, 8>, public KMS::Embedded::SPI::ISlave
    {

    public:

        AD5668();

        void* operator new(size_t aSize_byte, void* aPtr);

        // ===== KMS::Embedded::SPI::ISlave =================================
        virtual uint8_t OnConnect   (uint16_t* aWord);
        virtual void    OnDisconnect();
        virtual uint8_t OnRxWord    (uint16_t* aWord);
        virtual uint8_t OnTxReady   (uint16_t* aWord);

    private:

        uint8_t  mAddr ;
        uint8_t  mState;
        uint16_t mValue;

    };

}
