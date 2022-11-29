
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   SPI-Sim
// File      SPI-Sim-B/Input.h

#pragma once

// ===== Import/Includes ====================================================
#include <KMS/DAQ/DigitalInput.h>
#include <KMS/Embedded/WorkItem.h>
#include <KMS/WOP/BitArray.h>

class Input : public KMS::WOP::BitArray<uint16_t>, public KMS::Embedded::WorkItem
{

public:

    Input();

    void SetIOs(KMS::DAQ::DigitalInput* aIOs);

    // ===== KMS::Embedded::WorkItem ========================================
    virtual void Work();

private:

    KMS::DAQ::DigitalInput* mIOs;

};
