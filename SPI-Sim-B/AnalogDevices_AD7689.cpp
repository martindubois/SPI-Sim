
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   SPI-Sim
// File      SPI-Sim-B/AnalogDevice_AD7689.cpp

// Analog Devices
// 16-Bit, 4-Channel/8Channel,
// 250 kSPS PulSAR ADCs
// AD7682/AD7689

#include "Component.h"

// ===== Local ==============================================================
#include "AnalogDevices_AD7689.h"

using namespace KMS;

// Constants
// //////////////////////////////////////////////////////////////////////////

typedef union
{
    struct
    {
        unsigned mReserved0 : 2;

        unsigned mRB   : 1; // Read Back configuration
        unsigned mSEQ  : 2; // SEQuencer mode
        unsigned mREF  : 3; // REFerence selection
        unsigned mBW   : 1; // BandWidth selection
        unsigned mINx  : 3; // INput channel selection
        unsigned mINCC : 3; // INput Channel Configuration
        unsigned mCFG  : 1; // ConFiGuration update
    }
    mFields;

    uint16_t mValue;
}
ConfigReg;

#define CFG              (0x8000)
#define INX_MASK         (0x0e00)
#define INX_SHIFT        (9)
#define SEQ_MASK         (0x0018)
#define SEQ_SHIFT        (3)

#define SEQ_MODE_DISABLE  (0x0)
#define SEQ_MODE_ALL_TEMP (0x2)
#define SEQ_MODE_ALL      (0x3)

#define INDEX_TEMP (8)

namespace AnalogDevices
{

    // Public
    // //////////////////////////////////////////////////////////////////////

    AD7689::AD7689() : mIndex_Acq(0), mIndex_Max(7), mIndex_Seq(0), mIndex_Tx(0), mSeqMode(SEQ_MODE_ALL)
    {
    }

    void* AD7689::operator new(size_t, void* aPtr) { return aPtr; }

    // ===== Embedded::SPI::ISlave ==========================================
    uint8_t AD7689::OnConnect(uint16_t* aWord)
    {
        // assert(NULL != aWord);

        *aWord = GetValue(mIndex_Tx);

        return FLAG_TX_LAST_WORD | FLAG_TX_WORD;
    }

    void AD7689::OnDisconnect() {}

    uint8_t AD7689::OnRxWord(uint16_t* aWord)
    {
        // assert(NULL != aWord);

        mIndex_Tx  = mIndex_Acq;
        mIndex_Acq = mIndex_Seq;

        ConfigReg lCR;

        lCR.mValue = *aWord;

        if (lCR.mFields.mCFG)
        {
            switch (lCR.mFields.mSEQ)
            {
            case SEQ_MODE_DISABLE :
                mIndex_Seq = lCR.mFields.mINx;
                mSeqMode   = lCR.mFields.mSEQ;
                break;
            case SEQ_MODE_ALL     :
            case SEQ_MODE_ALL_TEMP:
                mIndex_Max = lCR.mFields.mINx;
                mIndex_Seq = 0;
                mSeqMode   = lCR.mFields.mSEQ;
                break;
            }
        }
        else
        {
            if (INDEX_TEMP == mIndex_Seq)
            {
                mIndex_Seq = 0;
            }
            else
            {
                switch (mSeqMode)
                {
                case SEQ_MODE_DISABLE: break;
                case SEQ_MODE_ALL:
                    mIndex_Seq++;
                    if (mIndex_Max < mIndex_Seq) { mIndex_Seq = 0; }
                    break;
                case SEQ_MODE_ALL_TEMP:
                    mIndex_Seq++;
                    if (mIndex_Max < mIndex_Seq) { mIndex_Seq = INDEX_TEMP; }
                    break;

                // default: assert(false);
                }
            }
        }

        return 0;
    }

    uint8_t AD7689::OnTxReady(uint16_t*) { return 0; }

}
