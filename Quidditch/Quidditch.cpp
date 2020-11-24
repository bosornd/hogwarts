﻿#include <bangtal>
#include <iostream>

using namespace bangtal;
using namespace std;

// 화면의 끝 좌표
const int min_x = 0, max_x = 1200, min_y = 0, max_y = 600;

// 상대 수 ( 수정 시 아래 코드도 수정 필요 )
const int num_enemies = 5;

// 시작 좌표
int my_x = 50, my_y = 300;

int enemy_x[num_enemies] = { 1100, 1000, 1100, 1000, 1100 };
int enemy_y[num_enemies] = { 560, 430, 300, 170, 40 };

int ball_x = 600, ball_y = 300;

// 속도
const int my_speed = 12;
int ball_speed = 2;

// 움직임 프레임
const float interval = 0.02;

// 각종 시간
const float invincible_time = 3.0;
const float special_ball_interval = 7.0;

// 획득 점수
const int goal_score = 1;
const int special_ball_score = 2;

// 캐릭터 선택
string my_ch = "harry";

// 점수, 생명
int score = 0;
int life = 5;

int main()
{
    setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
    setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
    setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);

    // 배경
    auto main_scene = Scene::create("main", "images/main.jpg");

    // 음악
    auto sound = Sound::create("sounds/퀴디치.mp3");
    sound->play(true);

    // 게임 타이머
    auto game_timer = Timer::create(60.0f);
    game_timer->start();
    showTimer(game_timer);
    game_timer->setOnTimerCallback([&](TimerPtr)->bool {
        cout << "Game End! / Final Score : " << score << endl;
        return true;
        });

    // 조작할 캐릭터
    auto my = Object::create("images/" + my_ch + "_right.png", main_scene, my_x, my_y);

    // 상대, 공 생성
    auto ball = Object::create("images/ball0.png", main_scene, ball_x, ball_y);
    ball->setScale(1.5f);
    bool own_ball = false;
    int ball_rotate = 0;
    int ball_rotate_speed = 0;

    int special_ball_x = rand() % 1000 + 100;
    int special_ball_y = rand() % 400 + 100;
    auto special_ball = Object::create("images/special_ball0.png", main_scene, special_ball_x , special_ball_y);
    special_ball->setScale(1.5f);
    auto special_ball_timer = Timer::create(special_ball_interval);
    bool special_ball_visible = true;
    int special_ball_fly = 0;

    // 상대, 공의 이동 방향, 속도
    int direction[6] = { -3, -2, -1, 1, 2, 3 };
    int enemy_dir_x[5] = { direction[rand() % 6], direction[rand() % 6], direction[rand() % 6],
                           direction[rand() % 6], direction[rand() % 6] };
    int enemy_dir_y[5] = { direction[rand() % 6], direction[rand() % 6], direction[rand() % 6],
                           direction[rand() % 6], direction[rand() % 6] };
    int speed[3] = { 2, 3, 4 };
    int enemy_speed[5] = { speed[rand() % 3], speed[rand() % 3], speed[rand() % 3],
                           speed[rand() % 3], speed[rand() % 3] };

    int ball_dir_x = direction[rand() % 6];
    int ball_dir_y = direction[rand() % 6];

    ObjectPtr enemies[num_enemies];
    for (int i = 0; i < num_enemies; i++) {
        string side;
        if (enemy_dir_x[i] < 0) side = "left";
        else side = "right";
        enemies[i] = Object::create("images/enemy_" + side + ".png", main_scene, enemy_x[i], enemy_y[i]);
    }

    // 상대, 공 이동
    auto timer = Timer::create(interval);
    bool invincible = false;
    auto invincible_timer = Timer::create(invincible_time);
    timer->start();
    timer->setOnTimerCallback([&](TimerPtr)->bool {
        // 상대 이동
        for (int i = 0; i < num_enemies; i++) {
            enemy_x[i] += enemy_dir_x[i] * enemy_speed[i];
            enemy_y[i] += enemy_dir_y[i] * enemy_speed[i];

            if ((enemy_x[i] < min_x) || (enemy_x[i] > max_x)) {
                enemy_dir_x[i] *= -1;
                if (enemy_dir_x[i] < 0) enemies[i]->setImage("images/enemy_left.png");
                else enemies[i]->setImage("images/enemy_right.png");
            }
            if ((enemy_y[i] < min_y) || (enemy_y[i] > max_y)) enemy_dir_y[i] *= -1;

            enemies[i]->locate(main_scene, enemy_x[i], enemy_y[i]);

            // my와 부딪혔을 때
            if ((invincible == false) &&
                (my_x - 120 < enemy_x[i]) && (my_x + 110 > enemy_x[i]) && 
                (my_y - 70 < enemy_y[i]) && (my_y + 70 > enemy_y[i])) {
                invincible = true;
                life--;
                cout << "crush! / life : " << life << endl;
                if (life == 0)cout << "Game End! / Final Score : " << score << endl;
                invincible_timer->start();
            }
        }
        
        // 볼 이동
        ball_x += ball_dir_x * ball_speed;
        ball_y += ball_dir_y * ball_speed;

        if ((ball_x < min_x) || (ball_x > max_x)) ball_dir_x *= -1;
        if ((ball_y < min_y) || (ball_y > max_y)) ball_dir_y *= -1;

        ball->locate(main_scene, ball_x, ball_y);
        ball_rotate_speed = (ball_rotate_speed + 1) % 3;
        if (ball_rotate_speed == 0) {
            ball_rotate = (ball_rotate + 1) % 4;
            ball->setImage("images/ball" + to_string(ball_rotate) + ".png");

            special_ball_fly = (special_ball_fly + 1) % 2;
            special_ball->setImage("images/special_ball" + to_string(special_ball_fly) + ".png");
        }
       
        if ((own_ball == false) &&
            (my_x - 10 < ball_x) && (my_x + 150 > ball_x) &&
            (my_y - 30 < ball_y) && (my_y + 70 > ball_y)) {
            own_ball = true;
            cout << "catch!" << endl;
            ball->hide();
            ball_dir_x = direction[rand() % 6];
            ball_dir_y = direction[rand() % 6];
        }

        timer->set(interval);
        timer->start();

        return true;
        });

    // my와 부딪히면 3초간 무적
    invincible_timer->setOnTimerCallback([&](TimerPtr)->bool {
        invincible = false;
        invincible_timer->set(invincible_time);
        return true;
        });

    // 캐릭터 이동
    TimerPtr my_timer[4] = { Timer::create(interval), Timer::create(interval), Timer::create(interval), Timer::create(interval) };
    main_scene->setOnKeyboardCallback([&](ScenePtr scene, int key, bool pressed)->bool {
        if ((pressed == 1) && (key >= 82) && (key <= 85)) {
            my_timer[key - 82]->start();
            my_timer[key - 82]->setOnTimerCallback([=, &own_ball, &invincible, 
                &special_ball_x, &special_ball_y, &special_ball_visible](TimerPtr)->bool {
                if ((key == 82) && (my_x > min_x)) {                        //좌
                    my_x -= my_speed;
                    my->setImage("images/" + my_ch + "_left.png");
                }
                else if ((key == 83) && (my_x < max_x)) {                   //우
                    my_x += my_speed;   
                    my->setImage("images/" + my_ch + "_right.png");
                }
                else if ((key == 84) && (my_y < max_y)) my_y += my_speed;   //상
                else if ((key == 85) && (my_y > min_y)) my_y -= my_speed;   //하
                my->locate(main_scene, my_x, my_y);

                // 골인
                if ((my_x > 1150) && (my_y > 200) && (my_y < 400) && (own_ball == true)) {
                    my_x = 50;
                    my_y = 300;
                    my->locate(main_scene, my_x, my_y);
                    
                    score += goal_score;
                    cout << "Score : " << score << " / goal!" << endl;

                    invincible = true;
                    invincible_timer->start();

                    own_ball = false;
                    ball->show();
                }

                // 스페셜 공 획득
                if ((special_ball_visible == true) &&
                    (my_x - 10 < special_ball_x) && (my_x + 120 > special_ball_x) &&
                    (my_y - 30 < special_ball_y) && (my_y + 40 > special_ball_y)) {
                    special_ball->hide();

                    score += special_ball_score;
                    special_ball_visible = false;

                    cout << "Score : " << score << " / get special ball!" << endl;

                    special_ball_timer->start();
                }

                my_timer[key - 82]->set(interval);
                my_timer[key - 82]->start();

                return true;
                });
        }
        else if ((pressed == 0) && (key >= 82) && (key <= 85)) {
            my_timer[key - 82]->stop();
        }
        return true;
        });

    special_ball_timer->setOnTimerCallback([&](TimerPtr)->bool {
        special_ball_x = rand() % 1000 + 100;
        special_ball_y = rand() % 400 + 100;
        
        special_ball->locate(main_scene, special_ball_x, special_ball_y);
        special_ball->show();
        special_ball_visible = true;

        special_ball_timer->set(special_ball_interval);
        
        return true;
        });


    bangtal::startGame(main_scene);
}
