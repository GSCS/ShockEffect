/*********************************
*Programação em linguagem C
*Clarice Ribeiro e Gustavo Simas
*V0.6.13
*Titulo: "NÃO DEFINIDO"
*********************************/

//inclus�o de bibliotecas
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/base.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "objects.h" //arquivo de objetos
////////////////////////////////////////////////////////////////////////////////
//GLOBALS
const int WIDTH = 1200; //largura display
const int HEIGHT = 600; //altura display
const int gravity = 1;
float size_enemy_red;
float size_enemy_blue;
int i=255;
enum KEYS {UP, DOWN, LEFT, RIGHT, Q, W, E, R};
bool keys[8] = {false, false, false, false, false, false, false, false};
////////////////////////////////////////////////////////////////////////////////
//prototypes player
void InitPlayer(Player &player);
void DrawPlayer(Player &player);
void PlayerJump(Player &player);
void PlayerRight(Player &player);
void PlayerLeft(Player &player);

//prototypes enemies
void InitEnemyRed(Enemy_red &enemyred);
void InitEnemyBlue(Enemy_blue &enemyblue);
void DrawEnemyRed(Enemy_red &enemyred, float &size_enemy_red, Player &player);
void DrawEnemyBlue(Enemy_blue &enemyblue, float &size_enemy_blue, Player &player);
void UpdateEnemyRed(Enemy_red &enemyred, float &size_enemy_red, Player &player);
void UpdateEnemyBlue(Enemy_blue &enemyblue, float &size_enemy_blue, Player &player);

//prototipos indutor "Q"
void InitShootQ(Shoot &shootQ);
void DrawShootQ(Shoot &shootQ);
void FireShootQ(Shoot &shootQ, Player &player);
void UpdateShootQ(Shoot &shootQ);

//prototipos capacitor "W"
void InitShootW(Shoot &shootW);
void DrawShootW(Shoot &shootW);
void FireShootW(Shoot &shootW, Player &player);
void UpdateShootW(Shoot &shootW);

//prototipos resistor "E"
/*void InitShootE(struct Shoot *shootE);
void DrawShootE(struct Shoot *shootE);
void FireShootE(struct Shoot *shootE);
void UpdateShootE(struct Shoot *shootE);*/

//protitpos colis�o
void ShootQColisionEnemyRed(Shoot &shootQ, Enemy_red &enemyred, float &size_enemy_red, Player &player);
void ShootWColisionEnemyBlue(Shoot &shootW, Enemy_blue &enemyblue, float &size_enemy_blue, Player &player);
void PlayerColisionEnemyRed(Player &player, Enemy_red &enemyred, float &size_enemy_red);
void PlayerColisionEnemyBlue(Player &player, Enemy_blue &enemyblue, float &size_enemy_blue);

//outros prot�tipos
void ResetPlayer(Player &player);
int Change(int i, Player &player);

