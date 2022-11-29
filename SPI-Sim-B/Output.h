
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   SPI-Sim
// File      SPI-Sim-B/Output.h

#pragma once

// ===== Import/Includes ====================================================
#include <KMS/DAQ/DigitalOutput.h>
#include <KMS/WOP/BitArray.h>

class Output : public KMS::WOP::BitArray<uint16_t>
{

public:

    Output();

    void SetIOs(KMS::DAQ::DigitalOutput* aIOs);

    // ===== KMS::WOP::BitArray<uint16_t> ===================================
    virtual uint8_t WriteData(const KMS::WOP::FrameBuffer* aIn);

private:

    KMS::DAQ::DigitalOutput* mIOs;

};
