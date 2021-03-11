#include <cassert>
#include <SFML/Graphics.hpp>
#include "Display/Display.hh"

int main() {
    const auto dm = sf::VideoMode::getDesktopMode();
    Display display(dm.width, dm.height);

    while (display.isOpen()) {

        sf::Event e;
        while (display.Poll(e)) {

            switch (e.type) {
            case sf::Event::EventType::Closed:
                display.Close();
                break;
            }
        }
    }
    return 0;
}