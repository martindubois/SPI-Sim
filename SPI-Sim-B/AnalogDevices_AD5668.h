
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   SPI-Sim
// File      SPI-Sim-B/AnalogDevice_AD5668.h

#pragma once

// ===== Import/Includes ====================================================
#include <KMS/Msg/IReceiver.h>
#include <KMS/WOP/ValueArray.h>

namespace AnalogDevices
{

    class AD5668 : public KMS::WOP::ValueArray<uint16_t, 8>, public KMS::Msg::IReceiver
    {

    public:

        void* operator new(size_t aSize_byte, void* aPtr);

        // ===== KMS::Msg::IReceiver ========================================
        unsigned int Receive(void* aSender, unsigned int aCode, void* aData);

    };

}
