#include "MaraudersMap.h"

#include <bangtal>
#include <iostream>
#include <ctime>

using namespace bangtal;
using namespace std;

extern void checkStage(int n, bool pf);
extern int dorm;

ScenePtr mapScene;

MapObjectPtr player;

TrapPtr trap[trapMAX];

ObjectPtr dark;

int target_xMax = 0, target_xMin = 0, target_yMax = 0, target_yMin = 0;

ObjectPtr lifeOb[3];

SoundPtr mapSound;
TimerPtr mainTimer, checkTimer, crushed_timer;
bool crushed;

void init() {

    switch (dorm) {
    case 0:
        cout << "Dorm : G" << endl;
        target_xMax = 87, target_xMin = 84, target_yMax = 22, target_yMin = 20;
        break;
    case 3:
        cout << "Dorm : H" << endl;
        target_xMax = 25, target_xMin = 23, target_yMax = 9, target_yMin = 4;
        break;
    case 1:
        cout << "Dorm : R" << endl;
        target_xMax = 49, target_xMin = 45, target_yMax = 13, target_yMin = 10;
        break;
    case 2:
        cout << "Dorm : S" << endl;
        target_xMax = 65, target_xMin = 63, target_yMax = 5, target_yMin = 3;
        break;
    }
}


void endMapGame(const string& m, bool pf) {
    showMessage(m);
    mapSound->stop();
    mainTimer->stop();
    hideTimer();
    checkTimer->stop();
    for (int i = 0; i < trapMAX; i++) {
        trap[i]->timer->stop();
    }
    checkStage(3, pf);
}

bool checkTrapRange(int r, int i) {

    if ((player->X() - trap[i]->foot1->X() > -r && player->Y() - trap[i]->foot1->Y() > -r
        && player->X() - trap[i]->foot1->X() < r && player->Y() - trap[i]->foot1->Y() < r)
        || (player->X() - trap[i]->foot2->X() > -r && player->Y() - trap[i]->foot2->Y() > -r
            && player->X() - trap[i]->foot2->X() < r && player->Y() - trap[i]->foot2->Y() < r)) {
        return true;
    }

    return false;

}

const float move_interval = 0.1;
TimerPtr key_timer[4];

