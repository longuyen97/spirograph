#include <SFML/Graphics.hpp>
#include <cmath>
#include <random>
#include <sstream>
#include <chrono>

typedef float fp32_t;

#define PI 3.14159265
#define DEG2RAD(x) ((x)*PI/180)
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 1200
#define CENTER sf::Vector2f{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2}
#define BLACK sf::Color{0, 0, 0, 0}
#define WHITE sf::Color{255, 255, 255, 50}
#define START_BLUE sf::Color(125, 34, 34, 10)
#define END_BLUE sf::Color(34, 125, 34, 255)
#define SAVE_FILE 1
#define SPEED 10


using namespace std;
using namespace sf;

int main() {
    ContextSettings settings;
    settings.depthBits = 32;
    settings.sRgbCapable = true;
    settings.antialiasingLevel = 64;
    RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Spirograph", Style::Default, settings);

    sf::Texture texture;
    texture.create(window.getSize().x, window.getSize().y);

    // Initialize parameters
    int32_t arm_count = 4;

    vector<int> radii(arm_count);
    vector<int> thetas(arm_count);
    vector<RectangleShape> arms(arm_count);

    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
    std::mt19937 gen(ms); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> radius_dist(-100, 100);
    std::uniform_real_distribution<> theta_dist(-40, 40);
    for (int &r : radii) {
        r = radius_dist(gen);
    }

    for (int &t : thetas) {
        t = theta_dist(gen);
    }

    for (int i = 0; i < arms.size(); i++) {
        arms[i].setSize(Vector2f(1, radii[i]));
        arms[i].setOrigin(0, radii[i]);
        arms[i].setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
        arms[i].setFillColor(Color(125, 120, 175));
    }

    VertexArray tracer(LineStrip);
    VertexArray painter(TriangleFan);
    painter.append(Vertex(Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), START_BLUE));

    Clock frameTime;
    float deltaTime = 0;
    Event event;

    int filename;
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                break;
            }
        }

        for (int i = 0; i < arms.size(); i++) {
            if (i >= 1) {
                fp32_t delta_x = radii[i - 1] * sin(DEG2RAD(arms[i - 1].getRotation()));
                fp32_t delta_y = radii[i - 1] * -cos(DEG2RAD(arms[i - 1].getRotation()));
                arms[i].setPosition(arms[i - 1].getPosition() + Vector2f(delta_x, delta_y));
            }
            arms[i].rotate(thetas[i] * deltaTime);
        }

        fp32_t delta_x = radii.back() * sin(DEG2RAD(arms.back().getRotation()));
        fp32_t delta_y = radii.back() * -cos(DEG2RAD(arms.back().getRotation()));
        Vertex vertex(arms.back().getPosition() + Vector2f(delta_x, delta_y));

        tracer.append(vertex);
        vertex.color = END_BLUE;
        painter.append(vertex);

        window.clear(Color::Black);
        window.draw(painter);
        window.draw(tracer);
        for (auto &arm : arms) {
            window.draw(arm);
            sf::Vertex point(arm.getPosition(), sf::Color::Red);
            window.draw(&point, 1, sf::Points);
        }

        window.display();

        if(SAVE_FILE == true) {
            std::stringstream ss;
            texture.update(window);
            ss << to_string(filename++) << ".png";
            texture.copyToImage().saveToFile(ss.str());
        }


        if(SAVE_FILE == true) {
            deltaTime += 0.005;
        }else{
            deltaTime = 15 * frameTime.restart().asSeconds();
        }
    }
}
