
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

#define MSG_INTERRUPT (1)

#define UNCONNECTED (0xff)

// Variables
// //////////////////////////////////////////////////////////////////////////

static uint8_t sMemSlots[PROTOCOL_CHIP_QTY][32];

// Public
// //////////////////////////////////////////////////////////////////////////

ChipList::ChipList()
    : ON_INTERRUPT(this, MSG_INTERRUPT)
    , mConnected(UNCONNECTED), mCount(0), mIOs(NULL), mSPI(NULL)
{}

void ChipList::SetIOs(DAQ::DigitalInput* aIOs) { mIOs = aIOs; }

void ChipList::SetSPI(Embedded::SPI* aSPI) { mSPI = aSPI; }

// ===== Msg::IReceiver =====================================================

unsigned int ChipList::Receive(void* aSender, unsigned int aCode, void* aData)
{
    unsigned int lResult = Msg::IReceiver::MSG_IGNORED;

    switch (aCode)
    {
    case MSG_INTERRUPT: lResult = OnInterrupt(); break;
    }

    return lResult;
}

// ===== WOP::BitArray<uint16_t> ============================================

uint8_t ChipList::WriteData(const WOP::FrameBuffer* aIn)
{
    gSystem.AddTrace("Z", 1);
    uint8_t lResult = WOP::ValueArray<uint16_t, 10>::WriteData(aIn);
    if (KMS_WOP_RESULT_OK == lResult)
    {
        gSystem.AddTrace("Y", 1);
        if (0 == GetValue(0))
        {
            gSystem.AddTrace("X", 1);
            mCount = 0;
            gSystem.SetInstances(gInstances, INSTANCE_CHIP_FIRST);
        }
        else
        {
            gSystem.AddTrace("W", 1);
            uint16_t lType = GetValue(mCount);
            if (0 != lType)
            {
                WOP::Object* lInstance = NULL;

                switch (lType)
                {
                case CHIP_ANALOG_DEVICES_AD5162: lInstance = new(sMemSlots[mCount]) AnalogDevices::AD5162();
                case CHIP_ANALOG_DEVICES_AD5668: lInstance = new(sMemSlots[mCount]) AnalogDevices::AD5668();
                case CHIP_ANALOG_DEVICES_AD7689: lInstance = new(sMemSlots[mCount]) AnalogDevices::AD7689();
                }

                if (NULL == lInstance)
                {
                    gSystem.AddTrace("V", 1);
                    // TODO APPLICATION_ERROR
                    SetValue(mCount, 0);
                    lResult = KMS_WOP_RESULT_INVALID_DATA_TYPE;
                }
                else
                {
                    gSystem.AddTrace("U", 1);
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

unsigned int ChipList::OnInterrupt()
{
    gSystem.AddTrace("A", 1);

    if (mCount > mConnected)
    {
        gSystem.AddTrace("B", 1);
        if (!mIOs[mConnected].Read())
        {
            gSystem.AddTrace("C", 1);
            return 0;
        }

        mSPI->Slave_Disconnect();

        mConnected = UNCONNECTED;
    }

    for (unsigned int i = 0; i < mCount; i++)
    {
        if (!mIOs[i].Read())
        {
            gSystem.AddTrace("D", 1);
            mSPI->Slave_Connect(mReceivers[i], CHIP_MSG_RX, CHIP_MSG_TX);

            mConnected = i;
            break;
        }
    }

    return 0;
}
