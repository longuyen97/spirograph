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
#define CENTER sf::Vector2f{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2}
#define BLACK sf::Color{0, 0, 0, 0}
#define WHITE sf::Color{255, 255, 255, 50}
#define START_BLUE sf::Color(34, 34, 125, 5)
#define END_BLUE sf::Color(34, 34, 125, 100)
#define SAVE_FILE 0


int main() {
    //Setting
    sf::ContextSettings contextSettings;
    contextSettings.antialiasingLevel = 64;
    contextSettings.depthBits = 32;

    // Initialize context
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Spirograph", sf::Style::Default,
                            contextSettings);
    sf::Event event;

    // Save file
    sf::Texture texture;
    texture.create(window.getSize().x, window.getSize().y);
    int fileName = 0;

    fp32_t firstArm = 100;
    fp32_t secondArm = 150;
    fp32_t thirdArm = 200;
    fp32_t firstTheta = 60;
    fp32_t secondTheta = -40;
    fp32_t thirdTheta = -20;

    sf::VertexArray tracer(sf::LineStrip);
    sf::VertexArray painter(sf::TriangleFan);
    painter.append(sf::Vertex(sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), START_BLUE));

    sf::Clock clock;
    fp32_t time;
    // Main loop
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                break;
            }
        }

        // First layer
        sf::CircleShape firstCircle(firstArm);
        firstCircle.setPosition(WINDOW_WIDTH / 2 - firstArm, WINDOW_HEIGHT / 2 - firstArm);
        firstCircle.setFillColor(BLACK);
        firstCircle.setOutlineColor(WHITE);
        firstCircle.setOutlineThickness(1);

        sf::RectangleShape firstLine;
        firstLine.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
        firstLine.setFillColor(WHITE);
        firstLine.rotate(firstTheta + time);
        firstLine.setSize(sf::Vector2f(firstArm, 1));

        // Second layer
        sf::RectangleShape secondLine;
        fp32_t delta_x = firstArm * sin(DEG2RAD(firstLine.getRotation() + 90));
        fp32_t delta_y = firstArm * -cos(DEG2RAD(firstLine.getRotation() + 90));

        secondLine.setPosition(firstLine.getPosition() + sf::Vector2f{delta_x, delta_y});
        secondLine.setFillColor(WHITE);
        secondLine.rotate(firstTheta + time);
        secondLine.rotate(secondTheta + time);
        secondLine.setSize(sf::Vector2f(secondArm, 1));

        sf::CircleShape secondCircle(secondArm);
        secondCircle.setPosition(
                firstLine.getPosition() + sf::Vector2f{delta_x, delta_y} - sf::Vector2f{secondArm, secondArm});
        secondCircle.setFillColor(BLACK);
        secondCircle.setOutlineColor(WHITE);
        secondCircle.setOutlineThickness(1);

        // Third layer
        sf::RectangleShape thirdLine;
        delta_x = secondArm * sin(DEG2RAD(secondLine.getRotation() + 90));
        delta_y = secondArm * -cos(DEG2RAD(secondLine.getRotation() + 90));

        thirdLine.setPosition(secondLine.getPosition() + sf::Vector2f{delta_x, delta_y});
        thirdLine.setFillColor(WHITE);
        thirdLine.rotate(firstTheta + time);
        thirdLine.rotate(secondTheta + time);
        thirdLine.rotate(thirdTheta + time);
        thirdLine.setSize(sf::Vector2f(thirdArm, 1));

        sf::CircleShape thirdCircle(thirdArm);
        thirdCircle.setPosition(
                secondLine.getPosition() + sf::Vector2f{delta_x, delta_y} - sf::Vector2f{thirdArm, thirdArm});
        thirdCircle.setFillColor(BLACK);
        thirdCircle.setOutlineColor(WHITE);
        thirdCircle.setOutlineThickness(1);

        // Vertex

        delta_x = thirdArm * sin(DEG2RAD(thirdLine.getRotation() + 90));
        delta_y = thirdArm * -cos(DEG2RAD(thirdLine.getRotation() + 90));
        sf::Vertex vertex(thirdLine.getPosition() + sf::Vector2f(delta_x, delta_y));

        tracer.append(vertex);
        vertex.color = END_BLUE;
        painter.append(vertex);

        // Display
        window.clear(sf::Color::Black);
        window.draw(firstCircle);
        window.draw(secondCircle);
        window.draw(thirdCircle);
        window.draw(firstLine);
        window.draw(secondLine);
        window.draw(thirdLine);
        window.draw(painter);
        window.draw(tracer);
        window.display();

        if(SAVE_FILE == 0) {
            time += 15 * clock.restart().asSeconds();
            time = std::abs(time);
        }else{
            texture.update(window);
            std::stringstream ss;
            ss << std::to_string(fileName++) << ".png";
            texture.copyToImage().saveToFile(ss.str());
            time += 1;
        }
    }
}
