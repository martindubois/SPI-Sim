
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   SPI-Sim
// File      SPI-Sim/SPI-Sim.cpp

// ===== Includes ===========================================================
#include <KMS/Banner.h>

// ===== Includes ===========================================================
#include <SPI-Sim/Simulator.h>

// ===== Local ==============================================================
#include "../Common/Version.h"

// Entry point
// //////////////////////////////////////////////////////////////////////////

int main(int aCount, const char** aVector)
{
    KMS_BANNER("SPI-Sim", "SPI-Sim");

    return SPI_Sim::Simulator::Main(aCount, aVector);
}
