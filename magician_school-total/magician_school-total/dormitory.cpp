#include <bangtal>
#include <iostream>
#include "main.h"

using namespace bangtal;
using namespace std;

extern ScenePtr mainScene;
//extern int dorm;
extern void setHome(int d);

const int num_dormitory = 4;
string dormitory[num_dormitory] = { "그리핀도르", "레번클로", "슬리데린", "후플푸프" };
string characters[num_dormitory] = { "harry", "luna", "malfoy", "newton" };

const int num_questions = 7;
string questions[num_questions] = { "선생님", "영화", "인사", "장소", "중요", "친구", "활동" };

int point[4] = { 0, 0, 0, 0 };

ScenePtr scene[num_questions + 1];
SoundPtr sound;
TimerPtr timer;

ObjectPtr blank[num_questions + 1];
ObjectPtr question_lst[num_questions + 1];
ObjectPtr answers[num_questions][num_dormitory];

ScenePtr result_scene;
ObjectPtr character;

void dormitory_main()
{
    

    // 음악
    sound = Sound::create("sounds/기숙사 배정.mp3");
    sound->play(true);

    timer = Timer::create(2.0f);

    scene[num_questions] = Scene::create("Selection", "images/dormitory/background.jpg");
    blank[num_questions] = Object::create("images/dormitory/unnamed.png", scene[num_questions], 473, 465);
    question_lst[num_questions] = Object::create("images/dormitory/배정 시작.png", scene[num_questions], 535, 535);

    for (int q = 0; q < num_questions; q++) {
        scene[q] = Scene::create("Question" + to_string(q + 1), "images/dormitory/background.jpg");
        blank[q] = Object::create("images/dormitory/unnamed.png", scene[q], 473, 465);
        if ((q == 2) || (q == 3)) question_lst[q] = Object::create("images/dormitory/" + questions[q] + ".png", scene[q], 565, 508);
        else question_lst[q] = Object::create("images/dormitory/" + questions[q] + ".png", scene[q], 520, 535);

        for (int d = 0; d < num_dormitory; d++) {
            answers[q][d] = Object::create("images/dormitory/" + questions[q] + "-" + dormitory[d] + ".png", scene[q], 450, 350 - 97 * d);
            answers[q][d]->setOnMouseCallback([=](ObjectPtr object, int x, int y, MouseAction action) -> bool {
                point[d]++;
                cout << d << endl;
                scene[q + 1]->enter();

                if (q == num_questions - 1) timer->start();

                return true;
                });
        }
    }

    
    timer->setOnTimerCallback([&](TimerPtr)->bool {
        int max_idx = 0;
        int max = point[0];
        for (int i = 1; i < num_dormitory; i++) {
            if (point[i] > max) {
                max = point[i];
                max_idx = i;
            }
        }

        result_scene = Scene::create("Result", "images/dormitory/" + dormitory[max_idx] + "3.png");

        auto result_Ob = Object::create("images/dormitory/" + dormitory[max_idx] + "3.png", result_scene);

        character = Object::create("images/" + characters[max_idx] + ".png", result_scene, 340, 130);
        character->setScale(3.0f);

        result_scene->enter();
        showMessage("화면을 클릭하여 다음 화면으로 이동하세요.");
        result_Ob->setOnMouseCallback([&, max_idx](ObjectPtr, int, int, MouseAction)->bool {
            //dorm = max_idx;
            sound->stop();
            setHome(max_idx);
            
            return true;
            });

        return true;
        });

    
    //startGame(scene[0]);
    scene[0]->enter();

}