
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   SPI-Sim
// File      SPI-Sim-A/Simulator.cpp

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

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define CONFIG_FILE "SPI-Sim.cfg"

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_CHIPS("Chips += {Type}");

#define MSG_CHIPS_CHANGED (1)
#define MSG_RECEIVE       (2)
#define MSG_SEND          (3)

namespace SPI_Sim
{

    // Public
    // //////////////////////////////////////////////////////////////////////

    int Simulator::Main(int aCount, const char** aVector)
    {
        assert(1 <= aCount);
        assert(NULL != aVector);

        int lResult = __LINE__;

        try
        {
            KMS::Cfg::Configurator lC;
            KMS::Installer         lInstaller;
            Simulator              lS;

            lC.AddConfigurable(&lInstaller);
            lC.AddConfigurable(&lS);

            lC.AddConfigurable(&KMS::Dbg::gLog);

            lC.ParseFile(KMS::File::Folder::CURRENT, CONFIG_FILE);
            lC.ParseArguments(aCount - 1, aVector + 1);

            lInstaller.Run();

            lResult = lS.Run();
        }
        KMS_CATCH_RESULT(lResult);

        return lResult;
    }

    Simulator::Simulator()
        : mChipCount(0)
        , mReceiver(&mSystem, mInstances, 3)
        , mSender  (&mSystem, mInstances, 3)
    {
        mInstances[0] = &mDigitalInputs;
        mInstances[1] = &mDigitalOutputs;
        mInstances[2] = &mChipTypes;

        mChips.mOnChanged.Set(this, MSG_CHIPS_CHANGED);
        mChips.SetCreator(KMS::DI::UInt<uint16_t>::Create);

        AddEntry("Chips", &mChips, false, &MD_CHIPS);
        AddEntry("Port" , &mPort , false);
    }

    Simulator::~Simulator() {}

    void Simulator::AddChip(Chip* aChip)
    {
        assert(NULL != aChip);

        KMS_EXCEPTION_ASSERT(CHIP_MAX > mChipCount, APPLICATION_USER_ERROR, "Too many chips", "");

        mChips.SetEntry(mChipCount, aChip, true);

        unsigned int lInstanceCount = 3 + mChipCount;

        mInstances[lInstanceCount];

        mChipCount++;

        lInstanceCount++;

        mReceiver.SetInstanceCount(lInstanceCount);
        mSender  .SetInstanceCount(lInstanceCount);
    }

    // ===== KMS::Msg::Receive ==============================================

    unsigned int Simulator::Receive(void* aSender, unsigned int aCode, void* aData)
    {
        unsigned int lResult = MSG_IGNORED;

        switch (aCode)
        {
        case MSG_CHIPS_CHANGED: lResult = OnChipsChanged(); break;
        case MSG_RECEIVE      : lResult = OnReceive     (); break;
        case MSG_SEND         : lResult = OnSend        (); break;
        }

        return lResult;
    }

    // ===== KMS::CLI::Tool =============================================

    void Simulator::DisplayHelp(FILE* aOut) const
    {
        assert(NULL != aOut);

        fprintf_s(aOut,
            "Chip {Index} {Command}\n"
            "Connect\n"
            "Disconnect\n"
            "DI_Read {Index}\n"
            "DO_Clear {Index}\n"
            "DO_Get {Index}\n"
            "DO_Set {Index}\n"
            "Dump\n"
            "Start\n"
            "Stop\n");

        return KMS::CLI::Tool::DisplayHelp(aOut);
    }

    void Simulator::ExecuteCommand(const char* aC)
    {
        char         lCmd[LINE_LENGTH];
        unsigned int lIndex;

        if (0 == strcmp(aC, "Connect"   )) { mPort.Connect(0); return; }
        if (0 == strcmp(aC, "Disconnect")) { mPort.Disconnect(); return; }
        if (0 == strcmp(aC, "Dump"      )) { Dump (); return; }
        if (0 == strcmp(aC, "Start"     )) { Start(); return; }
        if (0 == strcmp(aC, "Stop"      )) { Stop (); return; }

        if (2 == sscanf_s(aC, "Chip %u %[^\n\r\t]", &lIndex, lCmd SizeInfo(lCmd))) { ChipCommand(lIndex, lCmd); return; }

        if (1 == sscanf_s(aC, "DI_Read %u\n" , &lIndex)) { DI_Read (lIndex); return; }
        if (1 == sscanf_s(aC, "DO_Clear %u\n", &lIndex)) { DO_Clear(lIndex); return; }
        if (1 == sscanf_s(aC, "DO_Get %u\n"  , &lIndex)) { DO_Get  (lIndex); return; }
        if (1 == sscanf_s(aC, "DO_Set %u\n"  , &lIndex)) { DO_Set  (lIndex); return; }

        KMS::CLI::Tool::ExecuteCommand(aC);
    }

