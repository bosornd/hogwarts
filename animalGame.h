#pragma once
#include <bangtal>
#include <iostream>

using namespace bangtal;
using namespace std;


class Cards
{
private:
	ObjectPtr card;
	int CardNum;
public:
	Cards() {
		CardNum = -1;
	}
	void setCard(string cardName, ScenePtr scene, int i, int j, int arr[]) {
		card = Object::create(cardName, scene, 90 + (arr[i] % 6) * 180, 490 - (arr[i] / 6) * 225);
		CardNum = j;
	}
	ObjectPtr getCard() {
		return card;
	}
	int getCardNum() {
		return CardNum;
	}
};

ScenePtr animalMain;
SoundPtr S_card;
SoundPtr S_success;
ObjectPtr hint;
ObjectPtr hintBtn;
int point; // 게임 점수
int resCheckCard;
int clickedObj;