
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   SPI-Sim
// File      SPI-Sim-B/ChipList.h

#include "Component.h"

// ===== Local ==============================================================
#include "../Common/Protocol.h"

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
    , mConnected(UNCONNECTED), mCount(0), mInstances(NULL), mIOs(NULL), mSPI(NULL), mSystem(NULL)
{}

void ChipList::SetIOs(DAQ::DigitalInput* aIOs) { mIOs = aIOs; }

void ChipList::SetSPI(Embedded::SPI* aSPI) { mSPI = aSPI; }

void ChipList::SetSystem(WOP::System* aSystem, WOP::Object** aInstances)
{
    mInstances = aInstances;
    mSystem    = aSystem;
}

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

// ===== KMS::WOP::BitArray<uint16_t> =======================================

uint8_t ChipList::WriteData(const WOP::FrameBuffer* aIn)
{
    uint8_t lResult = WOP::ValueArray<uint16_t, 10>::WriteData(aIn);
    if (KMS_WOP_RESULT_OK == lResult)
    {
        if (0 == GetValue(0))
        {
            mCount = 0;
            mSystem->SetInstances(mInstances, INSTANCE_CHIP_FIRST);
        }
        else
        {
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
                    // TODO APPLICATION_ERROR
                    SetValue(mCount, 0);
                    lResult = KMS_WOP_RESULT_INVALID_DATA_TYPE;
                }
                else
                {
                    mInstances[INSTANCE_CHIP_FIRST + mCount] = lInstance;

                    mCount++;
                    mSystem->SetInstances(mInstances, INSTANCE_CHIP_FIRST + mCount);
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
    if (mCount > mConnected)
    {
        if (mIOs[mConnected].Read())
        {
            return 0;
        }

        mSPI->Slave_Disconnect();

        mConnected = UNCONNECTED;
    }

    for (unsigned int i = 0; i < mCount; i++)
    {
        if (mIOs[i].Read())
        {
            mSPI->Slave_Connect(mReceivers[i], CHIP_MSG_RX, CHIP_MSG_TX);

            mConnected = i;
            break;
        }
    }

    return 0;
}
