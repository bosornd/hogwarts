#include <bangtal>
#include <iostream>
#include "main.h"

using namespace bangtal;
using namespace std;

extern void MaraudersMap_main(Dorm dorm);

ScenePtr home;

int main() {

	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);

	home = Scene::create("home", "maze/startScene.jpg");
	auto homeOb = Object::create("maze/startScene.jpg", home);

	Dorm playerDorm = Dorm::G;


	homeOb->setOnMouseCallback([&](ObjectPtr, int, int, MouseAction) -> bool {
		MaraudersMap_main(playerDorm);

		//cout << "finish!" << endl;

		return true;
		});

	

	

	startGame(home);


	return 0;
}