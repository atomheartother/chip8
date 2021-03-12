#include <cassert>
#include <SFML/Graphics.hpp>
#include "Display/Display.hh"

int main() {
    Display display(64, 32);

    while (display.isOpen()) {

        sf::Event e;
        while (display.Poll(e)) {
            // Even if we don't do anything it's important to have an event loop
            // because Display::Poll() does some work here
        }
        display.SetPixel(8, 8);
        display.SetPixel(22, 22);
        display.Draw();
    }
    return 0;
}