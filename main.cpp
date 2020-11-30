#include <bangtal>
#include <iostream>
//#include "main.h"

using namespace bangtal;
using namespace std;


extern void dormitory_main();
extern int magicalHerb(const string ch);
extern void animalGame();
extern int Quidditch(const string ch);
extern void astronomyGame();
extern void MaraudersMap_main();

const int gameMax = 5;
static int stage;

ScenePtr mainScene, endScene;			// home 화면(게임 선택할 수 있는)
int dorm;
bool P_F[gameMax] = { 0, }; // 게임 P/F

void setHome(const int d) {

	dorm = d;
	mainScene->setImage("images/home" + to_string(dorm) + ".jpg");
	mainScene->enter();
}

void checkStage() {
	if (stage == gameMax) endScene->enter();
	else mainScene->enter();
}

int main() {

	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);

	stage = 0;
	string playerName;


	// 시작 장면
	auto startScene = Scene::create("", "images/startScene.jpg");
	auto startbutton = Object::create("images/startbutton.png", startScene);

	startbutton->setOnMouseCallback([&playerName, startScene](ObjectPtr, int, int, MouseAction)-> bool {
		auto story = Object::create("images/story.jpg", startScene);
		story->setOnMouseCallback([&playerName](ObjectPtr, int, int, MouseAction)->bool {
			
			dormitory_main();
			
			return true;
			});
		
		return true;
		});
	

	// 게임 선택 home화면
	mainScene = Scene::create(" ", "images/home0.jpg");
	

	// 게임 선택 버튼, 위치 설정 미완료
	ObjectPtr games[gameMax];
	games[0] = Object::create("images/magicalHerb.png", mainScene, 100, 400);				// Magical herb - 0
	games[1] = Object::create("images/astronomy.png", mainScene, 500, 400);				// Astronomy Game - 1
	games[2] = Object::create("images/magicalCreature.png", mainScene, 900, 400);			// Magical Creature Game - 2
	games[3] = Object::create("images/maraudersMap.png", mainScene, 300, 150);			// Marauders Map Game - 3
	games[4] = Object::create("images/quidditch.png", mainScene, 700, 150);				// Quidditch Game - 4

	bool gameComplete[gameMax] = { 0, };	// 이미 실행한 게임인지 확인하기 위한 변수


	// 게임 버튼 클릭 시 게임 실행
	for (int i = 0; i < gameMax; i++) {
		games[i] ->setOnMouseCallback([=, &playerName, &gameComplete](ObjectPtr, int, int, MouseAction) -> bool {
			
			string ch_list[4] = { "harry", "luna", "malfoy", "newton" };
			playerName = ch_list[dorm];
			

			if (!gameComplete[i]) {
				stage++;
				gameComplete[i] = true;
				auto tutorial = Scene::create("", "images/tutorial" + to_string(i) + ".jpg");
				auto button = Object::create("images/startbutton.png", tutorial); // 위치 설정 미완료
				tutorial->enter();
				button->setOnMouseCallback([=](ObjectPtr, int, int, MouseAction) -> bool {
					switch (i) {
					case 0: magicalHerb(playerName); break;
					case 1: astronomyGame(); break;
					case 2: animalGame(); break;
					case 3: MaraudersMap_main(); break;
					case 4: Quidditch(playerName); break;
					default: break;
					}
					return true;
					});

			}
			else if (gameComplete[i]) {
				showMessage("already done");
			}
			return true;
			});

	}
	/*
	herb->setOnMouseCallback([=, &stage](ObjectPtr, int, int, MouseAction) -> bool {
		if (!gameComplete[0]) {
			stage++;
			auto tutorial = Scene::create("", "images/herb_tutorial.jpg");
			auto button = Object::create("images/startbutton.png", tutorial); // 위치 설정 미완료
			button->setOnMouseCallback([playerName](ObjectPtr, int, int, MouseAction) -> bool {
				magicalHerb(playerName);
				return true;
				});
			
		}
		else if (gameComplete[0]) {
			showMessage("already done");
		}
		return true;
		});

	astronomy->setOnMouseCallback([=, &stage](ObjectPtr, int, int, MouseAction) -> bool {
		if (!gameComplete[1]) {
			stage++;
			astronomyGame();
		}
		else if (gameComplete[1]) {
			showMessage("already done");
		}
		return true;
		});

	magicialCreature->setOnMouseCallback([=, &stage](ObjectPtr, int, int, MouseAction) -> bool {
		if (!gameComplete[2]) {
			stage++;
			animalGame();
		}
		else if (gameComplete[2]) {
			showMessage("already done");
		}
		return true;
		});

	maraudersMap->setOnMouseCallback([=, &stage](ObjectPtr, int, int, MouseAction) -> bool {
		if (!gameComplete[3]) {
			stage++;
			MaraudersMap_main(dorm); 
		}
		else if (gameComplete[3]) {
			showMessage("already done");
		}
		return true;
		});

	quidditch->setOnMouseCallback([=, &stage](ObjectPtr, int, int, MouseAction) -> bool {
		if (!gameComplete[4]) {
			stage++;
			Quidditch(playerName);
		}
		else if (gameComplete[4]) {
			showMessage("already done");
		}
		return true;
		});
		*/


	endScene = Scene::create("", "images/endScene.jpg");
	//mainScene->setOnEnterCallback([stage, endScene](ScenePtr)->bool {
	//	if (stage == gameMax) endScene->enter();
	//	return true;
	//	});
	// 모든 게임을 완료했을 시 성적장면으로 이동


	ObjectPtr grade[gameMax]; // 게임별 성적을 보여주기 위한 Object
	for (int i = 0; i < gameMax; i++) {
		string s = (P_F[i]) ? "pass" : "fail";
		grade[i] = Object::create("images/" + s + ".png", endScene); // 위치 설정 미완료.

		auto timer = Timer::create(60.0f);
		timer->setOnTimerCallback([](TimerPtr)->bool {
			auto graduation = Object::create("images/graduation.jpg", endScene);
			graduation->setOnMouseCallback([](ObjectPtr, int, int, MouseAction)->bool {
				endGame();
				return true;
				});
			return true;
			});
		timer->start();
		// 60초 동안 성적 보여주고 자동으로 졸업 장면으로 넘어감. 졸업 장면 클릭 시 게임 종료
	}
	
	
		

	startGame(startScene);

}