#include <SFML/Graphics.hpp>
#include <cmath>

typedef float fp32_t;

#define PI 3.14159265
#define DEG2RAD(x) ((x + 90)*PI/180)
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 1200
#define CENTER sf::Vector2f{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2}
#define BLACK sf::Color{0, 0, 0, 0}
#define WHITE sf::Color{255, 255, 255, 50}
#define START_BLUE sf::Color(34, 34, 125, 10)
#define END_BLUE sf::Color(334, 125, 34, 255)
#define SAVE_FILE 0
#define SPEED 10

int main() {
    // Initialize context
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Spirograph", sf::Style::Default);
    sf::Event event;

    fp32_t firstArm = 100;
    fp32_t secondArm = 50;
    fp32_t thirdArm = 150;
    fp32_t firstTheta = 40;
    fp32_t secondTheta = -40;
    fp32_t thirdTheta = -20;

    sf::VertexArray tracer(sf::LineStrip);
    sf::VertexArray painter(sf::TriangleFan);
    painter.append(sf::Vertex(sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), sf::Color(34, 34, 125, 5)));

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
        firstCircle.setFillColor(sf::Color{0, 0, 0, 0});
        firstCircle.setOutlineColor(sf::Color::White);
        firstCircle.setOutlineThickness(1);

        sf::RectangleShape firstLine;
        firstLine.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
        firstLine.setFillColor(sf::Color::White);
        firstLine.rotate(firstTheta + time);
        firstLine.setSize(sf::Vector2f(firstArm, 1));

        // Second layer
        sf::RectangleShape secondLine;
        fp32_t delta_x = firstArm * sin(DEG2RAD(firstLine.getRotation()));
        fp32_t delta_y = firstArm * -cos(DEG2RAD(firstLine.getRotation()));

        secondLine.setPosition(firstLine.getPosition() + sf::Vector2f{delta_x, delta_y});
        secondLine.setFillColor(sf::Color::White);
        secondLine.rotate(firstTheta + time);
        secondLine.rotate(secondTheta + time);
        secondLine.setSize(sf::Vector2f(secondArm, 1));

        sf::CircleShape secondCircle(secondArm);
        secondCircle.setPosition(secondLine.getPosition() + secondLine.getPoint(secondArm));
        secondCircle.setFillColor(sf::Color{0, 0, 0, 0});
        secondCircle.setOutlineColor(sf::Color::White);
        secondCircle.setOutlineThickness(1);

        // Third layer
        sf::RectangleShape thirdLine;
        delta_x = thirdArm * sin(DEG2RAD(secondLine.getRotation()));
        delta_y = thirdArm * -cos(DEG2RAD(secondLine.getRotation()));

        thirdLine.setPosition(secondLine.getPosition() + sf::Vector2f{delta_x, delta_y});
        thirdLine.setFillColor(sf::Color::White);
        thirdLine.rotate(firstTheta + time);
        thirdLine.rotate(secondTheta + time);
        thirdLine.rotate(thirdTheta + time);
        thirdLine.setSize(sf::Vector2f(thirdArm, 1));

        sf::CircleShape thirdCircle(thirdArm);
        thirdCircle.setPosition(secondLine.getPosition() + sf::Vector2f{delta_x, delta_y} - sf::Vector2f{thirdArm, thirdArm});
        thirdCircle.setFillColor(sf::Color{0, 0, 0, 0});
        thirdCircle.setOutlineColor(sf::Color::White);
        thirdCircle.setOutlineThickness(1);

        // Display
        window.clear(sf::Color::Black);
        window.draw(firstCircle);
        window.draw(secondCircle);
        //window.draw(thirdCircle);
        window.draw(firstLine);
        window.draw(secondLine);
        //window.draw(thirdLine);
        window.display();

        time += 15 * clock.restart().asSeconds();
        time = std::abs(time);
    }
}