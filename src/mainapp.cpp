#include <cinttypes>
#include <iostream>
#ifdef WIN32
#include <Windows.h>
#endif
#include "mainapp.hpp"

/**
 * @brief Construct a new CMainApp::CMainApp object
 * 
 */
CMainApp::CMainApp(CLoop& pParent) : CProcessEvent(pParent), _mem(_bus, 0x0000, 0x0000), _cpu(_bus), _crt(_bus,pParent)
{
#ifdef WIN32
    // Set console code page to UTF-8 so console known how to interpret string data
    SetConsoleOutputCP(CP_UTF8);

    // Enable buffering to prevent VS from chopping up UTF-8 byte sequences
    setvbuf(stdout, nullptr, _IOFBF, 1000);
#endif
    _timePoint = hrc::now();
    using namespace m6502;
	_cpu.reset( 0x4000 );
   // when:
	/*
	* = $4000

	ldx #00
    start
	inx
  	jmp start
	*/
	Byte TestPrg[] = 
		{ 0x00,0x40,0xA2,0x00,0xE8,0x4C,0x02,0x40 };

	Word StartAddress = _cpu.loadPrg( TestPrg, sizeof(TestPrg) );

    _clock = 3000000; // Set Clock speed in Hz
    _crt.setDebug(true);
}

/**
 * @brief Destroy the CMainApp::CMainApp object
 * 
 */
CMainApp::~CMainApp()
{

}

/**
 * @brief 
 * 
 */
void CMainApp::onProcess(const period& pInterval)
{
    using namespace m6502;
    hrc::time_point start = hrc::now();
    std::int64_t Interval = std::chrono::duration_cast<std::chrono::microseconds>(pInterval).count();
    // Compute how much clock cycle to do in interval
    std::int64_t ExpectedCycle = static_cast<double>(_clock/1000000.0) * Interval;
    std::int64_t ExecutedCycle = 0;
    std::int64_t Cycle_Time = std::chrono::nanoseconds((Interval*1000) / ExpectedCycle).count();
    ExecutedCycle = _cpu.execute( ExpectedCycle );
    hrc::time_point end = hrc::now();
    std::int64_t ExecTime = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();


    // Display statistic every second 
    if (start > (_timePoint + std::chrono::seconds(1)))
    {
        _timePoint = start;
        std::cout << "Top 1 second : Calling interval = " << Interval
                  << " µsec , Execution Time = " << ExecTime
                  << " µsec , Cycles Executed = " << ExpectedCycle
                  << " CPU X = " << std::hex << static_cast<int>(_cpu.X) << std::dec
                  << " CPU Cycle Time = " << Cycle_Time << " nsec"
                  << " SleepTime = " << getLastSleep().count() << " µsec"
                  << std::endl;
    }
}

CCRT& CMainApp::getCRT()
{
    return _crt;
}