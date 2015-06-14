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
#include "functions.h"
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
            PlayerJump(player);
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


            ResetPlayer(player, enemyred);
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
