
#include <SFML/Graphics.hpp>
#include <optional>
#include <cmath>
#include <string>
#include <algorithm>

int main()
{
    sf::RenderWindow window(
        sf::VideoMode({ 800, 600 }),
        "SFML"
    );

    window.setFramerateLimit(60);

    float mass = 1.0f;
    float stiffness = 20.0f;
    float damping = 1.0f;

    float displacement = 120.0f;
    float velocity = 0.0f;

    const float equilibriumY = 300.0f;
    const float springTopY = 70.0f;
    const float gravity = 9.8f;

    bool running = true;

    sf::Clock clock;

    sf::RectangleShape massBlock({ 80.0f, 50.0f });
    massBlock.setOrigin({ 40.0f, 25.0f });
    massBlock.setFillColor(sf::Color(80, 170, 240));

    sf::RectangleShape springLine({ 4.0f, 1.0f });
    springLine.setFillColor(sf::Color::White);

    sf::Font font;
    bool fontLoaded = font.openFromFile(
        "C:/Windows/Fonts/arial.ttf"
    );

    sf::Text info(font, "", 18);
    info.setFillColor(sf::Color::White);
    info.setPosition({ 20.0f, 20.0f });

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (event->is<sf::Event::KeyPressed>())
            {
                const auto* key =
                    event->getIf<sf::Event::KeyPressed>();

                if (key->code == sf::Keyboard::Key::Space)
                    running = !running;

                if (key->code == sf::Keyboard::Key::R)
                {
                    displacement = 120.0f;
                    velocity = 0.0f;
                    running = true;
                }

                if (key->code == sf::Keyboard::Key::Up)
                    stiffness += 5.0f;

                if (key->code == sf::Keyboard::Key::Down)
                    stiffness = std::max(1.0f, stiffness - 5.0f);

                if (key->code == sf::Keyboard::Key::Right)
                    damping += 0.2f;

                if (key->code == sf::Keyboard::Key::Left)
                    damping = std::max(0.0f, damping - 0.2f);
            }
        }

        float dt = clock.restart().asSeconds();
        if (dt > 0.03f)
            dt = 0.03f;

        if (running)
        {

            float acceleration =
                (-stiffness * displacement
                    - damping * velocity) / mass;

            velocity += acceleration * dt;
            displacement += velocity * dt;
        }

        float blockY = equilibriumY + displacement;

        massBlock.setPosition({ 400.0f, blockY });

        sf::VertexArray spring(sf::PrimitiveType::LineStrip);
        const int coils = 12;
        const float springEndY = blockY - 25.0f;
        const float springLength = springEndY - springTopY;

        spring.append(sf::Vertex{
            {400.0f, springTopY}, sf::Color::White
            });

        for (int i = 1; i < coils * 2; ++i)
        {
            float y = springTopY + springLength * i / (coils * 2);
            float x = (i % 2 == 0) ? 400.0f : 375.0f;

            spring.append(sf::Vertex{
                {x, y}, sf::Color::White
                });
        }

        spring.append(sf::Vertex{
            {400.0f, springEndY}, sf::Color::White
            });

        window.clear(sf::Color(25, 30, 45));
        window.draw(spring);
        window.draw(massBlock);

        if (fontLoaded)
        {
            info.setString(
                "Spring oscillations\n"
                "Stiffness k: " + std::to_string(stiffness) +
                "\nDamping c: " + std::to_string(damping) +
                "\nDisplacement: " + std::to_string(displacement) +
                "\nVelocity: " + std::to_string(velocity) +
                "\nSPACE - pause / resume"
                "\nR - restart"
                "\nUP/DOWN - stiffness"
                "\nLEFT/RIGHT - damping"
            );

            window.draw(info);
        }

        window.display();
    }

    return 0;
}
