#include <bangtal>
#include <iostream>
#include <ctime>


using namespace bangtal;
using namespace std;

//extern ScenePtr mainScene;
extern void checkStage();

ScenePtr animalMain;
SoundPtr S_card;
SoundPtr S_success;
ObjectPtr hint;
ObjectPtr hintBtn;
int resCheckCard;
int clickedObj;


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
}card[18];

int goal = 0;
int handedCard = -1;  // handedCard 가 -1이면 새로운 카드 번호 저장, 아니면 둘을 비교
int prevClickedCard;
int flipedCard[18];
int flipedNum = 0;

int checkCard(int cardNum, int clickedCardNum) {

	for (int i = 0; i < 18; i++) {			// 이미 뒤집어진 카드를 누른 경우 or 직전에 누른 카드를 또 누른 경우 -> -4반환
		if (flipedCard[i] == clickedCardNum) {

			showMessage("이미 뒤집어진 카드입니다.");
			return -4;
		}
	}
	if (prevClickedCard != clickedCardNum) {
		if (handedCard == -1) {
			handedCard = cardNum;
			prevClickedCard = clickedCardNum;
			return -3;
		}
		else if (handedCard == cardNum) {				// 같은 카드를 고른 경우
			handedCard = -1;
			flipedCard[flipedNum] = prevClickedCard;
			flipedCard[flipedNum + 1] = clickedCardNum;
			flipedNum += 2;
			goal++;
			return goal == 9 ? -2 : -1;					// 맞춘게 9개면 승리선언
		}
		else if (handedCard != cardNum) {
			handedCard = -1;
			int tmp = prevClickedCard;
			prevClickedCard = -1;
			return tmp;
		}
	}
	else {
		return -4;
	}
};


//메인 함수
void animalGame() {
	fill_n(flipedCard, 18, -1);
	animalMain = Scene::create("", "images/animalGame/animalScene.jpeg");

	// 소리
	S_card = Sound::create("sounds/animalGame/Card.mp3");
	S_success = Sound::create("sounds/animalGame/Success.mp3");



	auto timer = Timer::create(180.0f);
	timer->setOnTimerCallback([&](TimerPtr)->bool {
		showMessage("카드 찾기 실패!!");
		return true;
		});
	auto timerFlip = Timer::create(0.8f);

	timer->start();
	showTimer(timer);


	int Shuffle[18];
	for (int i = 0; i < 18; i++) {
		Shuffle[i] = i;
	}
	for (int i = 0; i < 80; i++) {
		int S1 = rand() % 18;
		int S2 = rand() % 18;
		int tmp = Shuffle[S1];
		Shuffle[S1] = Shuffle[S2];
		Shuffle[S2] = tmp;
	}


	for (int i = 0; i < 9; i++) {
		card[i].setCard("images/animalGame/animal_back.png", animalMain, i, i, Shuffle);
		card[i + 9].setCard("images/animalGame/animal_back.png", animalMain, i + 9, i, Shuffle);
	}

	//hint
	hintBtn = Object::create("images/animalGame/hintBtn.png", animalMain, 1170, 590);
	hint = Object::create("images/animalGame/animal_hint.png", animalMain, 50, 30, false);
	hintBtn->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		hint->show();
		return true;
		});
	hint->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		hint->hide();
		return true;
		});

	for (int i = 0; i < 18; i++) {
		card[i].getCard()->setOnMouseCallback([=](ObjectPtr object, int x, int y, MouseAction)->bool {
			// clickedObj = 클릭 된 카드의 숫자 
			for (int j = 0; j < 18; j++) {
				if (card[j].getCard() == object) {
					clickedObj = j;
				}
			}

			// 뒤집은 카드들 연산
			resCheckCard = checkCard(card[clickedObj].getCardNum(), clickedObj);   // | -1: 뒤집기 성공 | -2: 게임 승리! | -3: 첫번째 카드 | -4: 직전에 누른 or 이미 뒤집어진 |  |  나머지 : PREV값			

			if (!(resCheckCard == -4)) {		// 뒤집어진 카드를 고르거나, 전에 고른 카드를 골랐을 경우가 아니면 -> 소리 발생, 카드 뒤집기
				S_card->play(false);
				// 카드 뒤집기 - num이 9보다 크면 카드이름 작으면 카드그림
				string openCardString = clickedObj < 9 ? "images/animalGame/animal" + to_string(card[clickedObj].getCardNum() + 1) + ".png" : "images/animalGame/animal" + to_string(card[clickedObj].getCardNum() + 1) + "_t.png";
				card[clickedObj].getCard()->setImage(openCardString);
			}

			if (resCheckCard > -1) {										// prev값이 돌아왔을떄
				showMessage("둘이 다른 그림입니다.");
				timerFlip->setOnTimerCallback([&](TimerPtr t)->bool {       //뒤집기 위한 시간, 두 사진을 다 flip 하면 오류 발생.
					card[clickedObj].getCard()->setImage("images/animalGame/animal_back.png");
					t->set(0.8f);
					return true;
					});
				timerFlip->start();
				card[resCheckCard].getCard()->setImage("images/animalGame/animal_back.png");
			}
			else if (resCheckCard == -1) {
				showMessage("맞추셨습니다!");
			}
			else if (resCheckCard == -2) {
				showMessage("신비한 마법동물 시험에 합격하셨습니다!");
				S_success->play(false);
				timer->stop();
				checkStage();
				//mainScene->enter();
			}

			return true;
			}
		);
	}
	enterScene(animalMain->ID());

}