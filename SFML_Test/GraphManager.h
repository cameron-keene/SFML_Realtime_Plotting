#pragma once
#include <SFML/Graphics.hpp>
#include "SocketManager.h"
#include "SelbaWard/Line.hpp"
#include "SelbaWard/Spline.hpp"
#include <chrono>
#include <thread>

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
	int scale;
	steady_clock::time_point next;
	steady_clock::time_point prev;

	sf::RenderWindow window;
	sf::View view;
	
	sf::VertexArray x_axis;
	sf::VertexArray y_axis;
	sf::VertexArray quad_x;
	sf::VertexArray quad_y;
	
	sw::Spline spline{};

	vector<sf::Vertex> m_verticies;

	uint16_t offset;

	vector<double> read_result;

	// functions
	GraphManager();
	void OpenWindow();
};