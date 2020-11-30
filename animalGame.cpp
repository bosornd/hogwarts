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
int handedCard = -1;  // handedCard �� -1�̸� ���ο� ī�� ��ȣ ����, �ƴϸ� ���� ��
int prevClickedCard;
int flipedCard[18];
int flipedNum = 0;

int checkCard(int cardNum, int clickedCardNum) {

	for (int i = 0; i < 18; i++) {			// �̹� �������� ī�带 ���� ��� or ������ ���� ī�带 �� ���� ��� -> -4��ȯ
		if (flipedCard[i] == clickedCardNum) {

			showMessage("�̹� �������� ī���Դϴ�.");
			return -4;
		}
	}
	if (prevClickedCard != clickedCardNum) {
		if (handedCard == -1) {
			handedCard = cardNum;
			prevClickedCard = clickedCardNum;
			return -3;
		}
		else if (handedCard == cardNum) {				// ���� ī�带 �� ���
			handedCard = -1;
			flipedCard[flipedNum] = prevClickedCard;
			flipedCard[flipedNum + 1] = clickedCardNum;
			flipedNum += 2;
			goal++;
			return goal == 9 ? -2 : -1;					// ����� 9���� �¸�����
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


//���� �Լ�
void animalGame() {
	fill_n(flipedCard, 18, -1);
	animalMain = Scene::create("", "images/animalGame/animalScene.jpeg");

	// �Ҹ�
	S_card = Sound::create("sounds/animalGame/Card.mp3");
	S_success = Sound::create("sounds/animalGame/Success.mp3");



	auto timer = Timer::create(180.0f);
	timer->setOnTimerCallback([&](TimerPtr)->bool {
		showMessage("ī�� ã�� ����!!");
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
			// clickedObj = Ŭ�� �� ī���� ���� 
			for (int j = 0; j < 18; j++) {
				if (card[j].getCard() == object) {
					clickedObj = j;
				}
			}

			// ������ ī��� ����
			resCheckCard = checkCard(card[clickedObj].getCardNum(), clickedObj);   // | -1: ������ ���� | -2: ���� �¸�! | -3: ù��° ī�� | -4: ������ ���� or �̹� �������� |  |  ������ : PREV��			

			if (!(resCheckCard == -4)) {		// �������� ī�带 ���ų�, ���� �� ī�带 ����� ��찡 �ƴϸ� -> �Ҹ� �߻�, ī�� ������
				S_card->play(false);
				// ī�� ������ - num�� 9���� ũ�� ī���̸� ������ ī��׸�
				string openCardString = clickedObj < 9 ? "images/animalGame/animal" + to_string(card[clickedObj].getCardNum() + 1) + ".png" : "images/animalGame/animal" + to_string(card[clickedObj].getCardNum() + 1) + "_t.png";
				card[clickedObj].getCard()->setImage(openCardString);
			}

			if (resCheckCard > -1) {										// prev���� ���ƿ�����
				showMessage("���� �ٸ� �׸��Դϴ�.");
				timerFlip->setOnTimerCallback([&](TimerPtr t)->bool {       //������ ���� �ð�, �� ������ �� flip �ϸ� ���� �߻�.
					card[clickedObj].getCard()->setImage("images/animalGame/animal_back.png");
					t->set(0.8f);
					return true;
					});
				timerFlip->start();
				card[resCheckCard].getCard()->setImage("images/animalGame/animal_back.png");
			}
			else if (resCheckCard == -1) {
				showMessage("���߼̽��ϴ�!");
			}
			else if (resCheckCard == -2) {
				showMessage("�ź��� �������� ���迡 �հ��ϼ̽��ϴ�!");
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