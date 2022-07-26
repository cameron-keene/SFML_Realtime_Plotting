#include "GraphManager.h"

GraphManager::GraphManager() {
	// initilization of Graph
	this->TestTitle = TestSocket.GetTitle();
	this->TrialDuration = TestSocket.GetDuration();
	this->sineAmp = 2.5;
	this->sineFreq = 0.5;
	this->gasEmgMax = 0.01;
	this->taEmgMax = 0.03;
	this->objectMax = 0.000000001;
	this->objectMin = 0.01;

	this->graphScale = 2500;
	this->scale = 12;

	// for normalization
	this->gas_normalization = 0.0261;
	this->ta_normalization = 0.0464;
	this->gas_threshold = 0.00252;
	this->ta_threshold = 0.00204;

	this->object_mass = 0.5;
	this->object_position = 0;
	this->object_velocity = 0;
	this->object_damping = 0.5;
	this->gravity = -20;
	this->object_start_position = -3;
	this->max_position = 10;

	this->next = chrono::steady_clock::now();
	this->prev_t = this->next - chrono::milliseconds(this->scale);

	sf::ContextSettings contextSettings; 
	contextSettings.antialiasingLevel = 16;
	this->window.create(sf::VideoMode(VIEW_WIDTH, VIEW_HEIGHT), this->TestTitle, sf::Style::Default, contextSettings);

	// create default view
	this->view = sf::View(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(VIEW_WIDTH, VIEW_HEIGHT));

	// x-axis:
	this->quad_x.setPrimitiveType(sf::Quads);
	// Bottom Left
	this->quad_x.append(sf::Vector2f(50.f, VIEW_HEIGHT - 50.f));
	this->quad_x[0].color = sf::Color(192, 192, 192);
	// Top Left
	this->quad_x.append(sf::Vector2f(50.f, VIEW_HEIGHT - 50.f + 10.f));
	this->quad_x[1].color = sf::Color(192, 192, 192);
	// Top Right
	this->quad_x.append(sf::Vector2f(VIEW_WIDTH - 25.f, VIEW_HEIGHT - 50.f + 10.f));
	this->quad_x[2].color = sf::Color(192, 192, 192);
	// Bottom Right
	this->quad_x.append(sf::Vector2f(VIEW_WIDTH - 25.f, VIEW_HEIGHT - 50.f));
	this->quad_x[3].color = sf::Color(192, 192, 192);

	// y-axis:
	this->quad_y.setPrimitiveType(sf::Quads);
	// Top Left
	this->quad_y.append(sf::Vector2f(50.f, 50.f));
	this->quad_y[0].color = sf::Color(192, 192, 192);
	// Bottom Left 
	this->quad_y.append(sf::Vector2f(50.f, VIEW_HEIGHT - 50.f));
	this->quad_y[1].color = sf::Color(192, 192, 192);
	// Bottom Right 
	this->quad_y.append(sf::Vector2f(50.f + 10.0f, VIEW_HEIGHT - 50.f));
	this->quad_y[2].color = sf::Color(192, 192, 192);
	// Top Right
	this->quad_y.append(sf::Vector2f(50.f + 10.f, 50.f));
	this->quad_y[3].color = sf::Color(192, 192, 192);

	// maxBar: 
	this->maxBar.setPrimitiveType(sf::Quads);
	this->maxBar.append(sf::Vector2f(100.f, VIEW_HEIGHT - 100.f - (this->gasEmgMax * this->graphScale)));
	this->maxBar[0].color = sf::Color(255, 0, 0);
	// Bottom Left
	this->maxBar.append(sf::Vector2f(100.f, VIEW_HEIGHT - 100.f + 8.f - (this->gasEmgMax * this->graphScale)));
	this->maxBar[1].color = sf::Color(255, 0, 0);
	// Bottom Right
	this->maxBar.append(sf::Vector2f(VIEW_WIDTH - 100.f, VIEW_HEIGHT - 100.f + 8.f - (this->gasEmgMax * this->graphScale)));
	this->maxBar[2].color = sf::Color(255, 0, 0);
	// Top Right
	this->maxBar.append(sf::Vector2f(VIEW_WIDTH - 100.f, VIEW_HEIGHT - 100.f - (this->gasEmgMax * this->graphScale)));
	this->maxBar[3].color = sf::Color(255, 0, 0);


	this->spline.setThickness(8);
	this->spline.setColor(sf::Color::Blue); 
	//this->spline.setBezierInterpolation(); // enable Bezier spline
	//this->spline.setInterpolationSteps(5u); // curvature resolution
	//this->spline.smoothHandles();
	// update always last after all modifications
	this->spline.update();

	this->sineSpline.setThickness(8);
	this->sineSpline.setColor(sf::Color(255, 30, 0));
	// update always last after all modifications
	//this->sineSpline.setBezierInterpolation();
	this->sineSpline.update();

	this->offset = 2;

	this->view.setCenter(VIEW_WIDTH / 2, VIEW_HEIGHT / 2);
	

	// Load text information
	if (!this->font.loadFromFile("arial.ttf")) {
		cout << "file can not be loaded" << endl;
		return;
	}
	
	this->text.setFont(font);

	// set the string to display
	this->text.setString("-1");

	// set the character size
	this->text.setCharacterSize(24); // in pixels, not points!

	// set the color
	this->text.setFillColor(sf::Color::Black);
	
	// set the text size
	this->text.setCharacterSize(25);

	this->text.setPosition(VIEW_WIDTH / 2 - 50.0f, VIEW_HEIGHT - 40.f);

	this->t0 = steady_clock::now();
	this->prev_t = this->t0;
}

