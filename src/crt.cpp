#include "crt.h"

CCRT::CCRT(m6502::Mem& pMem) : m_mem(pMem)
{
    m_video_mem = 0xC000;
    m_Video_Ptr = m_video_mem;
    m_mode = 1;
    m_line = 0;
    m_col = 0;
    m_screen.create(CRT_COL,CRT_LINES*2,sf::Color::Blue);
    m_TimeCounter = 0;
    R0 = 63; // Width of a scanline
    R1 = 40; // OFF on end of scanline;
    R2 = 46; // HSync goes ON on the scan line
    R3 = 0x8E; // VSync width can only be changed on type 3 and 4
    R4 = 38; // Define the height of a screen
    R5 = 0; // Define additionnal scanlines at the end of a screen
    R6 = 25; // Define when DISPEN remains OFF until a new screen starts
    R7 = 30; // Define when the VSync goes ON on a screen
    R8 = 0; // Configure various stuff such as interlaced mode, skews, ...
    R9 = 7; // Define the height of a CRTC-Char in scanline
    R10 = 0; // Useless on the Amstrad CPC/Plus (text-mode is not wired)
    R11 = 0; // Useless on the Amstrad CPC/Plus (text-mode is not wired)
    R12 = 0xC0; // Define the MSB of MA when a CRTC-screen starts
    R13 = 0x00; //Define the LSB of MA when a CRTC-screen starts
    R14 = 0; //Useless on the Amstrad CPC/Plus (text-mode is not wired)
    R15 = 0; //Useless on the Amstrad CPC/Plus (text-mode is not wired)
    R16 = 0; //Hold the MSB of the cursor position when the lightpen was ON
    R17 = 0; //Hold the LSB of the cursor position when the lightpen was ON
    C0 = 0;
    C9 = 0;
    C4 = 0;
    C5 = 0;
    VMA = R12;
    VMA = (R12 << 8) | R13;
    printf("VMA = %04X\n", VMA);
}

CCRT::~CCRT()
{

}

void CCRT::Execute(s64 pTime)
{
    m_TimeCounter += pTime;
    if (m_TimeCounter >= 1000) // 1µSec 
    {
        m_TimeCounter -=1000;
        Byte Char = m_mem[VMA];

        //Counters
        C0++;
        if (C0 == R0) // = 63 Width of screen in char
        {
            C0 = 0;
            C9++;
            if (C9 == R9) // = 7 (CRT Char)
            {
                C9 = 0;
                C4++;
                if (C4 == R4) // = 38 Height of screen in char
                {
                    if (C5 == R5) // = 0 Additional scan line
                    {
                        C4 = 0;
                        C5 = 0;
                        VMA = (R12 << 8) | R13;
                    }
                    else
                    {
                        C5++;
                    }
                }
            }
        }

        //Horizontal Sync
        if (C0 == R2) // = 46 Start of HSync
        {
            HSYNC = true;
            C3h=0;
        }
        if (HSYNC)
        {
            if((R3 & 0x0F)>0)
            {
                C3h ++;
            }
            if (C3h == (R3 & 0x0F)) // = 14 
            {
                HSYNC = false;
            }
        }
        //Vertical Sync
        if (C4 == R7) // = 30 Start of VSync
        {
            VSYNC = true;
            C3v=0;
        }
        if (VSYNC)
        {
            C3v++;
            if (C3v == ((R1 & 0xF0)>>4))
            {
                VSYNC=false;
            }
        }
    }
}

void CCRT::RenderScreen(sf::RenderWindow& pWindow)
{
    pWindow.clear();
    sf::Texture texture;
    texture.loadFromImage(m_screen);
    sf::Sprite sprite;
    sprite.setTexture(texture);
    pWindow.draw(sprite);
    pWindow.display();
}
