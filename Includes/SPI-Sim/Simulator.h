
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   SPI-Sim
// File      Includes/SPI-Sim/Simulator.h

#pragma once

// ===== Import/Includes ====================================================
#include <KMS/CLI/Tool.h>
#include <KMS/Com/Port.h>
#include <KMS/Msg/IReceiver.h>
#include <KMS/Thread/Thread.h>
#include <KMS/WOP/Link_Port.h>
#include <KMS/WOP/System.h>
#include <KMS/WOP/ValueArray.h>

// ===== Includes ===========================================================
#include <SPI-Sim/Chip.h>
#include <SPI-Sim/DigitalInputs.h>
#include <SPI-Sim/DigitalOutputs.h>

namespace SPI_Sim
{

    class Simulator : public KMS::CLI::Tool, public KMS::Msg::IReceiver
    {

    public:

        static const unsigned int TICK_PERIOD_ms;

        static int Main(int aCount, const char** aVector);

        static int Main(int aCount, const char** aVector, Simulator* aSimulator);

        Simulator(const KMS::Version& aVersion);

        ~Simulator();

        // aIn  The Simulator destructor will delete this chips
        void AddChip(Chip* aIn);

        // ===== Commands ===================================================

        virtual void Connect   ();
        virtual void Disconnect();

        virtual void Tick_Start();
        virtual void Tick_Stop ();

        // ===== Events =====================================================

        virtual void OnLoad();
        virtual void OnTick();

        DigitalInputs  mDigitalInputs;
        DigitalOutputs mDigitalOutputs;

        KMS::Com::Port mPort;

        // ===== KMS::Msg::IReceiver ========================================
        virtual unsigned int Receive(void* aSender, unsigned int aCode, void* aData);

        // ===== KMS::CLI::Tool =============================================
        virtual void DisplayHelp(FILE* aOut) const;
        virtual void ExecuteCommand(const char* aC);
        virtual int  Run();

    private:

        enum
        {
            CHIP_MAX = 10,
        };

        Chip* GetChip(unsigned int aIndex);

        // ===== Events =====================================================
        unsigned int OnChipsChanged();
        unsigned int OnIterate();

        // ===== Command ====================================================
        void ChipCommand(unsigned int aIndex, const char* aCmd);
        void Dump();
        void DI_Read(unsigned int aId);
        void DO_Clear(unsigned int aId);
        void DO_Get(unsigned int aId);
        void DO_Set(unsigned int aId);

        KMS::DI::Array                           mChips;
        unsigned int                             mChipCount;
        KMS::WOP::ValueArray<uint16_t, CHIP_MAX> mChipTypes;

        KMS::WOP::Object* mInstances[3 + CHIP_MAX];

        KMS::WOP::Link_Port mLink;
        KMS::WOP::System    mSystem;

        uint64_t            mTick_Next_100ns;
        KMS::Thread::Thread mTick_Thread;

    };

}
