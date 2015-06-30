/*********************************
*Programação em linguagem C
*Clarice Ribeiro e Gustavo Simas
*V1.6.18
*Titulo: "SHOCK EFFECT"
*********************************/

//inclusao de bibliotecas
#include <stdlib.h>
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/bitmap.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "objects.h" //header de objetos
#include "functions.h" //header de funcoes

////////////////////////////////////////////////////////////////////////////////
//GLOBALS
int WIDTH; //largura display
int HEIGHT; //altura display
const int GRAVITY = 1;
const int FPS = 60;
int back_x; //ponto x do fundo do background
int back_y; //ponto y do fundo do background
enum KEYS {UP, DOWN, LEFT, RIGHT, Q, W, E, R};
bool keys[8] = {false, false, false, false, false, false, false, false};
////////////////////////////////////////////////////////////////////////////////

//main
int main()
{
    int b;
    int letra;
    OpcaoBackground(letra);

    //primitive variables
    int NUM_ENEMYRED = 10; //quantidade de inimigos vermelhos
    int NUM_ENEMYBLUE = 10; //quantidade de inimigos azuis
    int NUM_BOSS = 5;
    int text_color = 255; //variavel para cor (animacao inicial de jogo - efeito relampago)
    int text_boss = 255; //variavel para cor de texto boss
    int FPS = 60; //frames per second
    bool done = false;
    bool redraw = true;

    //object variables
    struct Player player;
    struct Enemy_red enemyred[NUM_ENEMYRED];
    struct Enemy_blue enemyblue[NUM_ENEMYBLUE];
    struct Boss boss[NUM_BOSS];
    struct Shoot shootQ;
    struct Shoot shootW;
    struct Shoot shootE;
    struct Obstacle obstacle;
    struct SpriteScientist scientist;
    struct Sprite background;
    struct Sprite background1;
    struct Sprite background2;
    struct Sprite background3;
    struct Sprite background4;
    struct Sprite background5;
    struct Sprite background6;
    struct Sprite enemyred_sprite;

    //allegro variables
    ALLEGRO_DISPLAY *display;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_SAMPLE *musica = NULL;
    ALLEGRO_SAMPLE *musica2 = NULL;
    ALLEGRO_SAMPLE *musica3 = NULL;
    ALLEGRO_SAMPLE *musica4 = NULL;
    ALLEGRO_SAMPLE *musica5 = NULL;
    ALLEGRO_SAMPLE *musica6 = NULL;
    ALLEGRO_SAMPLE_ID musica3id;
    ALLEGRO_SAMPLE_ID musica6id;

    ALLEGRO_FONT *title_font = NULL;
    ALLEGRO_FONT *medium_font = NULL;

////////////////////////////////////////////////////////////////////////

    //verificacoes de erro
    if(!al_init())
        return -1; //caso de erro ao inicializar allegro

    display = al_create_display(WIDTH,HEIGHT); //criar display

    if(!display)
        return -1; //se der merda

    //Allegro Module Init
    al_init_primitives_addon();
    al_init_font_addon();
    if (!al_init_ttf_addon())
    {
        printf("Falha ao inicializar addon allegro_ttf.\n");
        return -1;
    }

    medium_font = al_load_font("fonts/EHSMB.TTF", WIDTH/20, 0);
    if (!medium_font)
    {
        al_destroy_display(display);
        printf("Falha ao carregar fonte.\n");
        return -1;
    }

    title_font = al_load_font("fonts/French Electric Techno.ttf", WIDTH/8, 0);
    if (!title_font)
    {
        al_destroy_display(display);
        printf("Falha ao carregar fonte.\n");
        return -1;
    }

    al_draw_text(title_font, al_map_rgb(255, 255, 255), WIDTH/2, HEIGHT*0.1, ALLEGRO_ALIGN_CENTRE, "SHOCK EFFECT");
    al_draw_text(title_font, al_map_rgb(255, 255, 255), WIDTH/2, HEIGHT*0.3, ALLEGRO_ALIGN_CENTRE, "LOADING...");
    al_flip_display();

    al_install_keyboard();

    if(!al_init_image_addon())
    {
        printf("Falha ao inicializar image addon");
        return -1;
    }

    al_install_audio();
    if(!al_install_audio())
    {
        printf("Falha ao inicializar audio");
        return -1;
    }

    al_init_acodec_addon();
    if(!al_init_acodec_addon())
    {
        printf("Falha ao inicializar acodec addon");
        return -1;
    }

    al_reserve_samples(6); //reserva amostras de som para serem tocadas
    if(!al_reserve_samples(6))
    {
        printf("Falha ao reservar samples");
        return -1;
    }

    //carregamento de musicas
    musica = al_load_sample("sounds/1/topgearsoundtrack.ogg");
    musica2 = al_load_sample("sounds/1/lucy.ogg");

    al_draw_text(medium_font, al_map_rgb(255, 255, 255), WIDTH/2, HEIGHT*0.5, ALLEGRO_ALIGN_CENTRE, "Mais um pouco...");
    al_flip_display();

    musica3 = al_load_sample("sounds/1/immigrant.ogg");
    musica4 = al_load_sample("sounds/1/melancholy.ogg");

    al_draw_text(medium_font, al_map_rgb(255, 255, 255), WIDTH/2, HEIGHT*0.7, ALLEGRO_ALIGN_CENTRE, "Quase la...");
    al_flip_display();

    musica5 = al_load_sample("sounds/1/starwars.ogg");
    musica6 = al_load_sample("sounds/1/nirvana.ogg");

    if(letra == 1 ||
            letra == 7)
        al_play_sample(musica, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
    if(letra == 2)
        al_play_sample(musica6, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, &musica6id);
    if(letra == 3)
        al_play_sample(musica2, 1.5, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
    if(letra == 4)
        al_play_sample(musica3, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, &musica3id);
    if(letra == 5)
        al_play_sample(musica4, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
    if(letra == 6)
        al_play_sample(musica5, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);

    al_draw_text(medium_font, al_map_rgb(255, 255, 255), WIDTH/2, HEIGHT*0.9, ALLEGRO_ALIGN_CENTRE, "Deu!");
    al_flip_display();

    //ALLEGRO QUEUE
    event_queue = al_create_event_queue();
    timer = al_create_timer(1.0 / FPS);

    //Inicializacao de objetos

    InitShootQ(shootQ); //funcao que inicializa disparo 1 (capacitor)
    if(!shootQ.bitmap)
    {
        al_destroy_display(display);
        printf("Falha ao carregar sprite shootQ.\n");
        return -1;
    }

    InitShootW(shootW); //funcao que inicializa disparo 2 (indutor)
    if(!shootW.bitmap)
    {
        al_destroy_display(display);
        printf("Falha ao carregar sprite shootW.\n");
        return -1;
    }

    InitShootE(shootE); //funcao que inicializa habilidade de escudo (shield / resistor)
    if(!shootE.bitmap)
    {
        al_destroy_display(display);
        printf("Falha ao carregar sprite shield.\n");
        return -1;
    }

    InitScientist(scientist);
    scientist.bitmap = al_load_bitmap("images/scientist.png");
    if (!scientist.bitmap)
    {
        al_destroy_display(display);
        printf("Falha ao carregar sprite scientist.\n");
        return -1;
    }

    InitPlayer(player, &text_color); //funcao que "inicia" player
    InitEnemyRed(enemyred, &NUM_ENEMYRED); //funcao que inicia enemyred
    InitEnemyBlue(enemyblue, &NUM_ENEMYBLUE); //funcao que inicia enemyblue
    InitObstacle(obstacle); //funcao que inicializa obstaculos
    InitBoss(boss, &NUM_BOSS, letra); //funcao que inicializa chefes (bosses)
    InitBackground(background, letra); //funcao que inicializa sprite de background
    InitBackground1(background1, letra); //funcao que inicializa sprite de background1 alternativo
    InitBackground2(background2, letra); //funcao que inicializa sprite de background2 alternativo
    InitBackground3(background3, letra); //funcao que inicializa sprite de background3 alternativo
    InitBackground4(background4, letra); //funcao que inicializa sprite de background4 alternativo
    InitBackground5(background5, letra); //funcao que inicializa sprite de background4 alternativo
    InitBackground6(background6, letra); //funcao que inicializa sprite de background4 alternativo
    InitEnemyredSprite(enemyred_sprite); // funcao que inicializa sprite de inimigo vermelho


    //ALLEGRO REGISTER
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    //Loop do jogo
    while (!done)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        //se clicar para fechar a janela
        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            done = true;
        }
        //evento do timer (vai entrar nesse else if sempre, a nao ser que feche a janela)
        else if(ev.type == ALLEGRO_EVENT_TIMER)
        {
            redraw = true;

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

            ChangeColor(&text_color, player, boss, &NUM_BOSS, &text_boss);
            PlayerJump(player, &keys[UP]);
            PlayerRight(player, &keys[RIGHT], scientist);
            PlayerLeft(player, &keys[LEFT]);
            PlayerSample(player, letra, &musica3id, musica3);
            TransportPlayer(player);
            BossSample(boss, &NUM_BOSS, letra, &musica6id, musica6);

            //updates
            UpdateShootQ(shootQ, player);
            UpdateShootW(shootW, player);
            UpdateShootE(shootE, player);
            UpdateEnemyRed(enemyred, &NUM_ENEMYRED, player, shootQ);
            UpdateEnemyBlue(enemyblue, &NUM_ENEMYBLUE, player, shootW);
            UpdateObstacle(obstacle, medium_font, player);
            UpdateBoss(boss, &NUM_BOSS, &text_boss, player, enemyred, &NUM_ENEMYRED, enemyblue, &NUM_ENEMYBLUE, letra);

            //colisoes
            ShootQColisionEnemyRed(shootQ,enemyred, &NUM_ENEMYRED, player);
            ShootWColisionEnemyBlue(shootW, enemyblue, &NUM_ENEMYBLUE, player);
            ShootColisionBoss(shootW, shootQ, boss, &NUM_BOSS, player);
            PlayerColisionEnemyBlue(player, enemyblue, &NUM_ENEMYBLUE);
            PlayerColisionEnemyRed(player, enemyred, &NUM_ENEMYRED);
            PlayerColisionObstacle(player,obstacle);
            PlayerColisionBoss(player, boss, &NUM_BOSS);

            ResetPlayer(player, enemyred, &NUM_ENEMYRED, enemyblue, &NUM_ENEMYBLUE, obstacle, boss, &NUM_BOSS, &text_color, musica3, &musica3id, letra);
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
            case ALLEGRO_KEY_E:
                keys[E] = true;
                FireShootE(shootE, player);
                break;
            case ALLEGRO_KEY_R:
                keys[R] = true;
                break;
            }
        }

        else if(ev.type == ALLEGRO_EVENT_KEY_UP)
        {
            switch(ev.keyboard.keycode)
            {
            case ALLEGRO_KEY_UP:
                keys[UP] = false;
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
            case ALLEGRO_KEY_R:
                keys[R] = false;
                break;
            }
        }

        if(redraw && al_is_event_queue_empty(event_queue))
        {
            redraw = false;

            //desenhar objetos
            DrawBackground(background, letra);
            DrawBackground1(background1, letra);
            DrawBackground2(background2, letra);
            DrawBackground3(background3, letra);
            DrawBackground4(background4, letra);
            DrawBackground5(background5, letra);
            DrawBackground6(background6, letra);
            DrawText(title_font, medium_font, player, boss, &NUM_BOSS, &text_color, &text_boss, obstacle);
            DrawShootQ(shootQ);
            DrawShootW(shootW);
            DrawShootE(shootE, player);
            DrawEnemyRed(enemyred, &NUM_ENEMYRED, player, enemyred_sprite);
            DrawEnemyBlue(enemyblue, &NUM_ENEMYBLUE, player);
            DrawBoss(boss, &NUM_BOSS, player);
            DrawObstacle(obstacle);
            DrawScientist(player, scientist, &keys[LEFT], &keys[RIGHT]);

            al_flip_display();
        }
    }

    //destroi coisas

    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    al_destroy_font(title_font);
    al_destroy_font(medium_font);
    al_destroy_display(display);
    al_destroy_sample(shootQ.sample);
    al_destroy_sample(shootW.sample);
    al_destroy_sample(musica);
    al_destroy_sample(musica2);
    al_destroy_sample(musica3);
    al_destroy_sample(musica4);
    al_destroy_sample(musica5);
    al_destroy_sample(musica6);
    al_destroy_sample(player.sample[0]);
    al_destroy_sample(player.sample[1]);
    al_destroy_sample(player.sample[2]);
    al_destroy_sample(player.sample[3]);
    al_destroy_sample(player.sample[4]);
    al_destroy_bitmap(scientist.bitmap);
    al_destroy_bitmap(shootE.bitmap);
    al_destroy_bitmap(shootQ.bitmap);
    al_destroy_bitmap(shootW.bitmap);
    for(b=0; b<NUM_ENEMYRED; b++)
    {
        al_destroy_bitmap(enemyred[b].image);
    }
    for(b=0; b<NUM_ENEMYBLUE; b++)
    {
        al_destroy_bitmap(enemyblue[b].image);
    }
    for(b=0; b<background.frame_max; b++)
    {
        al_destroy_bitmap(background.image[b]);
    }
    for(b=0; b<background1.frame_max; b++)
    {
        al_destroy_bitmap(background1.image[b]);
    }
    for(b=0; b<background2.frame_max; b++)
    {
        al_destroy_bitmap(background2.image[b]);
    }
    for(b=0; b<background3.frame_max; b++)
    {
        al_destroy_bitmap(background3.image[b]);
    }
    for(b=0; b<background4.frame_max; b++)
    {
        al_destroy_bitmap(background4.image[b]);
    }
    for(b=0; b<background5.frame_max; b++)
    {
        al_destroy_bitmap(background5.image[b]);
    }
    for(b=0; b<background6.frame_max; b++)
    {
        al_destroy_bitmap(background6.image[b]);
    }

    return 0;
}

//final da MAIN!!