void GraphManager::SlideGraph()
{
	// start scrolling
	this->offset += this->scale;
	this->view.setCenter(-(VIEW_HEIGHT / 2) - 300 + this->offset, VIEW_HEIGHT / 2);

	// Top Right
	this->quad_x[0].position = sf::Vector2f(this->offset + 75.f, VIEW_HEIGHT - 50.f);
	// Bottom Right
	this->quad_x[1].position = sf::Vector2f(this->offset + 75.f, VIEW_HEIGHT - 50.f + 10.f);
	// Bottom Left
	this->quad_x[2].position = sf::Vector2f(-VIEW_WIDTH + 50.f + 120.0f + this->offset, VIEW_HEIGHT - 50.f + 10.f);
	// Top Left
	this->quad_x[3].position = sf::Vector2f(-VIEW_WIDTH + 50.f + 120.0f + this->offset, VIEW_HEIGHT - 50.f);

	// Top Left
	this->quad_y[0].position = sf::Vector2f(-VIEW_WIDTH + 50.f + 120.0f + this->offset, 50.f);
	// Bottom Left 
	this->quad_y[1].position = sf::Vector2f(-VIEW_WIDTH + 50.f + 120.0f + this->offset, VIEW_HEIGHT - 50.f);
	// Bottom Right 
	this->quad_y[2].position = sf::Vector2f(-VIEW_WIDTH + 50.f + 130.0f + this->offset, VIEW_HEIGHT - 50.f);
	// Top Right
	this->quad_y[3].position = sf::Vector2f(-VIEW_WIDTH + 50.f + 130.0f + this->offset, 50.f);

	// Move Text
	//this->text.setPosition(500.f + this->offset, VIEW_HEIGHT - 40.f);

}

void GraphManager::SlideMaxEmg()
{
	// Top Right
	this->maxBar[0].position = sf::Vector2f(this->offset + 75.f, VIEW_HEIGHT - 100.f - (this->gasEmgMax * this->graphScale));
	// Bottom Right
	this->maxBar[1].position = sf::Vector2f(this->offset + 75.f, VIEW_HEIGHT - 100.f + 10.f - (this->gasEmgMax * this->graphScale));
	// Bottom Left
	this->maxBar[2].position = sf::Vector2f(-VIEW_WIDTH + 60.f + 120.f + this->offset, VIEW_HEIGHT - 100.f + 10.f - (this->gasEmgMax * this->graphScale));
	// Top Left
	this->maxBar[3].position = sf::Vector2f(-VIEW_WIDTH + 60.f + 120.f + this->offset, VIEW_HEIGHT - 100.f - (this->gasEmgMax * this->graphScale));
}

