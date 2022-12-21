
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   SPI-Sim
// File      SPI-Sim-A/Simulator.cpp

// TEST COVERAGE 2022-12-05 KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Import/Includes ====================================================
#include <KMS/Cfg/Configurator.h>
#include <KMS/Cfg/MetaData.h>
#include <KMS/Console/Color.h>
#include <KMS/Dbg/Log.h>
#include <KMS/DI/UInt.h>
#include <KMS/Exception.h>
#include <KMS/Installer.h>

// ===== Includes ===========================================================
#include <SPI-Sim/Simulator.h>

// ===== Local ==============================================================
#include "../Common/Protocol.h"
#include "../Common/Version.h"

#include "Utilities.h"

using namespace KMS;

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define CONFIG_FILE "SPI-Sim.cfg"

// Constants
// //////////////////////////////////////////////////////////////////////////

static const Cfg::MetaData MD_CHIPS("Chips += {Type}");

#define MSG_CHIPS_CHANGED (1)
#define MSG_ITERATE       (2)

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static void Display_Error(const char* aMsg);
static void Display_OK   (const char* aMsg);

namespace SPI_Sim
{

    // Public
    // //////////////////////////////////////////////////////////////////////

    const unsigned int Simulator::TICK_PERIOD_ms = 50;

    int Simulator::Main(int aCount, const char** aVector)
    {
        Simulator lSimulator(VERSION);

        return Main(aCount, aVector, &lSimulator);
    }

    int Simulator::Main(int aCount, const char** aVector, Simulator* aSimulator)
    {
        assert(1 <= aCount);
        assert(NULL != aVector);

        int lResult = __LINE__;

        try
        {
            Cfg::Configurator lC;
            Installer         lInstaller;

            lC.AddConfigurable(&lInstaller);
            lC.AddConfigurable(aSimulator);

            lC.AddConfigurable(&Dbg::gLog);

            lC.ParseFile(File::Folder::CURRENT, CONFIG_FILE);
            lC.ParseArguments(aCount - 1, aVector + 1);

            lInstaller.Run();

            lResult = aSimulator->Run();
        }
        KMS_CATCH_RESULT(lResult);

        return lResult;
    }

    Simulator::Simulator(const Version& aVersion)
        : mChipCount(0)
        , mLink(&mSystem, &mPort)
        , mSystem(aVersion, PROTOCOL_MAGIC, PROTOCOL_VERSION)
    {
        mInstances[0] = &mDigitalInputs;
        mInstances[1] = &mDigitalOutputs;
        mInstances[2] = &mChipTypes;

        mSystem.SetInstances(mInstances, INSTANCE_CHIP_FIRST);

        mChips.mOnChanged.Set(this, MSG_CHIPS_CHANGED);
        mChips.SetCreator(DI::UInt<uint16_t>::Create);

        mTick_Thread.mOnIterate.Set(this, MSG_ITERATE);

        AddEntry("Chips", &mChips, false, &MD_CHIPS);
        AddEntry("Port" , &mPort , false);
    }

    // No need to delete the chips, the mChips (DI::Array) destructor will
    // delete them.
    Simulator::~Simulator() {}

    void Simulator::AddChip(Chip* aChip)
    {
        assert(NULL != aChip);

        KMS_EXCEPTION_ASSERT(CHIP_MAX > mChipCount, APPLICATION_USER_ERROR, "Too many chips", "");

        mChipTypes.SetValue(mChipCount, aChip->GetChipType());

        mChips.SetEntry(mChipCount, aChip, true);

        unsigned int lInstanceCount = INSTANCE_CHIP_FIRST + mChipCount;

        mInstances[lInstanceCount] = aChip->GetInstance();

        mChipCount++;

        lInstanceCount++;

        mSystem.SetInstances(mInstances, lInstanceCount);
    }

    // ===== Commands =======================================================

    void Simulator::Connect()
    {
        mPort.Connect(Dev::Device::FLAG_READ_ACCESS | Dev::Device::FLAG_WRITE_ACCESS);

        mLink.Start();

        Display_OK("Connected");
    }