//main
int main()
{
    //primitive variables
    const int FPS = 60;
    bool done = false;
    bool redraw = true;

    //object variables
    struct Player player;
    struct Enemy_red enemyred;
    struct Enemy_blue enemyblue;
    struct Shoot shootQ;
    struct Shoot shootW;
    //struct Shoot shootE;

    //allegro variables
    ALLEGRO_DISPLAY *display;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_FONT *title_font = NULL;
    ALLEGRO_FONT *medium_font = NULL;

////////////////////////////////////////////////////////////////////////
    //initializate functions
    if(!al_init())
        return -1; //caso d� erro ao inicializar allegro

    display = al_create_display(WIDTH,HEIGHT); //criar display

    if(!display)
        return -1; //se der merda

    //Allegro Module Init
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    if (!al_init_ttf_addon())
    {
        printf("Falha ao inicializar addon allegro_ttf.\n");
        return -1;
    }
    al_install_keyboard();

    event_queue = al_create_event_queue();
    timer = al_create_timer(1.0 / FPS);
    medium_font = al_load_font("EHSMB.ttf", 50, 0);
    title_font = al_load_font("French Electric Techno.ttf", 200, 0);
    if (!title_font)
    {
        al_destroy_display(display);
        printf("Falha ao carregar fonte.\n");
        return -1;
    }

    //Game Init
    InitPlayer(player); //chamar fun��o que "inicia" player
    InitEnemyRed(enemyred); //chamar fun��o que inicia enemyred
    InitEnemyBlue(enemyblue); //chamar fun��o que inicia enemyblue
    InitShootQ(shootQ);
    InitShootW(shootW);
    //InitShootE(&shootE);

    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    while (!done)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        //se clicar para fechar a janela
        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            done = true;
        }
        //evento do timer (vai entrar nesse else if sempre, a n�o ser que feche a janela)
        else if(ev.type == ALLEGRO_EVENT_TIMER)
        {
            redraw = true;
            if(keys[UP] && player.jump)
            {
                player.vely = -player.jumpSpeed;
                player.jump = false;
            }
            if(keys[RIGHT] && !player.moving)
            {
                player.velx = player.speed;
                player.moving = true;
            }
            if(keys[LEFT] && !player.moving)
            {
                player.velx = player.speed;
                player.moving = true;
            }
            i=Change(i, player);
            al_clear_to_color(al_map_rgb(i*2,i*2,i*2));
            al_draw_text(title_font, al_map_rgb(i,0,0), WIDTH/2, 150, ALLEGRO_ALIGN_CENTRE, "SHOCK EFFECT");
            UpdateShootQ(shootQ);
            UpdateShootW(shootW);
            //UpdateShootE(&shootE);
            ShootQColisionEnemyRed(shootQ,enemyred, size_enemy_red, player);
            ShootWColisionEnemyBlue(shootW, enemyblue, size_enemy_blue, player);
            PlayerColisionEnemyBlue(player, enemyblue, size_enemy_blue);
            PlayerColisionEnemyRed(player, enemyred, size_enemy_red);
            UpdateEnemyRed(enemyred, size_enemy_red, player);
            UpdateEnemyBlue(enemyblue, size_enemy_blue, player);
            al_draw_textf(medium_font, al_map_rgb(255, 255, 255), 50, 20, ALLEGRO_ALIGN_LEFT, "Score: %d", player.score);
            al_draw_textf(medium_font, al_map_rgb(255, 255, 255), WIDTH - 50, 20, ALLEGRO_ALIGN_RIGHT, "Lives: %d", player.lives);


            ResetPlayer(player);
        }

        else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            switch(ev.keyboard.keycode)
            {
            case ALLEGRO_KEY_ESCAPE:
                done = true;
                break;
            case ALLEGRO_KEY_UP:
                keys[UP] = true;
                break;
            case ALLEGRO_KEY_RIGHT:
                keys[RIGHT] = true;
                break;
            case ALLEGRO_KEY_LEFT:
                keys[LEFT] = true;
                break;
            case ALLEGRO_KEY_Q:
                keys[Q] = true;
                FireShootQ(shootQ, player);
                break;
            case ALLEGRO_KEY_W:
                keys[W] = true;
                FireShootW(shootW, player);
                break;
                /*case ALLEGRO_KEY_E:
                    	keys[E] = true;
                        FireShootE(&shootE);
                    	break;*/

            }
        }

        else if(ev.type == ALLEGRO_EVENT_KEY_UP)
        {
            switch(ev.keyboard.keycode)
            {
            case ALLEGRO_KEY_UP:
                keys[UP] = false;
                //player.vely = 0;
                break;
            case ALLEGRO_KEY_RIGHT:
                keys[RIGHT] = false;
                player.moving = false;
                break;
            case ALLEGRO_KEY_LEFT:
                keys[LEFT] = false;
                player.moving = false;
                break;
            case ALLEGRO_KEY_Q:
                keys[Q] = false;
                break;
            case ALLEGRO_KEY_W:
                keys[W] = false;
                break;
            case ALLEGRO_KEY_E:
                keys[E] = false;
                break;
            }
        }

        if(redraw && al_is_event_queue_empty(event_queue))
        {
            PlayerJump(player);

            if(keys[RIGHT] == true)
            {
                PlayerRight(player);
            }

            if(keys[LEFT] == true)
            {
                PlayerLeft(player);
            }

            redraw = false;

            DrawPlayer(player);
            DrawShootQ(shootQ);
            DrawShootW(shootW);
            //DrawShootE(&shootE);
            DrawEnemyRed(enemyred, size_enemy_red, player);
            DrawEnemyBlue(enemyblue, size_enemy_blue, player);
            al_flip_display();
            if(i==0)
            al_clear_to_color(al_map_rgb(0,0,0));
        }
    }

    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    al_destroy_font(title_font);
    al_destroy_font(medium_font);
    al_destroy_display(display);

    return 0;
}//final da MAIN!!
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

