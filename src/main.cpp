#include <cassert>
#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow sfmlWin(sf::VideoMode(600, 360), "Hello World SFML Window");
    while (sfmlWin.isOpen()) {

        sf::Event e;
        while (sfmlWin.pollEvent(e)) {

            switch (e.type) {
            case sf::Event::EventType::Closed:
                sfmlWin.close();
                break;
            }
        }

        sfmlWin.clear();
        sfmlWin.display();
    }
    return 0;
}