
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   SPI-Sim
// File      SPI-Sim-B/ChipList.h

#pragma once

// ===== Import/Includes ====================================================
#include <KMS/DAQ/DigitalInput.h>
#include <KMS/DAQ/DigitalOutput.h>
#include <KMS/Embedded/IInterruptHandler.h>
#include <KMS/Embedded/SPI.h>
#include <KMS/WOP/ValueArray.h>

class ChipList : public KMS::WOP::ValueArray<uint16_t, PROTOCOL_CHIP_QTY>, public KMS::Embedded::IInterruptHandler
{

public:

    ChipList();

    void Init(const uint8_t* aChipFromInt, KMS::Embedded::SPI* aSPI);

    void SetIOs(KMS::DAQ::DigitalInput* aIOs, const KMS::DAQ::DigitalOutput& aLED);

    // ===== Embedded::IInterruptHander =====================================
    virtual void OnInterrupt(uint8_t aIndex, uint8_t aLevel);

    // ===== KMS::WOP::BitArray<uint16_t> ===================================
    virtual uint8_t WriteData(const KMS::WOP::FrameBuffer* aIn);

private:

    KMS::WOP::Object* CreateChip(uint16_t aType, KMS::Embedded::SPI::ISlave** aSlave);

    const uint8_t             * mChipFromInt;
    uint8_t                     mConnected;
    uint8_t                     mCount;
    KMS::DAQ::DigitalInput    * mIOs;
    KMS::DAQ::DigitalOutput     mLED;
    KMS::Embedded::SPI::ISlave* mSlaves[PROTOCOL_CHIP_QTY];
    KMS::Embedded::SPI        * mSPI;

};
