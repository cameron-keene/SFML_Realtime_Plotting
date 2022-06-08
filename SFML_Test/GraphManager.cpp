#include "GraphManager.h"

GraphManager::GraphManager(){ 
	// initilization of Graph
	this->TestTitle = TestSocket.GetTitle();
	this->TrialDuration = TestSocket.GetDuration();
	this->sineAmp = 2.5;
	this->sineFreq = 0.2;

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
	this->quad_x[0].color = sf::Color(192, 192, 192);
	// Bottom Left
	this->quad_x.append(sf::Vector2f(100.f, VIEW_HEIGHT - 100.f + 10.f));
	this->quad_x[1].color = sf::Color(192, 192, 192);
	// Bottom Right
	this->quad_x.append(sf::Vector2f(VIEW_WIDTH - 100.f, VIEW_HEIGHT - 100.f + 10.f));
	this->quad_x[2].color = sf::Color(192, 192, 192);
	// Top Right
	this->quad_x.append(sf::Vector2f(VIEW_WIDTH - 100.f, VIEW_HEIGHT - 100.f));
	this->quad_x[3].color = sf::Color(192, 192, 192);

	// y-axis:
	this->quad_y.setPrimitiveType(sf::Quads);
	// Top Left
	this->quad_y.append(sf::Vector2f(100.f, 100.f));
	this->quad_y[0].color = sf::Color(192, 192, 192);
	// Bottom Left 
	this->quad_y.append(sf::Vector2f(100.f, VIEW_HEIGHT - 100.f));
	this->quad_y[1].color = sf::Color(192, 192, 192);
	// Bottom Right 
	this->quad_y.append(sf::Vector2f(110.f, VIEW_HEIGHT - 100.f));
	this->quad_y[2].color = sf::Color(192, 192, 192);
	// Top Right
	this->quad_y.append(sf::Vector2f(110.f, 100.f));
	this->quad_y[3].color = sf::Color(192, 192, 192);

	this->spline.setThickness(10);
	this->spline.setColor(sf::Color(255, 30, 0));
	// update always last after all modifications
	this->spline.update();

	this->sineSpline.setThickness(10);
	this->sineSpline.setColor(sf::Color::Blue);
	// update always last after all modifications
	this->sineSpline.update();

	this->offset = 2;

	this->view.setCenter(VIEW_WIDTH / 2, VIEW_HEIGHT / 2);
}

void GraphManager::SlideGraph()
{
	// start scrolling
	this->offset += this->scale;
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


}
void GraphManager::UpdateSplineMVC(int emgGasScaled, int vertex_position)
{
	sf::Vector2f v1(215.f + this->offset, VIEW_HEIGHT - 100.f - emgGasScaled);
	sw::Spline spline2{ 1, v1 };

	spline2.setThickness(10);
	// update always last after all modifications
	this->spline.addSplineToBack(spline2);
	this->spline.update();

	// rolling window to remove verticies of old.
	this->spline.removeVertices(vertex_position, 1);
	this->spline.update();
}
void GraphManager::UpdateSineSpine(int sineValue, int vertex_position)
{
	// v1(width, height)
	sf::Vector2f v1(215.f + this->offset, VIEW_HEIGHT - 100.f - sineValue);
	sw::Spline spline2{ 1, v1 };

	spline2.setThickness(10);
	// update always last after all modifications
	this->sineSpline.addSplineToBack(spline2);
	this->sineSpline.update();

	// rolling window to remove verticies of old.
	this->sineSpline.removeVertices(vertex_position, 1);
	this->sineSpline.update();
}

void GraphManager::OpenWindow(string _type)
{
	while (this->window.isOpen()) 
	{
		this->read_result = this->TestSocket.Read();
		double piTime = read_result[0];
		double emgGAS = read_result[1];
		double emgTA = read_result[2];
		printf("piTime: %.15g - emgGAS: %.15g - emgTA: %.15g\n", piTime, emgGAS, emgTA);
		// cout << "TrialDuration: " << this->TrialDuration << endl;
		// new - taking old emg data and plotting it
		// data is on scale 0.000199 to 0.0258
		double emgGasScaled = emgGAS * 2500;

		int vertex_position = 0;
		double trackingPosition = 0.0;

		sf::Event event;
		while (this->window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				this->window.close();
		}
		if (_type == "MVC") 
		{
			if ((this->spline.getVertexCount() + 1) == VIEW_WIDTH / this->scale) {
				SlideGraph();
				UpdateSplineMVC(emgGasScaled, vertex_position);
				vertex_position += this->scale;
			}
			else {
				sf::Vector2f v1(230.f + offset, VIEW_HEIGHT - 100.f - emgGasScaled);
				sw::Spline spline2{ 1, v1 };

				spline2.setThickness(10);
				// update always last after all modifications
				this->spline.addSplineToBack(spline2);
				this->spline.update();

				this->offset += this->scale;
			}
		}
		else if (_type == "Dynamic")
		{
			//Sine Wave y(t) = A * sin(2 * PI * f * t + shift)
			if ((this->spline.getVertexCount() + 1) == VIEW_WIDTH / this->scale) {
				SlideGraph();
				UpdateSplineMVC(emgGasScaled, vertex_position);
				vertex_position += this->scale;
				trackingPosition = this->sineAmp * sin(2 * PI * this->offset) * 2500;

			}
			else {
				sf::Vector2f v1(230.f + offset, VIEW_HEIGHT - 100.f - emgGasScaled);
				sw::Spline spline2{ 1, v1 };

				spline2.setThickness(10);
				// update always last after all modifications
				this->spline.addSplineToBack(spline2);
				this->spline.update();

				this->offset += this->scale;
				trackingPosition = this->sineAmp * sin(2 * PI * this->offset) * 2500;

			}
			cout << "Sine Value: " << trackingPosition << endl;

		}

			this->window.clear(sf::Color(255, 255, 255));
			this->window.draw(quad_x);
			this->window.draw(quad_y);
			this->window.draw(spline);
			this->window.draw(sineSpline);
			this->window.setView(view);

			//printf("Vertex Count: %d", spline.getVertexCount());

			this->window.display();
		


		// do stuff
		steady_clock::time_point now = steady_clock::now();
		// std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(now - prev).count() << std::endl;
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