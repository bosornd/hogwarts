#include <bangtal>
#include <iostream>

using namespace std;
using namespace bangtal;

extern void animalGame();
extern void astronomyGame();

int totalPoint;

ScenePtr mainScene;

//메인 함수
int main() {
	totalPoint = 0;
	// 불필요한 부분 삭제
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);

	//메인 장면
	mainScene = Scene::create("", "Images/Background.png");


	auto start = Object::create("images/start.png", mainScene, 500, 250);
	start->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		astronomyGame();
		return true;
		});
	
	
	//장면들
	startGame(mainScene);

	return 0;
}

