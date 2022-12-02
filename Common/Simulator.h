
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   SPI-Sim
// File      Common/Simulator.h

#pragma once

// ===== Import/Includes ====================================================
#include <KMS/DAQ/DigitalInput.h>
#include <KMS/DAQ/DigitalOutput.h>
#include <KMS/Embedded/SPI.h>
#include <KMS/Embedded/WorkLoop.h>
#include <KMS/Msg/Destination.h>
#include <KMS/WOP/System.h>
#include <KMS/WOP/Link_USART.h>

// ===== Local ==============================================================
#include "Protocol.h"

class Simulator
{

public:

    class IOMap
    {

    public:
        KMS::DAQ::DigitalInput  mChipSelects[PROTOCOL_CHIP_QTY];
        KMS::DAQ::DigitalInput  mInputs     [PROTOCOL_INPUT_QTY];
        KMS::DAQ::DigitalOutput mLEDs       [4];
        KMS::DAQ::DigitalOutput mOutputs    [PROTOCOL_OUTPUT_QTY];

    };

    const KMS::Msg::Destination ON_INTERRUPT;

    Simulator(IOMap* aIOMap, KMS::Embedded::USART* aUSART, KMS::Embedded::SPI* aSPI);

    int Run();

private:

    IOMap                 * mIOMap;
    KMS::WOP::Link_USART    mLink;
    KMS::Embedded::WorkLoop mWorkLoop;

};
