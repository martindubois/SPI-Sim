
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   SPI-Sim
// File      Common/Version.h

#pragma once

// Constants
// //////////////////////////////////////////////////////////////////////////

// ===== Chip ===============================================================
#define CHIP_ANALOG_DEVICES_AD5162 (0x0001) // KMS::WOP::ValueArray<uint8_t, 2>
#define CHIP_ANALOG_DEVICES_AD5668 (0x0002) // KMS::WOP::ValueArray<uint16_t, 8>
#define CHIP_ANALOG_DEVICES_AD7689 (0x0003) // KMS::WOP::ValueArray<uint16_t, 8>

// ===== Instance index =====================================================
#define INSTANCE_DIGITAL_INPUTS (0) // KMS::WOP::BitArray<uint16>
#define INSTANCE_DIGITAL_OUTPUT (1) // KMS::WOP::BitArray<uint16>
#define INSTANCE_CHIP_TYPES     (2) // KMS::WOP::ValueArray<uint16_t, 10>
#define INSTANCE_CHIP_FIRST     (3)

#define INSTANCE_CHIP(I) (INSTANCE_CIP_FIRST + (I))
