#include <SFML/Graphics.hpp>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream> /*cout, endl*/

int main()
{
    int random_num;
    srand(time(NULL));


    uint16_t width = 1920;
    uint16_t height = 1080;

    sf::RenderWindow window(sf::VideoMode(width, height), "SFML works!");
    //sf::CircleShape shape(100.f);
    //shape.setFillColor(sf::Color::Green);

    sf::Vertex border[] = {
        sf::Vertex(sf::Vector2f(0.f, 100.f), sf::Color::Black),
        sf::Vertex(sf::Vector2f(width, 100.f), sf::Color::Black)
    };


    sf::VertexArray x_axis(sf::Lines);
    x_axis.append(sf::Vector2f(100.f, height - 100.f));
    x_axis.append(sf::Vector2f(width - 100.f, height - 100.f));

    sf::VertexArray y_axis(sf::Lines);
    x_axis.append(sf::Vector2f(100.f, 100.f));
    x_axis.append(sf::Vector2f(100.f, height - 100.f));

    sf::VertexArray linestrip_test(sf::LineStrip);
    linestrip_test.append(sf::Vertex(sf::Vector2f(100.f, height - 100.f), sf::Color::Red));

    sf::PrimitiveType m_primitiveType;
    std::vector<sf::Vertex> m_verticies;
    m_verticies.push_back(sf::Vertex(sf::Vector2f(100.f, height - 100.f), sf::Color::Red));


    // (0,0) = (100.f, height - 100.f)

    uint16_t offset = 2;

    while (window.isOpen())
    {   
        random_num = std::rand() % height + 1;
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(x_axis);
        window.draw(y_axis);
        //window.draw(linestrip_test);
        window.draw(m_verticies.data(), m_verticies.size(), m_primitiveType);
        window.display();


        /*if (height - 100.f - offset == 100.f) {
        
        }
        else {
            linestrip_test.append(sf::Vertex(sf::Vector2f(100.f + offset, height - 100.f - offset), sf::Color::Red));
            offset += 1;
        }*/
        //linestrip_test.append(sf::Vertex(sf::Vector2f(100.f + offset, height - 100.f - random_num), sf::Color::Red));
        
        m_verticies.push_back(sf::Vertex(sf::Vector2f(100.f + offset, height - 100.f - random_num), sf::Color::Red));
        offset += 10;


    }

    return 0;
}