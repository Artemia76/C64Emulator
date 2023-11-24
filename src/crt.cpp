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
    _screen.create(CRT_COL,CRT_LINES*2,sf::Color::Black);
    if (!_font.loadFromFile("assets/fonts/OpenSans-Regular.ttf"))
    {
        std::cout << "Error loading SFML Font" << std::endl;
    }
    _debug = false;
    _clock = 1000000; // Set Clock speed in Hz
    Byte R,V,B;
    R=V=B=0;
    for (Byte i=0 ; i< PALETTE.size(); i++)
    {
        PALETTE[i] = sf::Color(R,V,B);
        if (R==0xFE && B==0xFE)
        {
            V+=0x7F;
            R=0;
            B=0;
        } else
        if (B==0xFE)
        {
            R+=0x7F;
            B=0;
        } else
        B += 0x7F;
    }
    PEN[0] = PALETTE[1];
    PEN[1] = PALETTE[24];
    PAPER = PALETTE[1];
    BORDER = PALETTE[15];
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
        if (C9 == R9) // = 7 (CRT Char)
        {
            C9 = 0;
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
            else
            {
                C4++;
            }
        }
        else
        {
            C9++;
        }
    }
    //draw screen
    X = C0 * 8;
    Y = 2*(C4 * (R9+1) + C9);
    if (DISPEN)
    {
        sf::Color ColToDraw=PAPER;
        for (Byte j = 0 ; j < 2; j++)
        {
            Byte Px = CBusChip::bus.readBusData(VMA+j);
            switch (_mode)
            {
                case 2:
                {
                    for (Byte i = 0 ; i <8; i++)
                    {
                        ColToDraw = PEN[(Px >> i)&0x01];
                        _screen.setPixel(X+(j*8)+i, Y,ColToDraw);
                    }
                } break;
                case 1:
                {
                    for (Byte i = 0 ; i <4; i++)
                    {
                        ColToDraw = PEN[(Px >> (i*2))&0x03];
                        _screen.setPixel(X+(j*4)+i, Y,ColToDraw);
                        _screen.setPixel(X+(j*4)+i+1, Y,ColToDraw);
                    }
                } break;
                default:
                {
                    for (Byte i = 0 ; i <2; i++)
                    {
                        ColToDraw = PEN[(Px >> (i*4))&0x0F];
                        _screen.setPixel(X+(j*8)+i, Y,ColToDraw);
                        _screen.setPixel(X+(j*8)+i+1, Y,ColToDraw);
                        _screen.setPixel(X+(j*8)+i+2, Y,ColToDraw);
                        _screen.setPixel(X+(j*8)+i+3, Y,ColToDraw);
                    }

                }
            }
        }
    } else
    {
         for (Byte i = 0 ; i <16; i++)
        {
            _screen.setPixel(X+i, Y,BORDER);
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
            << ",VMA="
            << std::setfill('0') << std::setw(4) << std::setbase(16)
            << VMA;
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
    using namespace m6502;
    std::int64_t Interval = std::chrono::duration_cast<std::chrono::microseconds>(pInterval).count();
    // Compute how much clock cycle to do in interval
    std::int64_t ExpectedCycle = static_cast<double>(_clock/1000000.0) * Interval;
    if (ExpectedCycle < 1) ExpectedCycle = 1;
    while (ExpectedCycle > 0)
    {
        //Execute 1 clock cycle 
        Execute();
        ExpectedCycle--;
    }
}

void CCRT::setDebug (bool pDebug)
{
    _debug = pDebug;
}