void MaraudersMap_main() {

    srand(unsigned(time(NULL)));

    mapScene = Scene::create(" ", "images/map/background.png");

    mapSound = Sound::create("sounds/map.mp3");
    mapSound->play();

    player = MapObject::create(15, 48, mapScene, "images/map/player_basic.png");
    player->setScale(0.03f);

    // player
    for (int i = 0; i < 4; i++) {
        key_timer[i] = Timer::create(move_interval);
    }

    mapScene->setOnKeyboardCallback([&](ScenePtr scene, KeyCode code, bool pressed)->bool {

        /* translate new key code to old key value */
        int key = 0;
        switch (code) {
            case KeyCode::KEY_LEFT_ARROW: key = 82; break;
            case KeyCode::KEY_RIGHT_ARROW: key = 83; break;
            case KeyCode::KEY_UP_ARROW: key = 84; break;
            case KeyCode::KEY_DOWN_ARROW: key = 85; break;
        }

        static bool setCh = false;
        if (!setCh) {
            init();
            setCh = true;
        }

        if ((pressed == 1) && (key >= 82) && (key <= 85)) {
            key_timer[key - 82]->start();
            Direction d = Direction::STAY;

            key_timer[key - 82]->setOnTimerCallback([=, &d](TimerPtr)->bool {
                if (key == 82) d = Direction::LEFT;
                else if (key == 83) d = Direction::RIGHT;
                else if (key == 84) d = Direction::UP;
                else if (key == 85) d = Direction::DOWN;


                if (pressed && d != Direction::STAY) {
                    if (player->move(d)) {
                        player->locate(mapScene, player->setX(player->X()), player->setY(player->Y()));
                        dark->locate(mapScene, player->setX(player->X()) - 1810, player->setY(player->Y()) - 970);
                    }
                }

                key_timer[key - 82]->set(move_interval);
                key_timer[key - 82]->start();

                return true;
                });
        }

        else if ((pressed == 0) && (key >= 82) && (key <= 85)) {
            key_timer[key - 82]->stop();
        }
        return true;

        });


    // dark around
    dark = Object::create("images/map/dark.png", mapScene, player->setX(player->X()) - 1810, player->setY(player->Y()) - 970);
    dark->setScale(1.2f);


    // trap
    int timeArr[trapMAX] = { 0, 0, 0, 1, 1, 1, 2, 2, 3, 3, 4, 4, 4 };
    for (int i = 0; i < trapMAX; i++) {
        int j = rand() % trapMAX;

        if (i != j) {
            swap(timeArr[i], timeArr[j]);
        }
    }

    int speedArr[trapMAX];
    for (int i = 0; i < trapMAX; i++) {
        timeArr[i] *= 10;
        speedArr[i] = ((rand() % 3) + 1);
    }

    int xyArr[trapMAX][2] = { { 37, 34 },
                        { 45, 37 },
                        { 76, 40 },
                        { 21, 9 },
                        { 58, 8 },
                        { 40, 17 },
                        { 35, 47 },
                        { 50, 14 },
                        { 20, 19 },
                        { 15, 39 },
                        { 19, 3 },
                        { 17, 35 },
                        { 71, 22 } };



    for (int i = 0; i < trapMAX; i++) {


        if (i < upMAX) {
            trap[i] = Trap::create(Direction::UP, xyArr[i][0], xyArr[i][1], mapScene);
        }
        else if (i < upMAX + downMAX) {
            trap[i] = Trap::create(Direction::DOWN, xyArr[i][0], xyArr[i][1], mapScene);
        }
        else if (i < upMAX + downMAX + leftMAX) {
            trap[i] = Trap::create(Direction::LEFT, xyArr[i][0], xyArr[i][1], mapScene);

        }
        else {
            trap[i] = Trap::create(Direction::RIGHT, xyArr[i][0], xyArr[i][1], mapScene);
        }


        trap[i]->foot1->setScale(0.25f);
        trap[i]->foot2->setScale(0.25f);


        trap[i]->timer = Timer::create(0.5f * timeArr[i]);
        trap[i]->timer->setOnTimerCallback([&, i, speedArr](TimerPtr t)->bool {

            trap[i]->foot1->show();
            trap[i]->foot2->show();
            trap[i]->showup = true;

            (trap[i]->direction == Direction::UP || trap[i]->direction == Direction::DOWN)
                ? trap[i]->foot2->changeY(trap[i]->foot1->Y() + trap[i]->setFootXY())
                : trap[i]->foot2->changeX(trap[i]->foot1->X() + trap[i]->setFootXY());
            if (trap[i]->foot2->checkXY(trap[i]->foot2->X(), trap[i]->foot2->Y()))
                trap[i]->locateFootprint(mapScene, 2);
            else trap[i]->turn_trap();


            auto timer2 = Timer::create(0.3f * speedArr[i]);
            timer2->setOnTimerCallback([&, i](TimerPtr t2) -> bool {
                (trap[i]->direction == Direction::UP || trap[i]->direction == Direction::DOWN)
                    ? trap[i]->foot1->changeY(trap[i]->foot2->Y())
                    : trap[i]->foot1->changeX(trap[i]->foot2->X());

                if (trap[i]->foot1->move(trap[i]->direction))
                    trap[i]->locateFootprint(mapScene, 1);
                else trap[i]->turn_trap();

                return true;
                });
            timer2->start();

            //cout << i << "-" << speedArr[i] << endl;
            t->set(0.6f * speedArr[i]);
            t->start();

            return true;
            });

        trap[i]->timer->start();


    }


    // target
    auto gryffindor = Object::create("images/map/g_logo.png", mapScene, 1155, 380);
    gryffindor->setScale(0.2f);
    auto hufflepuff = Object::create("images/map/h_logo.png", mapScene, 350, 580);
    hufflepuff->setScale(0.2f);
    auto ravenclaw = Object::create("images/map/r_logo.png", mapScene, 640, 535);
    ravenclaw->setScale(0.2f);
    auto slytherin = Object::create("images/map/s_logo.png", mapScene, 870, 620);
    slytherin->setScale(0.2f);


    // life
    static int lifeCount = 3;
    for (int i = 0; i < 3; i++) {
        lifeOb[i] = Object::create("images/quidditch/life.png", mapScene, 1200 - 35 * i, 670);
        lifeOb[i]->setScale(0.2f);
    }



    // timer
    mainTimer = Timer::create(60.0f);
    showTimer(mainTimer);
    mainTimer->setOnTimerCallback([](TimerPtr)->bool {
        endMapGame("지도 게임 실패!", false);
        return false;
        });

    mainTimer->start();

    crushed = false;
    static bool end = false;
    crushed_timer = Timer::create(1.0f);
    // check trap & target
    checkTimer = Timer::create(0.1f);
    checkTimer->setOnTimerCallback([&](TimerPtr t)->bool {

        bool nearby = false;
        bool next = false;
        for (int i = 0; i < trapMAX; i++) {

            if (checkTrapRange(1, i) && (crushed == false)) {
                lifeCount--;
                crushed = true;
                lifeOb[lifeCount]->hide();
                crushed_timer->start();
                if (lifeCount == 0) {
                    endMapGame("지도 게임 실패!", false);
                    end = true;
                }
            }
            else if (checkTrapRange(3, i)) {
                //cout << "Someone's right next to me." << " / trap" << i << endl;
                if (trap[i]->showup) next = true;
            }
            else if (checkTrapRange(6, i)) {
                //cout << "There's someone nearby." << " / trap" << i << endl;
                if (trap[i]->showup) nearby = true;
            }
            else {
                //cout << "safe" << endl;
            }

        }

        if (next) player->setImage("images/map/player_next.png");
        else if (nearby) player->setImage("images/map/player_near.png");
        else player->setImage("images/map/player_basic.png");

        if ((player->X() >= target_xMin && player->X() <= target_xMax
            && player->Y() >= target_yMin && player->Y() <= target_yMax)) {
            endMapGame("지도 게임 성공!", true);
            end = true;
        }

        if (!end) {
            t->set(0.1f);
            t->start();
        }

        return true;
        });
    
    crushed_timer->setOnTimerCallback([&](TimerPtr)->bool {
        crushed = false;
        crushed_timer->set(1.0f);

        return true;
        });
    
    checkTimer->start();

    mapScene->enter();

}