    void Simulator::Disconnect()
    {
        mLink.Stop();

        mPort.Disconnect();

        Display_OK("Disconnected");
    }

    void Simulator::Tick_Start()
    {
        uint64_t lNow_100ns = Utl_GetNow_100ns();

        mTick_Next_100ns = lNow_100ns;

        mTick_Thread.Start();

        Display_OK("Started");
    }

    void Simulator::Tick_Stop()
    {
        mTick_Thread.StopAndWait(2000); // 2 s

        Display_OK("Stopped");
    }

    // ===== Events =========================================================

    void Simulator::OnLoad() {}
    void Simulator::OnTick() {}

    // ===== Msg::Receive ===================================================

    unsigned int Simulator::Receive(void* aSender, unsigned int aCode, void* aData)
    {
        unsigned int lResult = MSG_IGNORED;

        switch (aCode)
        {
        case MSG_CHIPS_CHANGED: lResult = OnChipsChanged(); break;
        case MSG_ITERATE      : lResult = OnIterate     (); break;
        }

        return lResult;
    }

    // ===== CLI::Tool ======================================================

    void Simulator::DisplayHelp(FILE* aOut) const
    {
        assert(NULL != aOut);

        fprintf_s(aOut,
            "Chip {Index} {Command}\n"
            "Chips\n"
            "Connect\n"
            "Disconnect\n"
            "DI_Read {Index}\n"
            "DIs\n"
            "DO_Clear {Index}\n"
            "DO_Get {Index}\n"
            "DO_Set {Index}\n"
            "DOs\n"
            "Dump\n"
            "Tick Start\n"
            "Tick Stop\n");

        return CLI::Tool::DisplayHelp(aOut);
    }

    void Simulator::ExecuteCommand(const char* aC)
    {
        assert(NULL != aC);

        char         lCmd[LINE_LENGTH];
        unsigned int lIndex;

        if (0 == strcmp(aC, "Connect"   )) { Connect   (); return; }
        if (0 == strcmp(aC, "Chips"     )) { Chips     (); return; }
        if (0 == strcmp(aC, "Disconnect")) { Disconnect(); return; }
        if (0 == strcmp(aC, "DIs"       )) { DIs       (); return; }
        if (0 == strcmp(aC, "DOs"       )) { DOs       (); return; }
        if (0 == strcmp(aC, "Dump"      )) { Dump      (); return; }
        if (0 == strcmp(aC, "Tick Start")) { Tick_Start(); return; }
        if (0 == strcmp(aC, "Tick Stop" )) { Tick_Stop (); return; }

        if (2 == sscanf_s(aC, "Chip %u %[^\n\r\t]", &lIndex, lCmd SizeInfo(lCmd))) { ChipCommand(lIndex, lCmd); return; }

        if (1 == sscanf_s(aC, "DI_Read %u\n" , &lIndex)) { DI_Read (lIndex); return; }
        if (1 == sscanf_s(aC, "DO_Clear %u\n", &lIndex)) { DO_Clear(lIndex); return; }
        if (1 == sscanf_s(aC, "DO_Get %u\n"  , &lIndex)) { DO_Get  (lIndex); return; }
        if (1 == sscanf_s(aC, "DO_Set %u\n"  , &lIndex)) { DO_Set  (lIndex); return; }

        CLI::Tool::ExecuteCommand(aC);
    }

    int Simulator::Run()
    {
        OnLoad();

        return CLI::Tool::Run();
    }

    // Private
    // //////////////////////////////////////////////////////////////////////

    Chip* Simulator::GetChip(unsigned int aIndex)
    {
        KMS_EXCEPTION_ASSERT(mChipCount > aIndex, APPLICATION_USER_ERROR, "Invalid index", aIndex);

        DI::Object* lObject = mChips.GetEntry_RW(aIndex);
        assert(NULL != lObject);

        Chip* lResult = dynamic_cast<Chip*>(lObject);
        assert(NULL != lObject);

        return lResult;
    }

