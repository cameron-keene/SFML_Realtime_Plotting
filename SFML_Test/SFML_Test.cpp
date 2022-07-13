#include <time.h>       /* time */
#include <iostream> /*cout, endl*/
#include <chrono>
#include <thread>

#include "SelbaWard/Line.hpp"
#include "SelbaWard/Spline.hpp"

#include "SocketManager.cpp"
#include "GraphManager.h"

#include "./UI/MainMenu.h";

#include "./TestManager/TestManager.h";

using namespace std;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds

int main()
{
	string userSelection = "intact";
	// main menu userSelection
	//MainMenu testingMenu;
	//testingMenu.OpenMenu(userSelection);
	//cout << "Result: " << userSelection << endl;

	TestManager testingManager(userSelection);
	for (int i = 0; i < 10; i++) {
		testingManager.RunTest();
		sleep_for(seconds(20));
	}


	//GraphManager TestGraph;
	//TestGraph.OpenWindow("Dynamic");
    return 0;
}