//fun��o para chamar jogador
void InitPlayer(Player &player)
{
    player.ID = PLAYER;
    player.x = 10;
    player.y = HEIGHT;
    player.lives = 3;
    player.speed = 7;
    player.jumpSpeed = 15;
    player.jump = true;
    player.moving = false;
    player.colision = false;
    player.alive = true;
    player.velx = 40;
    player.vely = 70;
    player.boundx = 40;
    player.boundy = 70;
    player.score = 0;
    i=0;
};

//fun��o para desenhar jogador
void DrawPlayer(struct Player &player)
{
    if(player.alive)
    {
        al_draw_filled_rectangle(player.x, player.y, player.x + 40, player.y - 70 , al_map_rgb(0, 175, 255));
    }
}

//fun��o para pulo do jogador
void PlayerJump(struct Player &player)
{
//adicionar gravidade ao pulo
    if (player.vely <= player.jumpSpeed && !player.jump)
    {
        player.vely += gravity;
        player.y += player.vely;
        if (player.vely == player.jumpSpeed)
        {
            player.y = HEIGHT;
            player.vely = 0;
            player.jump = true;
        }
    }
}

//Andar para direita
void PlayerRight(struct Player &player)
{
    player.x += player.speed;
    player.moving = true;
}

//andar para esquerda
void PlayerLeft(struct Player &player)
{
    player.x -= player.speed;
    player.moving = true;
}

//fun��o para reiniciar jogador
void ResetPlayer(struct Player &player)
{
    if(player.lives <= 0)
    {
        player.alive = false;
    }
    if(player.x + player.boundx < -80)
    {
        player.alive = false;
    }
    if(player.alive == false)
    {
        player.x = 10;
        player.y = HEIGHT;
        player.lives = 3;
        player.speed = 7;
        player.jumpSpeed = 15;
        player.jump = true;
        player.moving = false;
        player.colision = false;
        player.alive = true;
        player.velx = 40;
        player.vely = 70;
        player.boundx = 40;
        player.boundy = 70;
        player.score = 0;
        i=0;
    }
}

//fun��es poder indutor "Q"//////////////////////////////////////////////////
void InitShootQ(struct Shoot &shootQ)
{
    shootQ.live = false;
    shootQ.speed = 10;
}
void DrawShootQ(struct Shoot &shootQ)
{
    if(shootQ.live)
    {
        al_draw_line(((shootQ.x)), (shootQ.y), ((shootQ.x)), ((shootQ.y) - 25), al_map_rgb(0, 0, 255), 5);
    }
}
void FireShootQ(struct Shoot &shootQ, struct Player &player)
{
    if (!(shootQ.live))
    {
        shootQ.x = (player.x) + 20;
        shootQ.y = (player.y) - 70;
        shootQ.live = true;
    }
}
void UpdateShootQ(struct Shoot &shootQ)
{
    if(shootQ.live)
    {
        shootQ.y -= shootQ.speed;
        if ((shootQ.y) < 0)
        {
            shootQ.live = false;
        }
    }

}

