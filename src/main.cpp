#include <cassert>
#include <SFML/Graphics.hpp>
#include "Display/Display.hh"

int main() {
    Display display(64, 32);

    while (display.isOpen()) {

        sf::Event e;
        while (display.Poll(e)) {

            switch (e.type) {
            case sf::Event::EventType::Closed:
                display.Close();
                break;
            }
        }
        display.SetPixel(8, 8);
        display.SetPixel(22, 22);
        display.Draw();
    }
    return 0;
}