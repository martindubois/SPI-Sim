
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   SPI-Sim
// File      SPI-Sim-B/ChipList.h

#pragma once

// ===== Import/Includes ====================================================
#include <KMS/DAQ/DigitalInput.h>
#include <KMS/Embedded/SPI.h>
#include <KMS/Msg/Destination.h>
#include <KMS/Msg/IReceiver.h>
#include <KMS/WOP/ValueArray.h>

class ChipList : public KMS::WOP::ValueArray<uint16_t, PROTOCOL_CHIP_QTY>, public KMS::Msg::IReceiver
{

public:

    const KMS::Msg::Destination ON_INTERRUPT;

    ChipList();

    void SetIOs(KMS::DAQ::DigitalInput* aIOs);

    void SetSPI(KMS::Embedded::SPI* aSPI);

    // ===== Msg::IReceiver =================================================
    unsigned int Receive(void* aSender, unsigned int aCode, void* aData);

    // ===== KMS::WOP::BitArray<uint16_t> ===================================
    virtual uint8_t WriteData(const KMS::WOP::FrameBuffer* aIn);

private:

    unsigned int OnInterrupt();

    uint8_t                 mConnected;
    uint8_t                 mCount;
    KMS::DAQ::DigitalInput* mIOs;
    KMS::Msg::IReceiver   * mReceivers[PROTOCOL_CHIP_QTY];
    KMS::Embedded::SPI    * mSPI;

};
