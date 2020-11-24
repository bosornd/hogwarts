#include <bangtal>
#include <iostream>

using namespace bangtal;
using namespace std;

const int num_dormitory = 4;
string dormitory[num_dormitory] = { "그리핀도르", "레번클로", "슬리데린", "후플푸프" };

const int num_questions = 7;
string questions[num_questions] = { "선생님", "영화", "인사", "장소", "중요", "친구", "활동" };

int point[4] = { 0, 0, 0, 0 };

int main_c()
{	
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
	
	ScenePtr scene[num_questions];
	ObjectPtr answers[num_questions][num_dormitory];

	for (int q = 0; q < num_questions; q++) {
		scene[q] = Scene::create("Question" + to_string(q + 1), "image/background.jpg");
		
		for (int d = 0; d < num_dormitory; d++) {
			answers[q][d] = Object::create("image/" + questions[q] + "-" + dormitory[d] + ".png", scene[q], 500, 500 - 100 * d);
		}
	}

	startGame(scene[0]);
}