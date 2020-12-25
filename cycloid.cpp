#include <SFML/Graphics.hpp>
#include <cmath>
#include <random>
#include <sstream>
#include <iostream>
#include <chrono>

typedef float fp32_t;

#define PI 3.14159265
#define DEG2RAD(x) ((x + 90)*PI/180)
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 1200
#define CENTER sf::Vector2f{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2}
#define BLACK sf::Color{0, 0, 0, 0}
#define WHITE sf::Color{255, 255, 255, 50}
#define START_BLUE sf::Color(34, 34, 125, 10)
#define END_BLUE sf::Color(34, 125, 34, 255)
#define SAVE_FILE 1
#define SPEED 25


int main() {
    //Setting
    sf::ContextSettings contextSettings;
    contextSettings.antialiasingLevel = 32;
    contextSettings.depthBits = 32;

    // Initialize context
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Spirograph", sf::Style::Default,
                            contextSettings);
    sf::Event event;

    // Save file
    sf::Texture texture;
    texture.create(window.getSize().x, window.getSize().y);
    int32_t fileName = 0;

    // Initialize parameters
    int32_t arms = 2;
    std::vector<fp32_t> radii;
    std::vector<fp32_t> alphas;
    std::vector<bool> directions;

    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
    std::default_random_engine gen(ms);
    std::uniform_real_distribution<> radii_dist(100.0, 300.0);
    std::uniform_real_distribution<> alpha_dist(10, 350.0);
    std::uniform_real_distribution<> direction_dist(0, 100);
    for (int32_t i = 0; i < arms; i++) {
        radii.push_back(radii_dist(gen));
        alphas.push_back(alpha_dist(gen));
        directions.push_back(direction_dist(gen) > 50);
    }

    // End visualization
    sf::VertexArray tracer(sf::LineStrip);
    sf::VertexArray painter(sf::TriangleFan);
    painter.append(sf::Vertex(sf::Vector2f(CENTER), START_BLUE));

    sf::Clock clock;
    fp32_t time = 0;
    // Main loop
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                break;
            }
        }
        std::vector<sf::CircleShape> circles;
        std::vector<sf::RectangleShape> lines;

        for (int32_t i = 0; i < radii.size(); i++) {
            if (i == 0) {
                sf::CircleShape circle(radii[i]);
                circle.setPosition(CENTER - sf::Vector2f{radii[i], radii[i]});
                circle.setFillColor(BLACK);
                circle.setOutlineColor(WHITE);
                circle.setOutlineThickness(1);

                sf::RectangleShape line;
                line.setPosition(CENTER);
                line.setFillColor(WHITE);
                if (directions[i]) {
                    line.rotate(alphas[i] + time);
                } else {
                    line.rotate(alphas[i] - time);
                }
                line.setSize(sf::Vector2f(radii[i], 1));

                circles.push_back(std::move(circle));
                lines.push_back(std::move(line));
            } else {
                sf::RectangleShape line;
                fp32_t delta_x = radii[i - 1] * sin(DEG2RAD(lines.back().getRotation()));
                fp32_t delta_y = radii[i - 1] * -cos(DEG2RAD(lines.back().getRotation()));

                line.setPosition(lines.back().getPosition() + sf::Vector2f{delta_x, delta_y});
                line.setFillColor(WHITE);
                for (int32_t j = 0; j <= i; j++) {
                    if (directions[j]) {
                        line.rotate(alphas[j] + time);
                    } else {
                        line.rotate(alphas[j] - time);
                    }
                }
                line.setSize(sf::Vector2f(radii[i], 1));

                sf::CircleShape circle(radii[i]);
                circle.setPosition(
                        lines.back().getPosition() + sf::Vector2f{delta_x, delta_y} - sf::Vector2f{radii[i], radii[i]});
                circle.setFillColor(BLACK);
                circle.setOutlineColor(WHITE);
                circle.setOutlineThickness(1);

                lines.push_back(std::move(line));
                circles.push_back(std::move(circle));
            }
        }

        // Vertex
        fp32_t delta_x = radii.back() * sin(DEG2RAD(lines.back().getRotation()));
        fp32_t delta_y = radii.back() * -cos(DEG2RAD(lines.back().getRotation()));
        sf::Vertex vertex(lines.back().getPosition() + sf::Vector2f(delta_x, delta_y));

        tracer.append(vertex);
        vertex.color = END_BLUE;
        painter.append(vertex);

        // Display
        window.clear(sf::Color::Black);
        for (const auto &circle : circles) {
            window.draw(circle);
        }
        for (const auto &line : lines) {
            window.draw(line);
        }
        window.draw(painter);
        window.draw(tracer);
        window.display();

        if (SAVE_FILE == 0) {
            time += SPEED * clock.restart().asSeconds();
            time = std::abs(time);
        } else {
            texture.update(window);
            std::stringstream ss;
            ss << std::to_string(fileName++) << ".png";
            texture.copyToImage().saveToFile(ss.str());
            time += 5;
        }
    }
}
