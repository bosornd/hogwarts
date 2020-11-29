#include <bangtal>
#include<iostream>
#include <time.h>
#include <stdio.h>
#define SIZE 5
using namespace bangtal;
using namespace std;


int main() 
{
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	ObjectPtr herbs[8], question[8]; 
	int herbs_count[8] = {0,0,0,0,0,0,0,0};
	int answer[8][8] = {{0,1,0,0,0,1,2,2}, {0,0,1,2,1,1,0,0}, {0,3,0,0,0,0,3,0}, {2,0,1,0,1,0,0,1}, {0,0,0,1,3,1,1,0}, {1,1,1,1,1,1,1,2}, {0,0,1,1,0,3,0,0}, {2,2,1,0,0,0,0,1}};
	int herb_x = 90, herb_y = 550;

	auto BGM = createSound("Sounds/magicpotion.mp3");
	playSound(BGM, true);
	auto magicpotion = Scene::create("MagicalHerb", "images/magicpotion.png");
	auto bg2 = Scene::create("MagicalHerb", "images/bg2.png");
	auto Student = Object::create("images/Potter.png", magicpotion, 840, 70);
	Student->setScale(2.0f);
	auto timer = Timer::create(0.1f);
	auto adding = createSound("Sounds/plus.mp3");
	auto end1 = createSound("Sounds/end1.mp3");
	auto end2 = createSound("Sounds/end2.mp3");

	for (int i = 0; i < 8; i++) {
		string filename1 = "images/h" + to_string(i + 1) + ".png";
		herbs[i] = Object::create(filename1, magicpotion, herb_x, herb_y);
		herb_x += 140;
	};//샐러리, 코카인, 크랜베리, 인삼, 라벤더, 맨드레이크, 민트, 로즈마리

	herbs[0]->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction)->bool {
		herbs_count[0] += 1;
		playSound(adding);
		return true;
		});
	herbs[1]->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction)->bool {
		herbs_count[1] += 1;
		playSound(adding);
		return true;
		});
	herbs[2]->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction)->bool {
		herbs_count[2] += 1;
		playSound(adding);
		return true;
		});
	herbs[3]->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction)->bool {
		herbs_count[3] += 1;
		playSound(adding);
		return true;
		});
	herbs[4]->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction)->bool {
		herbs_count[5] += 1;
		playSound(adding);
		return true;
		});
	herbs[5]->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction)->bool {
		herbs_count[5] += 1;
		playSound(adding);
		return true;
		});
	herbs[6]->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction)->bool {
		herbs_count[6] += 1;
		playSound(adding);
		return true;
		});
	herbs[7]->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction)->bool {
		herbs_count[7] += 1;
		playSound(adding);
		return true;
		});

	auto preques = Object::create("images/q0.png", bg2, 20, 20);
	auto bowl = Object::create("images/bowl.png", magicpotion, 490, 70);
	bowl->setScale(1.5f);
	auto result = Object::create("images/wrong.png", bg2, 300, 200);
	result->hide();
	auto pass = Object::create("images/pass.png", bg2, 20, 20);
	pass->hide();
	auto fail = Object::create("images/fail.png", bg2, 20, 20);
	fail->hide();
	int wrong_num = 0, num=0, total =0, p=0, f=0;

	for (int i = 0; i < 8; i++) {
		string filename2 = "images/q" + to_string(i + 1) + ".png";
		question[i] = Object::create(filename2, bg2, 20, 20);
		question[i]->hide();
		question[i]->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction)->bool {
			question[num]->hide();
			magicpotion->enter();
				return true;
				});
	}


	preques->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction)->bool {
		preques->hide();
		wrong_num = 0;
		num = rand() % 8;
		question[num]->show();
		for (int k = 0; k < 8; k++) {
			printf("%d",answer[num][k]);
		};
		return 0;
		});

	bowl->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction)->bool {
		
		for (int t = 0; t < 8; t++) {
			printf("%d", herbs_count[t]);
		};

		for (int i = 0; i < 8; i++) {
			if (answer[num][i] != herbs_count[i])
			{
				wrong_num += 1;
			}
		};
		total += 1;

		if (wrong_num > 0) {
			bg2->enter();
			result->setImage("images/wrong.png");
			f += 1;
			question[num]->hide();
			result->show();
			return 0;
		}
		else if (wrong_num == 0) {
			bg2->enter();
			result->setImage("images/correct.png");
			p += 1;
			question[num]->hide();
			result->show();
			return 0;
		};
		});

	result->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction)->bool {
		result->hide();

		for (int j = 0; j < 8; j++) {
			herbs_count[j] = 0;
		};

		if (total == 5) {
			bg2->enter();
			if (pass > fail) {
				playSound(end1);
				pass->show();
			}
			else {
				playSound(end2);
				fail->show();
			};
		}
		else {
			wrong_num = 0;
			num = rand() % 8;
			question[num]->show();
		};

		return 0;
		});

	startGame(bg2);
	return 0;
}