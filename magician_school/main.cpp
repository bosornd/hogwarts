#include <bangtal>
#include <iostream>

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

ScenePtr mainScene, endScene;	// mainScene: home 화면(게임 선택할 수 있는), endScene: 성적 화면, 졸업화면
SoundPtr main_bgm;
int dorm;
bool P_F[gameMax] = { 0, }; // 게임 P/F

ScenePtr empty_scene;
TimerPtr result_timer;

void setHome(const int d) {

	dorm = d;
	mainScene->setImage("images/home" + to_string(dorm) + ".jpg");
	endScene->setImage("images/home" + to_string(dorm) + ".jpg");
	mainScene->enter();
	main_bgm->play(true);
}

void checkStage(int gameNum, bool PF = false) {
	P_F[gameNum] = PF;

	if (PF == true) { auto pass = Object::create("images/pass" + to_string(gameNum) + ".png", empty_scene, 0, -10); pass->setScale(1.1f); }
	else if (PF == false) { auto fail = Object::create("images/fail" + to_string(gameNum) + ".png", empty_scene, 0, -10); fail->setScale(1.1f); }

	empty_scene->enter();

	result_timer->start();

	result_timer->setOnTimerCallback([&](TimerPtr)->bool {
		if (stage == gameMax) endScene->enter();
		else {
			mainScene->enter();
			main_bgm->play(true);
		}

		result_timer->set(3.0f);
		return true;
		});
}

int main() {

	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);

	empty_scene = Scene::create("", "images/startScene1.png");
	result_timer = Timer::create(3.0f);

	stage = 0;
	string playerName;

	// 시작 장면
	auto startScene = Scene::create("", "images/startScene.png");
	auto startbutton = Object::create("images/startbutton.png", startScene);
	main_bgm = Sound::create("sounds/오프닝엔딩.mp3");
	main_bgm->play(true);

	startbutton->setOnMouseCallback([&playerName, startScene](ObjectPtr, int, int, MouseAction)-> bool {
		auto story = Object::create("images/story.jpg", startScene);
		story->setScale(1.2f);
		showMessage("호그와트 입학을 축하드립니다. 이제 기숙사를 정해볼까요?      (화면을 클릭해주세요.)");
		story->setOnMouseCallback([&playerName](ObjectPtr, int, int, MouseAction)->bool {
			main_bgm->stop();

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
		games[i]->setOnMouseCallback([=, &playerName, &gameComplete](ObjectPtr, int, int, MouseAction) -> bool {

			string ch_list[4] = { "harry", "luna", "malfoy", "newton" };
			playerName = ch_list[dorm];


			if (!gameComplete[i]) {
				stage++;
				gameComplete[i] = true;
				auto tutorial = Scene::create("", "images/back_tutorial" + to_string(i) + ".png");
				auto howtoplay = Object::create("images/tutorial" + to_string(i) + ".png", tutorial, -50, 0);
				auto button = Object::create("images/startbutton.png", tutorial); // 위치 설정 미완료
				tutorial->enter();
				button->setOnMouseCallback([=](ObjectPtr, int, int, MouseAction) -> bool {
					switch (i) {
					case 0: main_bgm->stop();  magicalHerb(playerName); break;
					case 1: main_bgm->stop(); astronomyGame(); break;
					case 2: main_bgm->stop(); animalGame(); break;
					case 3: main_bgm->stop(); MaraudersMap_main(); break;
					case 4: main_bgm->stop(); Quidditch(playerName); break;
					default: break;
					}
					return true;
					});

			}
			else if (gameComplete[i]) {
				showMessage("이미 플레이한 게임입니다.");
			}
			return true;
			});

	}



	endScene = Scene::create("", "images/home0.jpg");
	endScene->setOnEnterCallback([](ScenePtr)->bool {
		main_bgm->play(true);
		auto gradePaper = Object::create("images/paper" + to_string(dorm) + ".png", endScene, 400, 0);
		gradePaper->setScale(0.36f);

		static int PFcount = 0;
		ObjectPtr grade[gameMax]; // 게임별 성적을 보여주기 위한 Object
		for (int i = 0; i < gameMax; i++) {
			string s = (P_F[i]) ? "p" : "f";
			if (P_F[i]) PFcount++;
			grade[i] = Object::create("images/" + s + ".jpg", endScene, 800, 390 - i * 30); // 위치 설정 미완료.
			grade[i]->setScale(0.8f);
			if (i >= 3) grade[i]->locate(endScene, 800, 380 - i * 30);
			cout << endl << P_F[i];
		}

		gradePaper->setOnMouseCallback([=](ObjectPtr, int, int, MouseAction)-> bool {
			string s = (PFcount >= 3) ? "graduation" : "fail";
			for (int i = 0; i < gameMax; i++) {
				grade[i]->hide();
			}
			gradePaper->hide();
			endScene->setImage("images/" + s + ".jpg");
			endScene->setOnKeyboardCallback([](ScenePtr, KeyCode, bool) -> bool {
				endGame();
				return true;
				});

			return true;
			});

		/*
		auto timer = Timer::create(10.0f);
		timer->setOnTimerCallback([=](TimerPtr)->bool {
			string s = (PFcount >= 3) ? "graduation" : "fail";
			for (int i = 0; i < gameMax; i++) {
				grade[i]->hide();
			}
			gradePaper->hide();
			endScene->setImage("images/" + s + ".jpg");
			endScene->setOnKeyboardCallback([](ScenePtr, int, bool) -> bool {
				endGame();
				return true;
				});
			return true;
			});
		timer->start();
		*/
		// 10초 동안 성적 보여주고 자동으로 졸업 장면으로 넘어감. 졸업 장면에서 키보드 클릭 시 게임 종료

		return true;
		});
	// 모든 게임을 완료했을 시 성적장면으로 이동







	bangtal::startGame(startScene);

}