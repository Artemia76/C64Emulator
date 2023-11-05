#pragma once
#include <m6502/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <mutex>

using namespace m6502;

#define CRT_LINES 200
#define CRT_COL 640

class CCRT
{
    private:
        Mem& m_mem;
        Word m_video_mem;
        Word m_Video_Ptr;
        int m_mode; // Curent video mode
        sf::Image m_screen;

        int m_line; // Actual line
        int m_col; // Actual column
        std::mutex m_mutex; // Lock thread access
        s64 m_TimeCounter; // Counter of emaulated time

        Byte R0; // Horizontal total character number
        Byte R1; // Horizontal displayed character number
        Byte R2; // Position of horizontal sync. Pulse
        Byte R3; // Pulse width of horizontal sync. pulse
        Byte R4; // Vertical total character number
        Byte R5; // Total raster adjust
        Byte R6; // Vertical displayed character number
        Byte R7; // Position of vertical sync. Pulse
        Byte R8; // Interlace Mode and Skew
        Byte R9; // Max Scan Line Address
        Byte R10; // Cursor start
        Byte R11; // Cursor end
        Byte R12; // Display start address (High)
        Byte R13; // Display start adress (Low)
        Byte R14; // Cursor address (High)
        Byte R15; // Cursor address (Low)
        Byte R16; // Light pen (High)
        Byte R17; // Light pen (Low)

        Byte C0; // HCC (Horizontal Char Counter)
        Byte C9; // VLC (Vertical Line Counter)
        Byte C4; // VCC (Vertical Character Counter)
        Byte C3v; // VSC (Vertical Sync Counter)
        Byte C3h; // HSC (Horizontal Sync Counter)
        Byte C5; // VTAC (Vertical Total Adjust Counter)
        Word VMA; // Byte Pointer
        Word VMA2; // Byte Pointer

        bool VSYNC; // VSync Signal
        bool HSYNC; // HSync Signal

    public:
        CCRT(Mem& pMem);
        ~CCRT();
    /**
     * @brief Draw Screen during time given
     * 
     * @param pTime 
     */
    void    Execute(s64 pTime);
    void    RenderScreen(sf::RenderWindow& pWindow); //Rendering one frame;
};