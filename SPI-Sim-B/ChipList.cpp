
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   SPI-Sim
// File      SPI-Sim-B/ChipList.h

#include "Component.h"

// ===== Local ==============================================================
#include "AnalogDevices_AD5162.h"
#include "AnalogDevices_AD5668.h"
#include "AnalogDevices_AD7689.h"

#include "ChipList.h"

using namespace KMS;

// Constants
// //////////////////////////////////////////////////////////////////////////

#define UNCONNECTED (0xff)

// Variables
// //////////////////////////////////////////////////////////////////////////

static uint8_t sMemSlots[PROTOCOL_CHIP_QTY][32];

// Public
// //////////////////////////////////////////////////////////////////////////

ChipList::ChipList()
    : mChipFromInt(NULL), mConnected(UNCONNECTED), mCount(0), mIOs(NULL), mSPI(NULL)
{}

void ChipList::Init(const uint8_t* aChipFromInt, Embedded::SPI* aSPI)
{
    // assert(NULL != aChipFromInt);
    // assert(NULL != aSPI);

    // assert(NULL == mChipFromInt);
    // assert(NULL == mSPI);

    mChipFromInt = aChipFromInt;
    mSPI         = aSPI;
}

void ChipList::SetIOs(DAQ::DigitalInput* aIOs)
{
    // assert(NULL != aIOs);

    // assert(NULL == mIOs);

    mIOs = aIOs;
}

// ===== Embedded::IInterruptHandler ========================================

// ASSUMPTION  The master never
//             - Enable 2 slaves at same time
//             - Forget to disable slave

// Level: ISR
// CRITICAL PATH  SPI slave connect
void ChipList::OnInterrupt(uint8_t aIndex, uint8_t aLevel)
{
    uint8_t lChip = mChipFromInt[aIndex];
    if (mCount > lChip)
    {
        if (aLevel)
        {
            if (mConnected == lChip)
            {
                mSPI->Slave_Disconnect();

                mConnected = UNCONNECTED;
            }
        }
        else
        {
            mSPI->Slave_Connect(mSlaves[lChip]);

            mConnected = lChip;
        }
    }
}

// ===== WOP::BitArray<uint16_t> ============================================

uint8_t ChipList::WriteData(const WOP::FrameBuffer* aIn)
{
    uint8_t lResult = WOP::ValueArray<uint16_t, 10>::WriteData(aIn);
    if (KMS_WOP_RESULT_OK == lResult)
    {
        if (0 == GetValue(0))
        {
            mCount = 0;
            gSystem.SetInstances(gInstances, INSTANCE_CHIP_FIRST);
        }
        else
        {
            uint16_t lType;
            
            while ((PROTOCOL_CHIP_QTY > mCount) && (0 != (lType = GetValue(mCount))))
            {
                Embedded::SPI::ISlave* lSlave;

                WOP::Object* lInstance = CreateChip(lType, &lSlave);
                if (NULL == lInstance)
                {
                    // TODO APPLICATION_ERROR
                    SetValue(mCount, 0);
                    lResult = KMS_WOP_RESULT_INVALID_DATA_TYPE;
                }
                else
                {
                    // assert(NULL != lSlave);

                    mSlaves[mCount] = lSlave;

                    gInstances[INSTANCE_CHIP_FIRST + mCount] = lInstance;

                    mCount++;
                    gSystem.SetInstances(gInstances, INSTANCE_CHIP_FIRST + mCount);
                }
            }
        }
    }

    return lResult;
}

// Private
// //////////////////////////////////////////////////////////////////////////

WOP::Object* ChipList::CreateChip(uint16_t aType, Embedded::SPI::ISlave** aSlave)
{
    // assert(NULL != aSlave);

    switch (aType)
    {
    case CHIP_ANALOG_DEVICES_AD5162:
        gSystem.AddTrace("AD5162", 6);
        AnalogDevices::AD5162* lAD5162;
        lAD5162 = new(sMemSlots[mCount]) AnalogDevices::AD5162();
        *aSlave = lAD5162;
        return lAD5162;
    case CHIP_ANALOG_DEVICES_AD5668:
        gSystem.AddTrace("AD5668", 6);
        AnalogDevices::AD5668* lAD5668;
        lAD5668 = new(sMemSlots[mCount]) AnalogDevices::AD5668();
        *aSlave = lAD5668;
        return lAD5668;
    case CHIP_ANALOG_DEVICES_AD7689:
        gSystem.AddTrace("AD7689", 6);
        AnalogDevices::AD7689* lAD7689;
        lAD7689 = new(sMemSlots[mCount]) AnalogDevices::AD7689();
        *aSlave = lAD7689;
        return lAD7689;
    }

    return NULL;
}
