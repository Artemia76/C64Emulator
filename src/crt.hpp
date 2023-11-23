#ifndef CRT_HPP
#define CRT_HPP

#include "loop.hpp"
#include <m6502/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Font.hpp>
#include <mutex>

using namespace m6502;

#define CRT_LINES 312
#define CRT_COL 512

/**
 * @brief Emulate CRTC 6845 and CRT Display
 * 
 */
class CCRT : public CBusChip, CProcessEvent
{
public:
    CCRT(CBus& pBus, CLoop& pLoop);
    ~CCRT();
    
    /**
     * @brief Draw Screen during time given
     * 
     */
    void Execute();

    /**
     * @brief Rendering one frame
     * 
     * @param pWindow 
     */
    void RenderScreen(sf::RenderWindow& pWindow);

    void setDebug (bool pDebug = false);

protected:
    /**
     * @brief Process Event loop callback
     * 
     * @param pInterval : Period of loop
     */
    void onProcess(const period& pInterval);

private:
    sf::Color PALETTE[27];
    sf::Color PEN[16];
    sf::Color PAPER;
    bool _debug;
    Word _videoMem;
    Word _videoPtr;
    int _mode; // Curent video mode
    sf::Image _screen;
    sf::Font _font;

    int _line; // Actual line
    int _col; // Actual column
    std::mutex _mutex; // Lock thread access
    s64 _timeCounter; // Counter of emaulated time
    m6502::u64 _clock; // Clock in Hz
    /**
     * @brief Horizontal total character number
     * 
     */
    Byte R0;

    /**
     * @brief Horizontal displayed character number
     * 
     */
    Byte R1;

    /**
     * @brief Position of horizontal sync. Pulse
     * 
     */
    Byte R2;

    /**
     * @brief Pulse width of horizontal sync. pulse
     * 
     */
    Byte R3;

    /**
     * @brief Vertical total character number
     * 
     */
    Byte R4;

    /**
     * @brief Total raster adjust
     * 
     */
    Byte R5;

    /**
     * @brief Vertical displayed character number
     * 
     */
    Byte R6;

    /**
     * @brief Position of vertical sync. Pulse
     * 
     */
    Byte R7;

    /**
     * @brief Interlace Mode and Skew
     * 
     */
    Byte R8;

    /**
     * @brief Max Scan Line Address
     * 
     */
    Byte R9;

    /**
     * @brief Cursor start
     * 
     */
    Byte R10;

    /**
     * @brief Cursor end
     * 
     */
    Byte R11;

    /**
     * @brief Display start address (High)
     * 
     */
    Byte R12;

    /**
     * @brief Display start adress (Low)
     * 
     */
    Byte R13;

    /**
     * @brief Cursor address (High)
     * 
     */
    Byte R14;

    /**
     * @brief Cursor address (Low)
     * 
     */
    Byte R15;

    /**
     * @brief Light pen (High)
     * 
     */
    Byte R16;

    /**
     * @brief Light pen (Low)
     * 
     */
    Byte R17;

    /**
     * @brief HCC (Horizontal Char Counter)
     * 
     */
    Byte C0;

    /**
     * @brief VLC (Vertical Line Counter)
     * 
     */

    /**
     * @brief VLC (Vertical Line Counter)
     * 
     */
    Byte C9;

    /**
     * @brief VCC (Vertical Character Counter)
     * 
     */
    Byte C4;

    /**
     * @brief VSC (Vertical Sync Counter)
     * 
     */
    Byte C3v; // VSC (Vertical Sync Counter)

    /**
     * @brief HSC (Horizontal Sync Counter)
     * 
     */
    Byte C3h;

    /**
     * @brief VTAC (Vertical Total Adjust Counter)
     * 
     */
    Byte C5;

    /**
     * @brief Video Memory Pointer
     * 
     */
    Word VMA;
    Word VMA2; // Byte Pointer

    bool VSYNC; // VSync Signal
    bool HSYNC; // HSync Signal
    
    /**
     * @brief Draw Border when off
     *        Else Draw Video Memory
     */
    bool DISPEN;

    Word X,Y;

    
};

#endif