    // ===== Events =========================================================

    unsigned int Simulator::OnChipsChanged()
    {
        if (mChips.GetCount() > mChipCount)
        {
            KMS_EXCEPTION_ASSERT(CHIP_MAX > mChipCount, APPLICATION_USER_ERROR, "Too many chips", mChipCount);

            const DI::Object* lObject = mChips.GetEntry_R(mChipCount);
            assert(NULL != lObject);

            const DI::UInt<uint16_t>* lType = dynamic_cast<const DI::UInt<uint16_t>*>(lObject);
            assert(NULL != lType);

            Chip* lChip = Chip::Create(lType->Get());
            if (NULL == lChip)
            {
                mChips.RemoveEntry(mChipCount);
                KMS_EXCEPTION(APPLICATION_USER_ERROR, "Invalid chip type", lType->Get());
            }

            AddChip(lChip);
        }
        // NOT TESTED
        else if (mChips.GetCount() == 0)
        {
            mChipCount = 0;
            mChipTypes.Clear();
        }

        return 0;
    }

    unsigned int Simulator::OnIterate()
    {
        uint64_t lNow_100ns = Utl_GetNow_100ns();

        mTick_Next_100ns += (TICK_PERIOD_ms * 10000);

        if (mTick_Next_100ns > lNow_100ns)
        {
            uint64_t lSleep_100ns = mTick_Next_100ns - lNow_100ns;

            Sleep(static_cast<DWORD>(lSleep_100ns / 10000));
        }

        OnTick();

        return 0;
    }

    // ===== Commands =======================================================

    void Simulator::ChipCommand(unsigned int aId, const char* aCmd)
    {
        GetChip(aId)->ExecuteCommand(aCmd);
    }

    void Simulator::Chips()
    {
        for (DI::Container::Entry& lEntry : mChips.mInternal)
        {
            Chip* lChip = dynamic_cast<Chip*>(lEntry.Get());
            assert(NULL != lChip);

            lChip->Dump();
        }
    }

    void Simulator::DI_Read(unsigned int aId)
    {
        if (mDigitalInputs.GetCount() <= aId)
        {
            Display_Error("USER ERROR  Invalid id");
            return;
        }

        std::cout << "\nDI #" << aId << " = " << (mDigitalInputs.DI_Read(aId) ? "true" : "false") << "\n" << std::endl;
    }

    void Simulator::DIs() { mDigitalInputs .Dump(); }
    void Simulator::DOs() { mDigitalOutputs.Dump(); }

    void Simulator::DO_Clear(unsigned int aId)
    {
        if (mDigitalOutputs.GetCount() <= aId)
        {
            Display_Error("USER ERROR  Invalid id");
            return;
        }

        mDigitalOutputs.DO_Clear(aId);

        Display_OK("Cleared");
    }

    void Simulator::DO_Get(unsigned int aId)
    {
        if (mDigitalOutputs.GetCount() <= aId)
        {
            Display_Error("USER ERROR  Invalid id");
            return;
        }

        std::cout << "\nDO #" << aId << " = " << (mDigitalOutputs.DO_Get(aId) ? "true" : "false") << "\n" << std::endl;
    }

    void Simulator::DO_Set(unsigned int aId)
    {
        if (mDigitalOutputs.GetCount() <= aId)
        {
            Display_Error("USER ERROR  Invalid id");
            return;
        }

        mDigitalOutputs.DO_Set(aId);

        Display_OK("Set");
    }

    void Simulator::Dump()
    {
        DIs();
        DOs();

        Chips();
    }

}

// Static functions
// //////////////////////////////////////////////////////////////////////////

void Display_Error(const char* aMsg)
{
    assert(NULL != aMsg);

    std::cout << Console::Color::RED;
    std::cout << "\n" << aMsg << "\n" << Console::Color::WHITE << std::endl;
}

void Display_OK(const char* aMsg)
{
    assert(NULL != aMsg);

    std::cout << "\n" << aMsg << "\n" << std::endl;
}
