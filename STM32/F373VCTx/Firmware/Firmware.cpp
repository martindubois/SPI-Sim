
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   SPI-Sim
// File      STM32/F373CVTx/Firmware/Firmware.cpp

// ===== C ==================================================================
#include <stdlib.h>

// ===== Includes ===========================================================
#include <KMS/STM/STM32F.h>

// ===== Local ==============================================================
#include "../../../Common/Simulator.h"

using namespace KMS;

// Constants
// //////////////////////////////////////////////////////////////////////////

static const uint8_t CHIP_SELECTS[10] = { 0, 1, 2, 4, 6, 7, 8, 9, 10, 14 };

static const uint8_t CHIP_FROM_INT[16] = { 0, 1, 2, 0xff, 3, 0xff, 4, 5, 6, 7, 8, 0xff, 0xff, 0xff, 9, 0xff };

// Static variables
// //////////////////////////////////////////////////////////////////////////

static Simulator::IOMap sIOMap;
static STM::STM32F      sProcessor;

// Entry point
// //////////////////////////////////////////////////////////////////////////

int main()
{
    sProcessor.Clock_64_MHz();

    // The selected IOs fit the STM32373C-EVAL board.

    DAQ::Id lId;
    unsigned int i;

    // LEDs
    for (i = 0; i < 4; i++)
    {
        lId = KMS_STM_ID_PC(i);

        sProcessor.IO_SetMode(lId, STM::STM32F::IO_Mode::DIGITAL_OUTPUT_OPEN_DRAIN);
        sIOMap.mLEDs[i] = DAQ::DigitalOutput(static_cast<DAQ::IDigitalOutputs*>(&sProcessor), lId);
    }

    // Input
    for (i = 0; i < PROTOCOL_INPUT_QTY; i++)
    {
        lId = KMS_STM_ID_PA(i);

        sProcessor.IO_SetMode(lId, STM::STM32F::IO_Mode::DIGITAL_INPUT);
        sIOMap.mInputs[i] = DAQ::DigitalInput(static_cast<DAQ::IDigitalInputs*>(&sProcessor), lId);
    }

    // Outputs
    for (i = 0; i < PROTOCOL_OUTPUT_QTY; i++)
    {
        lId = KMS_STM_ID_PE(i);

        sProcessor.IO_SetMode(lId, STM::STM32F::IO_Mode::DIGITAL_OUTPUT_OPEN_DRAIN);
        sIOMap.mOutputs[i] = DAQ::DigitalOutput(static_cast<DAQ::IDigitalOutputs*>(&sProcessor), lId);
    }

    // Chip selects
    for (i = 0; i < PROTOCOL_CHIP_QTY; i++)
    {
        lId = KMS_STM_ID_PB(CHIP_SELECTS[i]);

        sProcessor.IO_SetMode(lId, STM::STM32F::IO_Mode::DIGITAL_INPUT);
        sIOMap.mChipSelects[i] = DAQ::DigitalInput(static_cast<DAQ::IDigitalInputs*>(&sProcessor), lId);
    }

    Embedded::USART* lUSART = sProcessor.USART_Get(1, KMS_STM_ID_PD(6), KMS_STM_ID_PD(5));
    Embedded::SPI  * lSPI   = sProcessor.SPI_Get(2, KMS_STM_ID_PC(11), KMS_STM_ID_PC(12), KMS_STM_ID_PC(10));

    static Simulator sSimulator(&sIOMap, CHIP_FROM_INT, lUSART, lSPI);

    // Interrupts
    for (i = 0; i < PROTOCOL_CHIP_QTY; i++)
    {
        sProcessor.IO_ConfigureInterrupt(KMS_STM_ID_PB(CHIP_SELECTS[i]), sSimulator.GetInterruptHandler());
    }

    return sSimulator.Run();
}