//fun��es poder capacitor "W"//////////////////////////////////////////////////
void InitShootW(struct Shoot &shootW)
{
    shootW.live = false;
    shootW.speed = 10;
}
void DrawShootW(struct Shoot &shootW)
{
    if (shootW.live)
    {
        al_draw_line(((shootW.x)), (shootW.y), ((shootW.x)), ((shootW.y) + 25), al_map_rgb(0, 255, 0), 5);
    }
}
void FireShootW(struct Shoot &shootW, struct Player &player)
{
    if(!(shootW.live))
    {
        shootW.x = (player.x) + 20;
        shootW.y = (player.y) - 70;
        shootW.live = true;
    }
}
void UpdateShootW(struct Shoot &shootW)
{
    if(shootW.live)
    {
        shootW.y -= shootW.speed;
        if ((shootW.y) < 0)
        {
            shootW.live = false;
        }
    }
}

//fun��es poder resistor "E"////////////////////////////////////////////////////////
/*void InitShootE(Shoot &shootE) {
    shootE.ID = SHOOT;
    shootE.live = false;
    shootE.speed = 0;
}
void DrawShootE(Shoot &shootE) {
    if (shootE.live) {
        al_draw_line((shootE.x), (shootE.y), ((shootE.x)), ((shootE.y) + 5), al_map_rgb(255, 0, 0), 40);
    }
}
void FireShootE(Shoot &shootE, Player &player) {
    if (!(shootE.live)) {
        shootE.x = player.x;
        shootE.y = player.y;
        shootE.live = true;
    }
}*/

//fun��o para iniciar inimigo tipo 1 (vermelho)
void InitEnemyRed(struct Enemy_red &enemyred)
{
    enemyred.x = rand() % WIDTH;
    enemyred.y = 200;
    enemyred.speed = 0.4;
    enemyred.velx = 0;
    enemyred.vely = 0;
    enemyred.boundx = 0;
    enemyred.boundy = 0;
    enemyred.jumping = true;
    enemyred.moving = false;
    enemyred.alive = true;
}

//fun��o para desenhar inimigo tipo 1 (vermelho)
void DrawEnemyRed(struct Enemy_red &enemyred, float &size_enemy_red, struct Player &player)
{
    if(enemyred.alive && player.alive)
    {
        al_draw_filled_circle(enemyred.x, enemyred.y, size_enemy_red, al_map_rgb(255,0,0));
    }
    else if(enemyred.alive == false)
    {
        enemyred.x=rand() % WIDTH;
        enemyred.y=200;
        size_enemy_red=0;
    }
}

void UpdateEnemyRed(struct Enemy_red &enemyred, float &size_enemy_red, struct Player &player)
{
    if(enemyred.alive)
    {
        if(size_enemy_red<80)
        {
            size_enemy_red+=enemyred.speed;
            enemyred.y+=1;
        }
    }
}

//fun��o para iniciar inimigo tipo 2 (azul)
void InitEnemyBlue(struct Enemy_blue &enemyblue)
{
    enemyblue.x = rand() % WIDTH;
    enemyblue.y = rand() % HEIGHT/2;
    enemyblue.speed = 0.5;
    enemyblue.velx = 0;
    enemyblue.vely = 0;
    enemyblue.boundx = 0;
    enemyblue.boundy = 0;
    enemyblue.jumping = true;
    enemyblue.moving = false;
    enemyblue.alive = true;
}

//fun��o para desenhar inimigo tipo 2 (azul)
void DrawEnemyBlue(struct Enemy_blue &enemyblue, float &size_enemy_blue, struct Player &player)
{
    if(enemyblue.alive && player.score >= 6)
    {
        al_draw_filled_circle(enemyblue.x, enemyblue.y, size_enemy_blue, al_map_rgb(0,255,255));
    }
    else if(enemyblue.alive == false)
    {
        enemyblue.x=rand() % WIDTH;
        enemyblue.y=rand() % HEIGHT/2;
        size_enemy_blue=0;
    }
}

