
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   SPI-Sim
// File      SPI-Sim-B/Simulator.cpp

#include "Component.h"

// ===== Local ==============================================================
#include "../Common/Version.h"

#include "../Common/Simulator.h"

#include "ChipList.h"
#include "Input.h"
#include "Output.h"

using namespace KMS;

// Variables
// //////////////////////////////////////////////////////////////////////////

WOP::Object* gInstances[INSTANCE_CHIP_FIRST + PROTOCOL_CHIP_QTY];
WOP::System  gSystem(VERSION, PROTOCOL_MAGIC, PROTOCOL_VERSION);

static ChipList sChips;
static Input    sInput;
static Output   sOutput;

// Public
// //////////////////////////////////////////////////////////////////////////

Simulator::Simulator(IOMap* aIOMap, Embedded::USART* aUSART, Embedded::SPI* aSPI)
    : ON_INTERRUPT(sChips.ON_INTERRUPT)
    , mIOMap(aIOMap)
    , mLink(&gSystem, aUSART)
{
    // assert(NULL != aIOMap);
    // assert(NULL != aUSART);
    // assert(NULL != aSPI);

    sChips.SetSPI(aSPI);

    sChips .SetIOs(aIOMap->mChipSelects);
    sInput .SetIOs(aIOMap->mInputs);
    sOutput.SetIOs(aIOMap->mOutputs);
}

int Simulator::Run()
{
    // Turn all LEDs off
    for (unsigned int i = 0; i < 4; i++)
    {
        mIOMap->mLEDs[i].Set(true);
    }

    gInstances[INSTANCE_DIGITAL_INPUTS ] = &sInput;
    gInstances[INSTANCE_DIGITAL_OUTPUTS] = &sOutput;
    gInstances[INSTANCE_CHIP_TYPES     ] = &sChips;

    gSystem.SetInstances(gInstances, 3);

    Embedded::USART* lUSART = mLink.GetUSART();
    // assert(NULL != lUSART);

    lUSART->Rx_Enable();

    mWorkLoop.AddItem(&sInput);
    mWorkLoop.AddItem(&mLink);
    mWorkLoop.AddItem(lUSART);

    // LED 0 = Running
    mIOMap->mLEDs[0].Set(false);

    mWorkLoop.Run();

    return 0;
}
