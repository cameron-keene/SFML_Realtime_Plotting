#pragma once
#include <SFML/Graphics.hpp>
#include "SocketManager.h"
#include "SelbaWard/Line.hpp"
#include "SelbaWard/Spline.hpp"
#include <chrono>
#include <thread>
#include <math.h>

#define PI 3.14159265
//#include <string>

static const float VIEW_HEIGHT = 1080.0f;
static const float VIEW_WIDTH = 1920.0f;
using namespace std;
using namespace std::chrono;


class GraphManager
{
public:
	SocketManager TestSocket;
	string TestTitle;
	int TrialDuration, scale, graphScale;
	double sineAmp, sineFreq, gasEmgMax, taEmgMax;
	steady_clock::time_point next;
	steady_clock::time_point prev;

	sf::RenderWindow window;
	sf::View view;
	
	sf::VertexArray x_axis;
	sf::VertexArray y_axis;
	sf::VertexArray quad_x;
	sf::VertexArray quad_y;
	sf::VertexArray maxBar;
	
	sw::Spline spline{};
	sw::Spline sineSpline{};

	uint16_t offset;

	vector<double> read_result;

	// functions
	GraphManager();
	void SlideMaxEmg();
	void UpdateMaxEMG();
	void SlideGraph();
	void UpdateSplineMVCStatic(double emgGasScaled);
	void UpdateSplineMVC(int emgGasScaled, int vertex_position);
	void UpdateSineSpine(int sineValue, int vertex_position); 
	void UpdateSineSpineStatic(int trackingPosition);
	void OpenWindow(string _type);

};