void GraphManager::UpdateMaxEMG()
{
	this->maxBar[0].position = sf::Vector2f(60.f, VIEW_HEIGHT - 100.f - (this->gasEmgMax * this->graphScale));
	// Bottom Left
	this->maxBar[1].position = sf::Vector2f(60.f, VIEW_HEIGHT - 100.f + 10.f - (this->gasEmgMax * this->graphScale));
	// Bottom Right
	this->maxBar[2].position = sf::Vector2f(VIEW_WIDTH - 25.f, VIEW_HEIGHT - 100.f + 10.f - (this->gasEmgMax * this->graphScale));
	// Top Right
	this->maxBar[3].position = sf::Vector2f(VIEW_WIDTH - 25.f, VIEW_HEIGHT - 100.f - (this->gasEmgMax * this->graphScale));
}

void GraphManager::UpdateSplineMVC(int emgGasScaled, int vertex_position)
{
	sf::Vector2f v1(60.0f + this->offset, VIEW_HEIGHT - 100.f - emgGasScaled);
	sw::Spline spline2{ 1, v1 };

	spline2.setThickness(10);
	// update always last after all modifications
	this->spline.addSplineToBack(spline2);
	this->spline.update();

	// rolling window to remove verticies of old.
	this->spline.removeVertices(vertex_position, 1);
	this->spline.update();
}

void GraphManager::UpdateSplineMVCStatic(double emgGasScaled)
{
	sf::Vector2f v1(60.f + offset, VIEW_HEIGHT - 100.f - emgGasScaled);
	sw::Spline spline2{ 1, v1 };

	spline2.setThickness(10);
	// update always last after all modifications
	this->spline.addSplineToBack(spline2);
	this->spline.update();

	this->offset += this->scale;
}

void GraphManager::UpdateSplineDynamic(int emgGasScaled, int vertex_position)
{
	sf::Vector2f v1(60.f + this->offset, VIEW_HEIGHT / 2 - emgGasScaled);
	sw::Spline spline2{ 1, v1 }; 

	spline2.setThickness(10);
	// update always last after all modifications
	this->spline.addSplineToBack(spline2);
	this->spline.update();

	// rolling window to remove verticies of old.
	//this->spline.removeVertices(vertex_position, 1);
	this->spline.removeVertex(0);
	this->spline.update();
}
void GraphManager::UpdateSplineStatic(double emgGasScaled)
{
	sf::Vector2f v1(70.f + this->offset, VIEW_HEIGHT / 2 - emgGasScaled);
	sw::Spline spline2{ 1, v1 };

	spline2.setThickness(10);
	// update always last after all modifications
	this->spline.addSplineToBack(spline2);
	this->spline.update();

	this->offset += this->scale;
}
void GraphManager::UpdateSineSplineStatic(int trackingPosition)
{
	// v1(width, height)
	sf::Vector2f v1(58.0f + this->offset, (VIEW_HEIGHT / 2) - trackingPosition); // + 115.f + this->offset
	sw::Spline spline2{ 1, v1 };
	spline2.setThickness(10);

	// update always last after all modifications
	this->sineSpline.addSplineToBack(spline2);
	this->sineSpline.update();

}
void GraphManager::UpdateSineSpline(int trackingPosition, int vertex_position)
{
	// v1(width, height)
	sf::Vector2f v1(58.f + this->offset, (VIEW_HEIGHT / 2) - trackingPosition); //+ 115.f 
	sw::Spline spline2{ 1, v1 };
	spline2.setThickness(10);

	// update always last after all modifications
	this->sineSpline.addSplineToBack(spline2);
	this->sineSpline.update();

	// rolling window to remove verticies of old.
	//cout << "vertex_position: " << vertex_position << endl;

	this->sineSpline.removeVertex(0);
	//this->sineSpline.removeVertices(vertex_position, 1);
	this->sineSpline.update();
}