    // Private
    // //////////////////////////////////////////////////////////////////////

    Chip* Simulator::GetChip(unsigned int aIndex)
    {
        KMS_EXCEPTION_ASSERT(mChipCount > aIndex, APPLICATION_USER_ERROR, "Invalid index", aIndex);

        KMS::DI::Object* lObject = mChips.GetEntry_RW(aIndex);
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
            const KMS::DI::Object* lObject = mChips.GetEntry_R(mChipCount);
            assert(NULL != lObject);

            const KMS::DI::UInt<uint16_t>* lType = dynamic_cast<const KMS::DI::UInt<uint16_t>*>(lObject);
            assert(NULL != lType);

            Chip* lChip = Chip::Create(lType->Get());
            if (NULL == lChip)
            {
                // TODO mChips.RemoveEntry(mChipCount);
                KMS_EXCEPTION(APPLICATION_USER_ERROR, "Invalid chip type", lType->Get());
            }

            AddChip(lChip);
        }
        else if (mChips.GetCount() == 0)
        {
            mChipCount = 0;
            mChipTypes.Clear();
        }

        return 0;
    }

    unsigned int Simulator::OnReceive()
    {
        uint8_t lBuffer[262];

        unsigned int lSize_byte = mPort.Read(lBuffer, sizeof(lBuffer), 0);
        if (0 < lSize_byte)
        {
            mReceiver.AddReceivedBytes(lBuffer, lSize_byte);
        }

        return 0;
    }

    unsigned int Simulator::OnSend()
    {
        const KMS::WOP::FrameBuffer* lFB = mSender.PrepareFrame();
        if (NULL != lFB)
        {
            mPort.Write(lFB->GetRawFrame(), lFB->GetFrameSize_byte());
        }

        return 0;
    }

    // ===== Commands =======================================================

    void Simulator::ChipCommand(unsigned int aId, const char* aCmd)
    {
        GetChip(aId)->ExecuteCommand(aCmd);
    }

    void Simulator::DI_Read(unsigned int aId)
    {
        if (mDigitalInputs.GetCount() <= aId)
        {
            std::cout << KMS::Console::Color::RED;
            std::cout << "USER ERROR  Invalid id" << KMS::Console::Color::WHITE << std::endl;
            return;
        }

        std::cout << "DI #" << aId << " = " << (mDigitalInputs.DI_Read(aId) ? "true" : "false") << std::endl;
    }

    void Simulator::DO_Clear(unsigned int aId)
    {
        if (mDigitalOutputs.GetCount() <= aId)
        {
            std::cout << KMS::Console::Color::RED;
            std::cout << "USER ERROR  Invalid id" << KMS::Console::Color::WHITE << std::endl;
            return;
        }

        mDigitalOutputs.DO_Clear(aId);

        std::cout << "Cleared" << std::endl;
    }

    void Simulator::DO_Get(unsigned int aId)
    {
        if (mDigitalOutputs.GetCount() <= aId)
        {
            std::cout << KMS::Console::Color::RED;
            std::cout << "USER ERROR  Invalid id" << KMS::Console::Color::WHITE << std::endl;
            return;
        }

        std::cout << "DO #" << aId << " = " << (mDigitalOutputs.DO_Get(aId) ? "true" : "false") << std::endl;
    }

    void Simulator::DO_Set(unsigned int aId)
    {
        if (mDigitalOutputs.GetCount() <= aId)
        {
            std::cout << KMS::Console::Color::RED;
            std::cout << "USER ERROR  Invalid id" << KMS::Console::Color::WHITE << std::endl;
            return;
        }

        mDigitalOutputs.DO_Set(aId);

        std::cout << "Set" << std::endl;
    }

    void Simulator::Dump()
    {
        mDigitalInputs .Dump();
        mDigitalOutputs.Dump();

        for (KMS::DI::Container::Entry& lEntry : mChips.mInternal)
        {
            Chip* lChip = dynamic_cast<Chip*>(lEntry.Get());
            assert(NULL != lChip);

            lChip->Dump();
        }
    }

    void Simulator::Stop()
    {
        mThread_Receiver.Stop();
        mThread_Sender  .Stop();

        mThread_Receiver.Wait(2000);
        mThread_Sender  .Wait(1000);

        std::cout << "Stopped" << std::endl;
    }

    void Simulator::Start()
    {
        mThread_Receiver.Start();
        mThread_Sender  .Start();
    }

}
