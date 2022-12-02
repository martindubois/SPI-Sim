
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   SPI-Sim
// File      SPI-Sim-B/Simulator.cpp

#pragma once

// ===== C ==================================================================
#include <memory.h>
#include <stdint.h>
#include <stdlib.h>

// ===== Import/Includes ====================================================
#include <KMS/WOP/System.h>

// ===== Local ==============================================================
#include "../Common/Protocol.h"

// Constants
// //////////////////////////////////////////////////////////////////////////

#define CHIP_MSG_RX (1)
#define CHIP_MSG_TX (2)

// Global variables
// //////////////////////////////////////////////////////////////////////////

// Defined in Simulator.cpp
extern KMS::WOP::Object * gInstances[INSTANCE_CHIP_FIRST + PROTOCOL_CHIP_QTY];
extern KMS::WOP::System   gSystem;