void GraphManager::OpenWindow(string _type)
{
	//this->window.setFramerateLimit(100);
	while (this->window.isOpen())
	{
		this->t_now = steady_clock::now();
		this->t_start = steady_clock::now();
		this->read_result = this->TestSocket.Read();
		if ((duration_cast<duration<double>>(steady_clock::now() - this->t_start)).count() > 0.025) {
			cout << "Delay!!! - need to interpolate" << endl;
			//raise(SIGABRT);
			 //try to set the current values to the previous values 
			//this->read_result = 
		}
		//this->t_end = steady_clock::now();

		double piTime = read_result[0];
		double emgGAS = read_result[1];
		double emgTA = read_result[2];
		if (emgGAS > this->gasEmgMax)
		{
			// set max GAS EMG value
			this->gasEmgMax = emgGAS;
			//cout << "Set New Max" << endl;
		}
		if (emgTA > this->taEmgMax)
		{
			// set max TA EMG value
			this->taEmgMax = emgTA;
		}
		// printf("piTime: %.15g - emgGAS: %.15g - emgTA: %.15g\n", piTime, emgGAS, emgTA);
		// cout << "TrialDuration: " << this->TrialDuration << endl;
		// new - taking old emg data and plotting it
		// data is on scale 0.000199 to 0.0258
		double emgGasScaled = emgGAS * this->graphScale;



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
			if ((this->spline.getVertexCount() + 1) >= ((VIEW_WIDTH - 100) / this->scale)) {
				SlideGraph();
				SlideMaxEmg();
				UpdateSplineMVC(emgGasScaled, vertex_position);
				vertex_position += this->scale;
			}
			else {
				UpdateMaxEMG();
				// UpdateSplineMVCStatic
				UpdateSplineMVCStatic(emgGasScaled);
			}
		}
		else if (_type == "Dynamic")
		{
			// need to make new scaled value for dynamic tracking 
			// step 1: Normalization
			double gas_norm = emgGAS / this->gas_normalization * 100;
			double ta_norm = emgTA / this->ta_normalization * 100;
			//cout << "gas_norm: " << gas_norm << " - ta_norm: " << ta_norm << endl;
			// step 2: check for if below threshold
			if (emgGAS < this->gas_threshold)
				// if below threshold then set to 0
			{
				gas_norm = 0;
			}
			if (emgTA < this->ta_threshold)
			{
				ta_norm = 0;
			}


			// step 3: calc previous time
			double calc_t = std::chrono::duration<double>(this->t_now - this->prev_t).count();
			//cout << "calc_t: " << calc_t << endl;

			// step 4: get combined emg value
			double emg_val = (ta_norm - gas_norm) / this->object_mass;
			//cout << "emg_val: " << emg_val << endl;
			// 
			// step 5: calc C1
			double C1 = this->object_velocity - emg_val;
			//cout << "C1: " << C1 << endl;
			
			// step 6: get C2 (inverse of C1)
			double C2 = -C1;

			// step 7: calculate object velocity
			this->object_velocity = C1 * exp(-this->object_mass / this->object_damping * calc_t) + emg_val;
			//cout << "object_velocity: " << object_velocity << endl;

			// step 8: calculate object position
			this->object_position = this->object_position + C1 + C2 * exp(-this->object_mass / this->object_damping * calc_t) + calc_t * emg_val;

			// step 9: check object position twice
			if (this->object_position > this->max_position)
			{
				this->object_position = this->max_position;
				this->object_velocity = 0;
			}
			else if (this->object_position < -this->max_position)
			{
				this->object_position = -this->max_position;
				this->object_velocity = 0;
			}
			//object_position = object_position * (double)10;
			cout << setprecision(15) << fixed;
			//object_position *= 100;
			//cout << "object_position: " << object_position*50 << endl;

			// step 10: check if dynamic line then convert tracking position. 
			// we know that it is a dynamic line because we are inside of the dynamic if clause.
			// tracking position only calculated to be returned in datalog, not used for plotting.
			// we are plotting the object_position
			// tracking_position = sine wave
			double tracking_position = this->sineAmp * sin(2 * PI * this->sineFreq * std::chrono::duration<double>(this->t_now - this->t0).count());
			tracking_position *= 100;
			//cout << "tracking_position: " << tracking_position << endl;
			
			// step 11: delay(skip)

			// step 12: update the time.
			this->prev_t = t_now;

			// step 13: plot to screen, same as return 
			// Dynamic Plotting
			if ((this->spline.getVertexCount() + 1) >= ((VIEW_WIDTH - 100)/ this->scale)) {
				SlideGraph();
				UpdateSplineDynamic(object_position * 44, vertex_position);
				trackingPosition = sin(this->offset * 0.003) * 100;
				UpdateSineSpline(tracking_position, vertex_position);
				vertex_position += this->scale;
				//trackingPosition = this->sineAmp * sin(2 * PI * this->offset) * 25000;


			} // Stationary Plotting
			else {
				UpdateSplineStatic(object_position * 44);
				//trackingPosition = this->sineAmp * sin(2 * PI * this->offset) * 25000;
				trackingPosition = sin(this->offset * 0.003) * 100;
				UpdateSineSplineStatic(tracking_position);

			}
			//cout << "Sine Value: " << trackingPosition << endl;

		}
		else if (_type == "Gravity_Dynamic") 
		{
			// need to make new scaled value for dynamic tracking 
			// step 1: Normalization
			double gas_norm = emgGAS / this->gas_normalization * 100;
			double ta_norm = emgTA / this->ta_normalization * 100;
			// cout << "gas_norm: " << gas_norm << " - ta_norm: " << ta_norm << endl;
			// step 2: check for if below threshold
			if (emgGAS < this->gas_threshold)
				// if below threshold then set to 0
			{
				gas_norm = 0;
			}
			if (emgTA < this->ta_threshold)
			{
				ta_norm = 0;
			}


			// step 3: calc previous time
			double calc_t = std::chrono::duration<double>(this->t_now - this->prev_t).count();
			// cout << "calc_t: " << calc_t << endl;

			// step 4: get combined emg value
			double emg_val = (this->gravity * this->object_damping + ta_norm) / this->object_mass;
			// cout << "emg_val: " << emg_val << endl;
			// step 5: calc C1
			double C1 = this->object_velocity - emg_val;
			// cout << "C1: " << C1 << endl;

			// step 6: calc object velocity
			this->object_velocity = C1 * exp(-this->object_mass / this->object_damping * calc_t) + (this->gravity * this->object_damping + ta_norm) / this->object_mass;
			
			// step 7: calc object position
			this->object_position = this->object_position + C1 + -C1 * exp(-this->object_mass / this->object_damping * calc_t) + calc_t * (this->gravity * this->object_damping + ta_norm) / this->object_mass;

			// step 8: check limit
			if (this->object_position < this->object_start_position) {
				// cout << "hitting position limit" << endl;
				this->object_position = this->object_start_position;
				this->object_velocity = 0;
			}

			// step 9: check max position
			if (this->object_position > this->max_position) {
				this->object_position = this->max_position;
				this->object_velocity = 0;
			}

			// cout << "this->object_velocity: " << this->object_velocity << endl;
			// cout << "this->object_position: " << this->object_position << endl;

			// step 10: Sine Wave
			double tracking_position = this->sineAmp * sin(2 * PI * this->sineFreq * std::chrono::duration<double>(this->t_now - this->t0).count());
			tracking_position *= 100;

			// step 11: plotting
			// Dynamic Plotting, sliding window
			if ((this->spline.getVertexCount() + 1) >= ((VIEW_WIDTH - 100) / this->scale)) {
				//cout << "Sliding" << endl;
				//cout << "this->spline.getVertexCount() + 1: " << this->spline.getVertexCount() + 1 << endl;
				cout << "VIEW_WIDTH / this->scale: " << (VIEW_WIDTH - 100) / this->scale << endl;
				SlideGraph();
				UpdateSplineDynamic(this->object_position * 44, vertex_position);
				trackingPosition = sin(this->offset * 0.003) * 100;
				UpdateSineSpline(tracking_position, vertex_position);
				vertex_position += this->scale;
				//trackingPosition = this->sineAmp * sin(2 * PI * this->offset) * 25000;


			} // Stationary Plotting
			else {
				UpdateSplineStatic(this->object_position * 44);
				//trackingPosition = this->sineAmp * sin(2 * PI * this->offset) * 25000;
				trackingPosition = sin(this->offset * 0.003) * 100;
				UpdateSineSplineStatic(tracking_position);

			}

			// step 12: update the time.
			this->prev_t = t_now;
		} 

		this->window.clear(sf::Color(255, 255, 255));
		this->window.draw(this->quad_x);
		this->window.draw(this->quad_y);
		this->window.draw(this->spline);
		this->window.draw(this->sineSpline);
		this->window.draw(this->text);
		if (_type == "MVC") {
			this->window.draw(this->maxBar);
		}
		this->window.setView(view);

		//printf("Vertex Count: %d", spline.getVertexCount());

		this->window.display();



		// do stuff
		steady_clock::time_point now = steady_clock::now();
		// std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(now - prev).count() << std::endl;
		//this->prev = now;

		// delay until time to iterate again
		this->next += std::chrono::milliseconds(this->scale);
		std::this_thread::sleep_until(this->next);
		if (piTime + 0.25 > this->TrialDuration)
		{
			this->window.close();
		}
	}
}