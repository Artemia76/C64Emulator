#include <SFML/Graphics.hpp>
#include "crt.hpp"
#include "loop.hpp"
#include "mainapp.hpp"

int main()
{
    CLoop Loop;
    CMainApp MainLoop(Loop);
    auto window = sf::RenderWindow{ { CRT_COL, CRT_LINES }, "CMake SFML Project" };
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
    // Start Emulator Thread
    Loop.start();
    while (window.isOpen())
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        MainLoop.getCRT().RenderScreen(window);
    }
    Loop.stop();
}