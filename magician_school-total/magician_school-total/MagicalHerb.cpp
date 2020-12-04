#include <bangtal>
#include<iostream>
#include <stdio.h>
#define SIZE 5
using namespace bangtal;
using namespace std;

extern void checkStage(int n, bool pf);

ScenePtr magicpotion, bg2;
SoundPtr BGM, adding[8], end1, end2;
ObjectPtr herbs[8], question_herb[8], preques, bowl, pass, fail, result, Student;
int wrong_num = 0, num = 0, total = 0, p = 0, f = 0;
int herbs_count[8] = { 0,0,0,0,0,0,0,0 };
int answer_herb[8][8] = { {0,1,0,0,0,1,2,2}, {0,0,1,2,1,1,0,0}, {0,3,0,0,0,0,3,0}, {2,0,1,0,1,0,0,1}, {0,0,0,1,3,1,1,0}, {1,1,1,1,1,1,1,2}, {0,0,1,1,0,3,0,0}, {2,2,1,0,0,0,0,1} };
int herb_x = 90, herb_y = 550;

int sound_num = 0;

int magicalHerb(const string ch)
{
    BGM = Sound::create("sounds/herb/magicpotion.mp3");
    BGM->play();
    magicpotion = Scene::create("MagicalHerb", "images/herb/magicpotion.png");
    bg2 = Scene::create("MagicalHerb", "images/herb/bg2.png");
    end1 = Sound::create("sounds/herb/end1.mp3");
    end2 = Sound::create("sounds/herb/end2.mp3");
    for (int i = 0; i < 5; i++) {
        adding[i] = Sound::create("sounds/herb/plus" + to_string(i) + ".mp3");
    }


    auto Student = Object::create("images/" + ch + ".png", magicpotion, 840, 70);
    Student->setScale(2.0f);

    for (int i = 0; i < 8; i++) {
        string filename1 = "images/herb/h" + to_string(i + 1) + ".png";
        herbs[i] = Object::create(filename1, magicpotion, herb_x, herb_y);
        herb_x += 140;
    };//샐러리, 코카인, 크랜베리, 인삼, 라벤더, 맨드레이크, 민트, 로즈마리

    herbs[0]->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction)->bool {
        herbs_count[0] += 1;
        adding[sound_num]->play();
        sound_num = (sound_num + 1) % 5;
        return true;
        });
    herbs[1]->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction)->bool {
        herbs_count[1] += 1;
        adding[sound_num]->play();
        sound_num = (sound_num + 1) % 5;
        return true;
        });
    herbs[2]->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction)->bool {
        herbs_count[2] += 1;
        adding[sound_num]->play();
        sound_num = (sound_num + 1) % 5;
        return true;
        });
    herbs[3]->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction)->bool {
        herbs_count[3] += 1;
        adding[sound_num]->play();
        sound_num = (sound_num + 1) % 5;
        return true;
        });
    herbs[4]->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction)->bool {
        herbs_count[4] += 1;
        adding[sound_num]->play();
        sound_num = (sound_num + 1) % 5;
        return true;
        });
    herbs[5]->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction)->bool {
        herbs_count[5] += 1;
        adding[sound_num]->play();
        sound_num = (sound_num + 1) % 5;
        return true;
        });
    herbs[6]->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction)->bool {
        herbs_count[6] += 1;
        adding[sound_num]->play();
        sound_num = (sound_num + 1) % 5;
        return true;
        });
    herbs[7]->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction)->bool {
        herbs_count[7] += 1;
        adding[sound_num]->play();
        sound_num = (sound_num + 1) % 5;
        return true;
        });

    preques = Object::create("images/herb/q0.png", bg2, 20, 20);
    bowl = Object::create("images/herb/bowl.png", magicpotion, 490, 70);
    bowl->setScale(1.5f);
    result = Object::create("images/herb/wrong.png", bg2, 300, 200);
    result->hide();
    pass = Object::create("images/herb/pass.png", bg2, 20, 20);
    pass->hide();
    fail = Object::create("images/herb/fail.png", bg2, 20, 20);
    fail->hide();


    for (int i = 0; i < 8; i++) {
        string filename2 = "images/herb/q" + to_string(i + 1) + ".png";
        question_herb[i] = Object::create(filename2, bg2, 20, 20);
        question_herb[i]->hide();
        question_herb[i]->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction)->bool {
            question_herb[num]->hide();
            magicpotion->enter();
            return true;
            });
    }


    preques->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction)->bool {
        preques->hide();
        wrong_num = 0;
        num = rand() % 8;
        question_herb[num]->show();
        for (int k = 0; k < 8; k++) {
            printf("%d", answer_herb[num][k]);
        };
        return 0;
        });

    bowl->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction)->bool {

        for (int t = 0; t < 8; t++) {
            printf("%d", herbs_count[t]);
        };

        for (int i = 0; i < 8; i++) {
            if (answer_herb[num][i] != herbs_count[i])
            {
                wrong_num += 1;
            }
        };
        total += 1;

        if (wrong_num > 0) {
            bg2->enter();
            result->setImage("images/herb/wrong.png");
            f += 1;
            question_herb[num]->hide();
            result->show();
            return 0;
        }
        else if (wrong_num == 0) {
            bg2->enter();
            result->setImage("images/herb/correct.png");
            p += 1;
            question_herb[num]->hide();
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
            if (p > f) {
                end1->play();
                pass->show();
            }
            else {
                end2->play();
                fail->show();
            };

        }
        else {
            wrong_num = 0;
            num = rand() % 8;
            question_herb[num]->show();
        };

        return 0;
        });

    pass->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction)->bool {
        BGM->stop();
        showMessage("마법 약초학 게임 성공!");
        checkStage(0, true);
        return 0;
        });

    fail->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction)->bool {
        BGM->stop();
        showMessage("마법 약초학 게임 실패!");
        checkStage(0, false);
        return 0;
        });

    bg2->enter();
    return 0;
}