#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream> /*cout, endl*/
#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>

#include "SelbaWard/Line.hpp"
#include "SelbaWard/Spline.hpp"

#include "SocketManager.cpp"

#include <string.h>

using namespace std::chrono;
using namespace std;

static const float VIEW_HEIGHT = 1080.0f;
static const float VIEW_WIDTH = 1920.0f;


// -------------------------------------- For Socket --------------------------------------
int main()
{
	// Graph Initialize

	SocketManager TestSocket;
	string test = TestSocket.GetTitle();

	int scale = 10;

    auto next = steady_clock::now();
    auto prev = next - std::chrono::milliseconds(scale);

    int random_num;
    srand(time(NULL));

    sf::RenderWindow window(sf::VideoMode(VIEW_WIDTH, VIEW_HEIGHT), test);

    // create default view
    sf::View view(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(VIEW_WIDTH, VIEW_HEIGHT));

    // for x & y axis (Graph)
    sf::VertexArray x_axis(sf::Lines);
    x_axis.append(sf::Vector2f(100.f, VIEW_HEIGHT - 100.f));
    x_axis.append(sf::Vector2f(VIEW_WIDTH - 100.f, VIEW_HEIGHT - 100.f));

    sf::VertexArray y_axis(sf::Lines);
    y_axis.append(sf::Vector2f(100.f, 100.f));
    y_axis.append(sf::Vector2f(100.f, VIEW_HEIGHT - 100.f));

	// new method to draw chart.
	// SFML lines cannot have a thickness
	// need to draw rectangles instead
	sf::VertexArray quad_x(sf::Quads, 4);
	// Top Left
	quad_x[0].position = sf::Vector2f(100.f, VIEW_HEIGHT - 100.f);
	quad_x[0].color = sf::Color::White;
	// Bottom Left
	quad_x[1].position = sf::Vector2f(100.f, VIEW_HEIGHT - 100.f + 10.f);
	quad_x[1].color = sf::Color::White;
	// Bottom Right
	quad_x[2].position = sf::Vector2f(VIEW_WIDTH - 100.f, VIEW_HEIGHT - 100.f + 10.f);
	quad_x[2].color = sf::Color::White;
	// Top Right
	quad_x[3].position = sf::Vector2f(VIEW_WIDTH - 100.f, VIEW_HEIGHT - 100.f);
	quad_x[3].color = sf::Color::White;

	// y-axis:
	sf::VertexArray quad_y(sf::Quads, 4);
	// Top Left
	quad_y[0].position = sf::Vector2f(100.f, 100.f);
	quad_y[0].color = sf::Color::White;
	// Bottom Left 
	quad_y[1].position = sf::Vector2f(100.f, VIEW_HEIGHT - 100.f);
	quad_y[1].color = sf::Color::White;
	// Bottom Right 
	quad_y[2].position = sf::Vector2f(110.f, VIEW_HEIGHT - 100.f);
	quad_y[2].color = sf::Color::White;
	// Top Right
	quad_y[3].position = sf::Vector2f(110.f, 100.f);
	quad_y[3].color = sf::Color::White;
	
	// Testing Spline
	sw::Spline spline{};
	spline.setThickness(10);
	spline.setColor(sf::Color::Red);
	// update always last after all modifications
	spline.update();

    // data stream
    std::vector<sf::Vertex> m_verticies;
    m_verticies.push_back(sf::Vertex(sf::Vector2f(100.f, VIEW_HEIGHT - 100.f), sf::Color::Red));

    uint16_t offset = 2;

    view.setCenter(VIEW_WIDTH / 2, VIEW_HEIGHT / 2);

    while (window.isOpen())
    {   
		// This is where we need to read info from the socket.

		vector<double> read_result = TestSocket.Read();
		double piTime = read_result[0];
		double emgGAS = read_result[1];
		double emgTA = read_result[2];

		printf("piTime: %.15g - emgGAS: %.15g - emgTA: %.15g\n", piTime, emgGAS, emgTA);

		// new - taking old emg data and plotting it
		// data is on scale 0.000199 to 0.0258
		double emgGasScaled = emgGAS * 2500;

		int vertex_position = 0; 

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        if (m_verticies.size() == VIEW_WIDTH / scale) {
            // start scrolling
            // erase first item in vector
            m_verticies.erase(m_verticies.begin());
            // push_back new vertex/point
            m_verticies.push_back(sf::Vertex(sf::Vector2f(230.f + offset, VIEW_HEIGHT - 100.f - emgGasScaled), sf::Color::Red));
            offset += scale;
            view.setCenter(-(VIEW_HEIGHT/2) -300 + offset, VIEW_HEIGHT/2);

			// Top Right
			quad_x[0].position = sf::Vector2f(offset, VIEW_HEIGHT - 100.f);
			// Bottom Right
			quad_x[1].position = sf::Vector2f(offset, VIEW_HEIGHT - 100.f + 10.f);
			// Bottom Left
			quad_x[2].position = sf::Vector2f(-VIEW_WIDTH + 220.f + offset, VIEW_HEIGHT - 100.f + 10.f);
			// Top Left
			quad_x[3].position = sf::Vector2f(- VIEW_WIDTH + 220.f + offset, VIEW_HEIGHT - 100.f);

			// Top Left
			quad_y[0].position = sf::Vector2f(-VIEW_WIDTH + 220.f + offset, 100.f);
			// Bottom Left 
			quad_y[1].position = sf::Vector2f(-VIEW_WIDTH + 220.f + offset, VIEW_HEIGHT - 100.f);
			// Bottom Right 
			quad_y[2].position = sf::Vector2f(-VIEW_WIDTH + 230.f + offset, VIEW_HEIGHT - 100.f);
			// Top Right
			quad_y[3].position = sf::Vector2f(-VIEW_WIDTH + 230.f + offset, 100.f);

			sf::Vector2f v1(215.f + offset, VIEW_HEIGHT - 100.f - emgGasScaled);
			sw::Spline spline2{ 1, v1 };

			spline2.setThickness(10);
			// update always last after all modifications
			spline.addSplineToBack(spline2);
			spline.update();

			// rolling window to remove verticies of old.
			spline.removeVertices(vertex_position, 1);
			spline.update();
			vertex_position += scale;

        }else{
            m_verticies.push_back(sf::Vertex(sf::Vector2f(100.f + offset, VIEW_HEIGHT - 100.f - emgGasScaled), sf::Color::Red));
			
			sf::Vector2f v1(230.f + offset, VIEW_HEIGHT - 100.f - emgGasScaled);
			sw::Spline spline2{1, v1};

			spline2.setThickness(10);
			// update always last after all modifications
			spline.addSplineToBack(spline2);
			spline.update();

            offset += scale;
        }

        window.clear();
        window.draw(quad_x);
		window.draw(quad_y);
		window.draw(spline);
        window.setView(view);

		//printf("Vertex Count: %d", spline.getVertexCount());

        window.display();
        

        // do stuff
        auto now = steady_clock::now();
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(now-prev).count() << std::endl;
        prev = now;

        // delay until time to iterate again
        next += std::chrono::milliseconds(scale);
        std::this_thread::sleep_until(next);

    }

    return 0;
}