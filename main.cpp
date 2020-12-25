#include <SFML/Graphics.hpp>
#include <cmath>
#include <random>
#include <sstream>
#include <iostream>

typedef float fp32_t;
typedef double fp64_t;

#define PI 3.14159265
#define DEG2RAD(x) ((x)*PI/180)
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000
#define LOG(m) std::cout << m << std::endl;

int main() {
    // Initialize context
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Spirograph", sf::Style::Default);
    sf::Event event;

    int32_t firstArm = 100;
    int32_t secondArm = 50;
    fp32_t firstTheta = 40;
    fp32_t secondTheta = -40;


    sf::Clock clock;
    fp32_t time;
    // Main loop
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                break;
            }
        }


        sf::CircleShape firstCircle(firstArm);
        firstCircle.setPosition(WINDOW_WIDTH / 2 - firstArm, WINDOW_HEIGHT / 2 - firstArm);
        firstCircle.setFillColor(sf::Color::Black);
        firstCircle.setOutlineColor(sf::Color::White);
        firstCircle.setOutlineThickness(1);

        sf::RectangleShape firstLine;
        firstLine.setFillColor(sf::Color::White);
        firstLine.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
        firstLine.rotate(firstTheta + time);
        firstLine.setSize(sf::Vector2f(firstArm, 1));

        sf::RectangleShape secondLine;
        sf::RectangleShape thirdLine;

        window.clear(sf::Color::Black);
        window.draw(firstCircle);
        window.draw(firstLine);
        window.draw(secondLine);
        window.draw(thirdLine);
        window.display();

        time += 15 * clock.restart().asSeconds();
        time = std::abs(time);
        LOG(time);
    }
}
