
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   SPI-Sim
// File      SPI-Sim-A/Utilities_W.cpp

#include "Component.h"

// ===== Windows ============================================================
#include <Windows.h>

// ===== Local ==============================================================
#include "Utilities.h"

// Functions
// //////////////////////////////////////////////////////////////////////////

uint64_t Utl_GetNow_100ns()
{
    uint64_t lResult_100ns;

    GetSystemTimeAsFileTime(reinterpret_cast<FILETIME*>(&lResult_100ns));

    return lResult_100ns;
}
