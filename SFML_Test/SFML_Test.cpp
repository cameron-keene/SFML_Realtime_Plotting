#include <time.h>       /* time */
#include <iostream> /*cout, endl*/

#include "SelbaWard/Line.hpp"
#include "SelbaWard/Spline.hpp"

#include "SocketManager.cpp"
#include "GraphManager.h"

#include "./UI/MainMenu.h";

using namespace std;

int main()
{
	string test = "NULL";
	// main menu test
	MainMenu testingMenu;
	testingMenu.OpenMenu(test);
	cout << "Result: " << test << endl;
	//GraphManager TestGraph;
	//TestGraph.OpenWindow("Dynamic");
    return 0;
}