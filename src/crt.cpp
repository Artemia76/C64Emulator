#include "crt.hpp"
#include <sstream>
#include <iomanip>

CCRT::CCRT(CBus& pBus, CLoop& pLoop) : CBusChip(pBus, 0xFFFF, 0), CProcessEvent(pLoop)
{
    _videoMem = 0xC000;
    _videoPtr = _videoMem;
    _mode = 1;
    _line = 0;
    _col = 0;
    _timeCounter = 0;
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
    DISPEN = false;
    X=Y=0;
    //Init SFML
    _screen.create(CRT_COL,CRT_LINES*2,sf::Color::Blue);
    if (!_font.loadFromFile("assets/fonts/OpenSans-Regular.ttf"))
    {
        std::cout << "Error loading SFML Font" << std::endl;
    }
    _debug = false;
    _clock = 4; // Set Clock speed in Hz
}

CCRT::~CCRT()
{

}

void CCRT::Execute()
{
    Byte Char = bus.readBusData(VMA);

    DISPEN = ( (C0 < R1) && ( C4 < R6)) ? true : false;

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

    if (DISPEN)
    {
        VMA += 2 ;
    }
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
}

void CCRT::RenderScreen(sf::RenderWindow& pWindow)
{

    pWindow.clear();
    sf::Texture texture;
    texture.loadFromImage(_screen);
    sf::Sprite sprite;
    sprite.setTexture(texture);
    pWindow.draw(sprite);
    if (_debug)
    {
        sf::Text text;
        // select the font
        text.setFont(_font); // font is a sf::Font
        std::ostringstream  dbg_mess;
        dbg_mess
            << "C0="
            << std::setfill('0') << std::setw(2)
            << static_cast<int>(C0)
            << ",C9="
            << std::setfill('0') << std::setw(2)
            << static_cast<int>(C9)
            << ",C4="
            << std::setfill('0') << std::setw(2)
            << static_cast<int>(C4);
        // set the string to display
        text.setString(dbg_mess.str());

        // set the character size
        text.setCharacterSize(16); // in pixels, not points!

        // set the color
        text.setFillColor(sf::Color::Red);

        // set the text style
        //text.setStyle(sf::Text::Bold | sf::Text::Underlined);
        pWindow.draw(text);
    }
    pWindow.display();
}

/**
 * @brief 
 * 
 */
void CCRT::onProcess(const period& pInterval)
{
    Execute();
}

void CCRT::setDebug (bool pDebug)
{
    _debug = pDebug;
}