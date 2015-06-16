#ifndef __objects_h
#define __objects_h

//obj IDS (para indentificar o objeto)
enum IDS{PLAYER, ENEMY, BULLET};

//estrutura do player
struct Player {
    char ID;
    int x; //posição x
    int y; //posição y
    int lives;
    int speed;
    int jumpSpeed;
    bool jump;
    bool moving;
    bool colision;
    bool alive;
    bool inverted;
    int velx;
    int vely;
    int boundx; //(?) descobrir ainda
    int boundy; //(?) descobrir ainda
    int score;
    int death_counter;
};

struct Enemy_red {
    float x;
    float y;
    float speed;
    float size_enemy_red;
    int velx;
    int vely;
    int boundx;
    int boundy;
    int real_size_enemy_red;
    bool moving;
    bool alive;
};

struct Enemy_blue {
    float x;
    float y;
    float speed;
    int velx;
    int vely;
    int boundx;
    int boundy;
    int real_size_enemy_blue;
    bool moving;
    bool alive;
};

struct Shoot {
    char ID;
    int x;
    int y;
    int speed;
    bool live;
};

struct Obstacle{
    char ID;
    float x;
    float y;
    float speed;
    float velx;
    float vely;
    int size_obst;
    int real_size_obst;
    int score;
    bool alive;
};

#endif
