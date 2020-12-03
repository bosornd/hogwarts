#include <bangtal>

#include <iostream>

#include <ctime>





using namespace bangtal;

using namespace std;



//extern ScenePtr mainScene;

extern void checkStage(int n, bool pf);



ScenePtr animalMain;

SoundPtr S_card;

SoundPtr S_success;

SoundPtr animal_bgm;
<<<<<<< HEAD

SoundPtr S_right;

SoundPtr S_wrong;

=======
SoundPtr S_right;
SoundPtr S_wrong;
>>>>>>> ea2b83450ca2505182cd2c74ce866d9eb38a40b0
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



TimerPtr timerFlip[18];



//���� �Լ�

void animalGame() {

	fill_n(flipedCard, 18, -1);

	animalMain = Scene::create("", "images/animalGame/animalScene.jpeg");



	// �Ҹ�

	S_card = Sound::create("sounds/animalGame/Card.mp3");

	S_success = Sound::create("sounds/animalGame/Success.mp3");
<<<<<<< HEAD

	S_right = Sound::create("sounds/animalGame/Right.mp3");

	S_wrong = Sound::create("sounds/animalGame/Wrong.mp3");

	animal_bgm = Sound::create("sounds/animalGame/Background.mp3");

=======
	S_right = Sound::create("sounds/animalGame/Right.mp3");
	S_wrong = Sound::create("sounds/animalGame/Wrong.mp3");
	animal_bgm = Sound::create("sounds/animalGame/Background.mp3");
>>>>>>> ea2b83450ca2505182cd2c74ce866d9eb38a40b0
	animal_bgm->play(true);





	auto timer = Timer::create(180.0f);

	timer->setOnTimerCallback([&](TimerPtr)->bool {

		showMessage("ī�� ã�� ����!!");

		animal_bgm->stop();

		checkStage(2, false);

		return true;

		});



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

		timerFlip[i] = Timer::create(0.8f);

		timerFlip[i]->setOnTimerCallback([=](TimerPtr t)->bool {       //������ ���� �ð�, �� ������ �� flip �ϸ� ���� �߻�.

			card[i].getCard()->setImage("images/animalGame/animal_back.png");



			t->set(0.8f);

			return true;

			});





		card[i].getCard()->setOnMouseCallback([=](ObjectPtr object, int x, int y, MouseAction)->bool {

			// clickedObj = Ŭ�� �� ī���� ���� 

			for (int j = 0; j < 18; j++) {

				if (card[j].getCard() == object) {

					clickedObj = j;

				}

			}



			// ������ ī��� ����

			int resCheckCard = checkCard(card[clickedObj].getCardNum(), clickedObj);   // | -1: ������ ���� | -2: ���� �¸�! | -3: ù��° ī�� | -4: ������ ���� or �̹� �������� |  |  ������ : PREV��			



			if (!(resCheckCard == -4)) {		// �������� ī�带 ���ų�, ���� �� ī�带 ����� ��찡 �ƴϸ� -> �Ҹ� �߻�, ī�� ������
<<<<<<< HEAD

				S_card->play();

=======
				S_card->play();
>>>>>>> ea2b83450ca2505182cd2c74ce866d9eb38a40b0
				// ī�� ������ - num�� 9���� ũ�� ī���̸� ������ ī��׸�

				string openCardString = clickedObj < 9 ? "images/animalGame/animal" + to_string(card[clickedObj].getCardNum() + 1) + ".png" : "images/animalGame/animal" + to_string(card[clickedObj].getCardNum() + 1) + "_t.png";

				card[clickedObj].getCard()->setImage(openCardString);

			}



			if (resCheckCard > -1) {										// prev���� ���ƿ�����

				showMessage("���� �ٸ� �׸��Դϴ�.");
<<<<<<< HEAD

				S_wrong->play();

=======
				S_wrong->play();
>>>>>>> ea2b83450ca2505182cd2c74ce866d9eb38a40b0
				timerFlip[i]->start();

				timerFlip[resCheckCard]->start();

			}

			else if (resCheckCard == -1) {
<<<<<<< HEAD

				S_right->play();

=======
				S_right->play();
>>>>>>> ea2b83450ca2505182cd2c74ce866d9eb38a40b0
				showMessage("���߼̽��ϴ�!");

			}

			else if (resCheckCard == -2) {

				showMessage("�ź��� �������� ���迡 �հ��ϼ̽��ϴ�!");
<<<<<<< HEAD

				S_success->play();

=======
				S_success->play();
>>>>>>> ea2b83450ca2505182cd2c74ce866d9eb38a40b0
				timer->stop();

				checkStage(2, true);

				hideTimer();

				animal_bgm->stop();

			}



			return true;

			}

		);

	}

	enterScene(animalMain->ID());



}