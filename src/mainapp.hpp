#ifndef MAINAPP_HPP
#define MAINAPP_HPP

#include "loop.hpp"
#include <chrono>
#include <m6502/System.hpp>
#include "crt.hpp"

class CMainApp : public CProcessEvent
{
    public:
        CMainApp (CLoop& pParent);
        ~CMainApp ();
        CCRT& getCRT();
    private:
        hrc::time_point m_timePoint;
        m6502::CBus bus;
	    m6502::CMem mem;
	    m6502::CCPU cpu;
        CCRT crt; // CRT Screen Emulator
        int m_clock; // Clock in Mhz

    protected:
        void OnProcess(const period& pInterval);
};

#endif