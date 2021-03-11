#include <cassert>
#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow sfmlWin(sf::VideoMode(64, 32), "Hello World SFML Window");
    sf::Image image;
    image.create(64, 32, sf::Color(255,0,0));
    sf::Texture texture;
    texture.loadFromImage(image);

    sf::Sprite sprite;
    sprite.setTexture(texture);

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
        sfmlWin.draw(sprite);
        sfmlWin.display();
    }
    return 0;
}