#pragma once
#include "loop.h"
#include <chrono>
#include <m6502/System.hpp>
#include "crt.h"

class CMainApp : public CProcessEvent
{
    public:
        CMainApp (CLoop& pParent);
        ~CMainApp ();
        CCRT&           getCRT();
    private:
        hrc::time_point m_timePoint;
	    m6502::Mem      mem;
	    m6502::CPU      cpu;
        CCRT            crt; // CRT Screen Emulator
        int             m_clock; // Clock in Mhz

    protected:
        void OnProcess(const period& pInterval);
};