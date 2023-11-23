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

protected:
    /**
     * @brief Process Event loop callback
     * 
     * @param pInterval : Period of loop
     */
    void onProcess(const period& pInterval);

private:
    hrc::time_point _timePoint;
    m6502::CBus _bus;
    m6502::CMem _mem;
    m6502::CCPU _cpu;
    CCRT _crt; // CRT Screen Emulator
    m6502::u64 _clock; // Clock in Hz
};

#endif