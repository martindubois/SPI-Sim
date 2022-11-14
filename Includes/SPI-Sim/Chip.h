
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   SPI-Sim
// File      Includes/SPI-Sim/Simulator.h

#pragma once

// ===== Import/Includes ====================================================
#include <KMS/DI/Dictionary.h>

namespace SPI_Sim
{

    class Chip : public KMS::DI::Dictionary
    {

    public:

        static Chip* Create(uint16_t aType);

        virtual ~Chip();

        const char* GetManufacturer() const;

        const char* GetName() const;

        virtual void Dump();

        virtual void ExecuteCommand(const char* aCmd);

    // Internal

        uint16_t GetChipType() const;

    protected:

        Chip(uint16_t aType, const char* aManufacturer, const char* aName);

    private:

        const char* mManufacturer;
        const char* mName;

        uint16_t mType;

    };

}