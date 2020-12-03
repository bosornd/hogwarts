#include <iostream>
#include <ctime>
#include "astronomyGame.h"

extern void checkStage(int n, bool pf);

ScenePtr astronomyOne;
ScenePtr astronomyTwo;
ScenePtr astronomyThree;

ObjectPtr AriesSample;
ObjectPtr AriesBoard;
ObjectPtr CancerSample;
ObjectPtr CancerBoard;
ObjectPtr CapricornusSample;
ObjectPtr CapricornusBoard;

StarPtr AriesStar;
StarPtr CancerStar;
StarPtr CapricornusStar;
StarPtr Aries[5];
StarPtr Cancer[5];
StarPtr   Capricornus[9];

ObjectPtr AriesLines[4];
ObjectPtr CancerLines[4];
ObjectPtr CapricornusLines[8];

int astronomyPoint = 0;

SoundPtr Success;
SoundPtr astronomy_bgm;

//메인 함수
void astronomyGame() {
    astronomy_bgm = Sound::create("sounds/astronomyGame/천문학.mp3");
<<<<<<< HEAD
    astronomy_bgm->play(true);
=======
    astronomy_bgm->play();

>>>>>>> ea2b83450ca2505182cd2c74ce866d9eb38a40b0
    astronomyOne = Scene::create("", "images/astronomyGame/astronomyScene.png");
    astronomyTwo = Scene::create("", "images/astronomyGame/astronomyScene.png");
    astronomyThree = Scene::create("", "images/astronomyGame/astronomyScene.png");

    Success = Sound::create("sounds/astronomyGame/Success.mp3");

    auto timer1 = Timer::create(20.0f);
    auto timer2 = Timer::create(20.0f);
    auto timer3 = Timer::create(20.0f);
    auto timerShowAns = Timer::create(1.7f);

    timer1->setOnTimerCallback([&](TimerPtr)->bool {
        showMessage("양자리 찾기 실패!");
        enterScene(astronomyTwo->ID());
        return true;
        });

    timer2->setOnTimerCallback([&](TimerPtr)->bool {
        showMessage("게자리 찾기 실패!!");
        enterScene(astronomyThree->ID());
        return true;
        });

    timer3->setOnTimerCallback([&](TimerPtr)->bool {
        showMessage("염소자리 찾기 실패!!");

        astronomy_bgm->stop();
        astronomyPoint > 2 ? checkStage(1, true) : checkStage(1, false);
        hideTimer();
        return true;
        });
    showTimer(timer1);

    AriesSample = Object::create("images/astronomyGame/aries_sample.png", astronomyOne, 965, 430);
    AriesBoard = Object::create("images/astronomyGame/aries.png", astronomyOne, 300, 50);

    CancerSample = Object::create("images/astronomyGame/cancer_sample.png", astronomyTwo, 965, 430);
    CancerBoard = Object::create("images/astronomyGame/cancer.png", astronomyTwo, 300, 50);
    CapricornusSample = Object::create("images/astronomyGame/capricornus_sample.png", astronomyThree, 965, 430);
    CapricornusBoard = Object::create("images/astronomyGame/capricornus.png", astronomyThree, 300, 50);
    showMessage("힌트를 참고하여 별자리를 완성해보세요!");

    //Aries 별, 선 생성
    for (int i = 0; i < sizeof(ariesLocation) / sizeof(ariesLocation[0]); i++) {
        if (i < 4) {
            string line = "images/astronomyGame/aries" + to_string(i + 1) + ".png";
            AriesLines[i] = Object::create(line, astronomyOne, ariesLinesLocation[i][0], ariesLinesLocation[i][1], false);
        }
        timer1->start();
        string img = "images/astronomyGame/ariesS" + to_string(i + 1) + ".png";
        Aries[i] = Star::create(ariesLocation[i][0], ariesLocation[i][1], i, astronomyOne, img);
        Aries[i]->setOnMouseCallback([=](ObjectPtr object, int x, int y, MouseAction action)->bool {
            Aries[i]->clickStar(ariesAdd, ariesLines, 4);
            int size = Ans.size();
            for (int j = 0; j < size; j++) {
                AriesLines[Ans[j]]->show();
            }

            if (Ans.size() == 4) {
                timerShowAns->setOnTimerCallback([&](TimerPtr t)->bool {       //뒤집기 위한 시간, 두 사진을 다 flip 하면 오류 발생.
                    enterScene(astronomyTwo->ID());
                    showTimer(timer2);
                    t->set(1.7f);
                    return true;
                    });
                astronomyPoint += 1;
                showMessage("양자리 맞추기 성공!");
                Ans.clear();
                Success->play(false);
                timer1->stop();
                timerShowAns->start();
            }
            return true;
            });
    }

    //Cancer 별, 선 생성
    for (int i = 0; i < sizeof(cancerLocation) / sizeof(cancerLocation[0]); i++) {
        if (i < 4) {
            string line = "images/astronomyGame/cancer" + to_string(i + 1) + ".png";
            CancerLines[i] = Object::create(line, astronomyTwo, cancerLinesLocation[i][0], cancerLinesLocation[i][1], false);
        }
        string img = "images/astronomyGame/cancerS" + to_string(i + 1) + ".png";
        Cancer[i] = Star::create(cancerLocation[i][0], cancerLocation[i][1], i, astronomyTwo, img);
        Cancer[i]->setOnMouseCallback([=](ObjectPtr object, int x, int y, MouseAction action)->bool {
            timer2->start();
            showTimer(timer2);
            Cancer[i]->clickStar(cancerAdd, cancerLines, 4);
            int size = Ans.size();
            for (int j = 0; j < size; j++) {
                CancerLines[Ans[j]]->show();
            }
            if (Ans.size() == 4) {
                timerShowAns->setOnTimerCallback([&](TimerPtr t)->bool {       //뒤집기 위한 시간, 두 사진을 다 flip 하면 오류 발생.
                    enterScene(astronomyThree->ID());
                    showTimer(timer3);
                    t->set(1.7f);
                    return true;
                    });
                showMessage("게자리 맞추기 성공!");
                Success->play(false);
                timer2->stop();
                timerShowAns->start();
                astronomyPoint += 1;
            }
            return true;
            });
    }
    //Capricornus 별, 선 생성
    for (int i = 0; i < sizeof(capricornusLocation) / sizeof(capricornusLocation[0]); i++) {
        if (i < 8) {
            string line = "images/astronomyGame/capricornus" + to_string(i + 1) + ".png";
            CapricornusLines[i] = Object::create(line, astronomyThree, capricornusLinesLocation[i][0], capricornusLinesLocation[i][1], false);
        }
        string img = "images/astronomyGame/capricornusS" + to_string(i + 1) + ".png";
        Capricornus[i] = Star::create(capricornusLocation[i][0], capricornusLocation[i][1], i, astronomyThree, img);
        Capricornus[i]->setOnMouseCallback([=](ObjectPtr object, int x, int y, MouseAction action)->bool {

            timer3->start();
            showTimer(timer3);
            Capricornus[i]->clickStar(capricornusAdd, capricornusLines, 8);
            int size = Ans8.size();
            for (int j = 0; j < size; j++) {
                CapricornusLines[Ans8[j]]->show();
            }
            if (Ans8.size() == 8) {
                timerShowAns->setOnTimerCallback([&](TimerPtr t)->bool {       //뒤집기 위한 시간, 두 사진을 다 flip 하면 오류 발생.
                    hideTimer();
                    astronomyPoint > 2 ? checkStage(1, true) : checkStage(1, false);
                    return true;
                    });
                Success->play(false);
                timer3->stop();
                astronomyPoint += 1;
                timerShowAns->start();
                astronomyPoint > 2 ? showMessage("천문학 게임 성공!") : showMessage("천문학 게임 실패!");
                hideTimer();
            }
            return true;
            });
    }
    enterScene(astronomyOne->ID());
}