void UpdateEnemyBlue(struct Enemy_blue &enemyblue, float &size_enemy_blue, struct Player &player)
{
    if(enemyblue.alive && player.score >= 6)
    {
        if(size_enemy_blue<40)
        {
            size_enemy_blue+=enemyblue.speed;
            enemyblue.y+=2;
        }
    }
}

//fun��o de colis�o de tiro Q com inimigo vermelho
void ShootQColisionEnemyRed(struct Shoot &shootQ, struct Enemy_red &enemyred, float &size_enemy_red, struct Player &player)
{
    enemyred.boundx = size_enemy_red;
    enemyred.boundy = size_enemy_red;
    if(enemyred.alive)
    {
        if(shootQ.live &&
                shootQ.x < (enemyred.x + size_enemy_red) &&
                shootQ.x > (enemyred.x - size_enemy_red) &&
                shootQ.y < (enemyred.y + size_enemy_red) &&
                shootQ.y > (enemyred.y - size_enemy_red))
        {
            enemyred.alive = false;
            player.score += 2;
        }
    }
    if(shootQ.live == false)
    {
        enemyred.alive = true;
    }
}

//fun��o de colis�o de tiro W com inimigo azul
void ShootWColisionEnemyBlue(struct Shoot &shootW, struct Enemy_blue &enemyblue, float &size_enemy_blue, struct Player &player)
{
    enemyblue.boundx = size_enemy_blue;
    enemyblue.boundy = size_enemy_blue;
    if(enemyblue.alive)
    {
        if(shootW.live &&
                shootW.x < (enemyblue.x + size_enemy_blue) &&
                shootW.x > (enemyblue.x - size_enemy_blue) &&
                shootW.y < (enemyblue.y + size_enemy_blue) &&
                shootW.y > (enemyblue.y - size_enemy_blue))
        {
            enemyblue.alive = false;
            player.score += 1;
        }
    }
    if(shootW.live == false)
    {
        enemyblue.alive = true;
    }
}

//fun��o para colis�o de jogador com inimigo vermelho
void PlayerColisionEnemyRed(struct Player &player, struct Enemy_red &enemyred, float &size_enemy_red)
{
    enemyred.boundx = size_enemy_red;
    enemyred.boundy = size_enemy_red;
    if(enemyred.alive && player.alive)
    {
        if((enemyred.x + size_enemy_red) > player.x &&
                (enemyred.x - size_enemy_red) < player.x + player.boundx &&
                (enemyred.y + size_enemy_red) > player.y - player.boundy &&
                (enemyred.y - size_enemy_red) < player.y)
        {
            enemyred.alive = false;
            player.score += 2;
            player.lives -= 1;
        }
    }
}

//função para colisão de jogador com inimigo azul
void PlayerColisionEnemyBlue(struct Player &player, struct Enemy_blue &enemyblue, float &size_enemy_blue)
{
    if(enemyblue.alive && player.alive)
    {
        enemyblue.boundx = size_enemy_blue;
        enemyblue.boundy = size_enemy_blue;
        if((enemyblue.x + size_enemy_blue) > player.x &&
                (enemyblue.x - size_enemy_blue) < player.x + player.boundx &&
                (enemyblue.y + size_enemy_blue) > player.y - player.boundy &&
                (enemyblue.y - size_enemy_blue) < player.y)
        {
            enemyblue.alive = false;
            player.score += 1;
            player.lives -= 1;
        }
    }
}

int Change(int i, struct Player &player)
{
    if(i>1)
        i-=1;
    if(i==0)
        i=255;
    return i;
}


/*
void DrawScoreText(struct Player &player)
{
    al_draw_textf(font, al_map_rgb(255, 255, 255), 50, 20, ALLEGRO_ALIGN_LEFT, "Score: %d", player.score);
}*/

