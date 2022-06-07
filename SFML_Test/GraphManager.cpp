#include "GraphManager.h"

GraphManager::GraphManager(){ 
	// initilization of Graph
	this->TestTitle = TestSocket.GetTitle();
	this->TrialDuration = TestSocket.GetDuration();

	this->scale = 10;
	this->next = chrono::steady_clock::now();
	this->prev = this->next - chrono::milliseconds(this->scale);

	this->window.create(sf::VideoMode(VIEW_WIDTH, VIEW_HEIGHT), this->TestTitle);

	// create default view
	this->view = sf::View(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(VIEW_WIDTH, VIEW_HEIGHT));

	// for x & y axis (Graph)
	this->x_axis.setPrimitiveType(sf::Lines);
	this->x_axis.append(sf::Vector2f(100.f, VIEW_HEIGHT - 100.f));
	this->x_axis.append(sf::Vector2f(VIEW_WIDTH - 100.f, VIEW_HEIGHT - 100.f));

	this->y_axis.setPrimitiveType(sf::Lines);
	this->y_axis.append(sf::Vector2f(100.f, 100.f));
	this->y_axis.append(sf::Vector2f(100.f, VIEW_HEIGHT - 100.f));

	// x-axis:
	this->quad_x.setPrimitiveType(sf::Quads);
	// Top Left
	this->quad_x.append(sf::Vector2f(100.f, VIEW_HEIGHT - 100.f));
	this->quad_x[0].color = sf::Color::White;
	// Bottom Left
	this->quad_x.append(sf::Vector2f(100.f, VIEW_HEIGHT - 100.f + 10.f));
	this->quad_x[1].color = sf::Color::White;
	// Bottom Right
	this->quad_x.append(sf::Vector2f(VIEW_WIDTH - 100.f, VIEW_HEIGHT - 100.f + 10.f));
	this->quad_x[2].color = sf::Color::White;
	// Top Right
	this->quad_x.append(sf::Vector2f(VIEW_WIDTH - 100.f, VIEW_HEIGHT - 100.f));
	this->quad_x[3].color = sf::Color::White;

	// y-axis:
	this->quad_y.setPrimitiveType(sf::Quads);
	// Top Left
	this->quad_y.append(sf::Vector2f(100.f, 100.f));
	this->quad_y[0].color = sf::Color::White;
	// Bottom Left 
	this->quad_y.append(sf::Vector2f(100.f, VIEW_HEIGHT - 100.f));
	this->quad_y[1].color = sf::Color::White;
	// Bottom Right 
	this->quad_y.append(sf::Vector2f(110.f, VIEW_HEIGHT - 100.f));
	this->quad_y[2].color = sf::Color::White;
	// Top Right
	this->quad_y.append(sf::Vector2f(110.f, 100.f));
	this->quad_y[3].color = sf::Color::White;

	this->spline.setThickness(10);
	this->spline.setColor(sf::Color::Red);
	// update always last after all modifications
	this->spline.update();

	// data stream
	this->m_verticies.push_back(sf::Vertex(sf::Vector2f(100.f, VIEW_HEIGHT - 100.f), sf::Color::Red));

	this->offset = 2;

	this->view.setCenter(VIEW_WIDTH / 2, VIEW_HEIGHT / 2);
}
void GraphManager::OpenWindow()
{
	while (this->window.isOpen()) 
	{
		this->read_result = this->TestSocket.Read();
		double piTime = read_result[0];
		double emgGAS = read_result[1];
		double emgTA = read_result[2];
		printf("piTime: %.15g - emgGAS: %.15g - emgTA: %.15g\n", piTime, emgGAS, emgTA);
		cout << "TrialDuration: " << this->TrialDuration << endl;
		// new - taking old emg data and plotting it
		// data is on scale 0.000199 to 0.0258
		double emgGasScaled = emgGAS * 2500;

		int vertex_position = 0;

		sf::Event event;
		while (this->window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				this->window.close();
		}
		if (this->m_verticies.size() == VIEW_WIDTH / this->scale) {
			// start scrolling
			// erase first item in vector
			this->m_verticies.erase(this->m_verticies.begin());
			// push_back new vertex/point
			this->m_verticies.push_back(sf::Vertex(sf::Vector2f(230.f + this->offset, VIEW_HEIGHT - 100.f - emgGasScaled), sf::Color::Red));
			offset += scale;
			this->view.setCenter(-(VIEW_HEIGHT / 2) - 300 + this->offset, VIEW_HEIGHT / 2);

			// Top Right
			this->quad_x[0].position = sf::Vector2f(this->offset, VIEW_HEIGHT - 100.f);
			// Bottom Right
			this->quad_x[1].position = sf::Vector2f(this->offset, VIEW_HEIGHT - 100.f + 10.f);
			// Bottom Left
			this->quad_x[2].position = sf::Vector2f(-VIEW_WIDTH + 220.f + this->offset, VIEW_HEIGHT - 100.f + 10.f);
			// Top Left
			this->quad_x[3].position = sf::Vector2f(-VIEW_WIDTH + 220.f + this->offset, VIEW_HEIGHT - 100.f);

			// Top Left
			this->quad_y[0].position = sf::Vector2f(-VIEW_WIDTH + 220.f + this->offset, 100.f);
			// Bottom Left 
			this->quad_y[1].position = sf::Vector2f(-VIEW_WIDTH + 220.f + this->offset, VIEW_HEIGHT - 100.f);
			// Bottom Right 
			this->quad_y[2].position = sf::Vector2f(-VIEW_WIDTH + 230.f + this->offset, VIEW_HEIGHT - 100.f);
			// Top Right
			this->quad_y[3].position = sf::Vector2f(-VIEW_WIDTH + 230.f + this->offset, 100.f);

			sf::Vector2f v1(215.f + this->offset, VIEW_HEIGHT - 100.f - emgGasScaled);
			sw::Spline spline2{ 1, v1 };

			spline2.setThickness(10);
			// update always last after all modifications
			this->spline.addSplineToBack(spline2);
			this->spline.update();

			// rolling window to remove verticies of old.
			this->spline.removeVertices(vertex_position, 1);
			this->spline.update();
			vertex_position += this->scale;

		}
		else {
			this->m_verticies.push_back(sf::Vertex(sf::Vector2f(100.f + this->offset, VIEW_HEIGHT - 100.f - emgGasScaled), sf::Color::Red));

			sf::Vector2f v1(230.f + offset, VIEW_HEIGHT - 100.f - emgGasScaled);
			sw::Spline spline2{ 1, v1 };

			spline2.setThickness(10);
			// update always last after all modifications
			this->spline.addSplineToBack(spline2);
			this->spline.update();

			this->offset += this->scale;
		}
		this->window.clear();
		this->window.draw(quad_x);
		this->window.draw(quad_y);
		this->window.draw(spline);
		this->window.setView(view);

		//printf("Vertex Count: %d", spline.getVertexCount());

		this->window.display();


		// do stuff
		steady_clock::time_point now = steady_clock::now();
		std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(now - prev).count() << std::endl;
		this->prev = now;

		// delay until time to iterate again
		this->next += std::chrono::milliseconds(this->scale);
		std::this_thread::sleep_until(this->next);
		if (piTime+0.25 > this->TrialDuration)
		{
			this->window.close();
		}
	}
}