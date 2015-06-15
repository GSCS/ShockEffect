#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

//GLOBALS
const int WIDTH = 1200; //largura display
const int HEIGHT = 600; //altura display
const int GRAVITY = 1;
float size_enemy_red;
float size_enemy_blue;
int i=255;
int j;
int FPS = 60;
enum KEYS {UP, DOWN, LEFT, RIGHT, Q, W, E, R};
bool keys[8] = {false, false, false, false, false, false, false, false};

//fun��o para chamar jogador
void InitPlayer(Player &player)
{
    player.ID = PLAYER;
    player.x = WIDTH/2;
    player.y = HEIGHT/2;
    player.lives = 3;
    player.speed = 7;
    player.jumpSpeed = 15;
    player.jump = true;
    player.moving = false;
    player.colision = false;
    player.alive = true;
    player.inverted = false;
    player.velx = 0;
    player.vely = 1;
    player.boundx = 40;
    player.boundy = 70;
    player.score = 0;
    player.death_counter = 0;
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
    bool vely_max = false;
//adicionar gravidade ao pulo
    if(!player.inverted)
    {
        if(player.vely <= player.jumpSpeed && !player.jump && keys[UP])
        {
            player.vely = player.jumpSpeed;
            player.jump = true;
        }
        if(player.jump && !vely_max)
        {
            player.vely -= GRAVITY;
            player.y -= player.vely;
        }
        if(player.jump && player.vely == 0)
            vely_max = true;
        if(player.jump && vely_max && player.y < HEIGHT)
        {
            player.y += player.vely;
        }
        if(player.y == HEIGHT)
        {
            player.vely = 15;
            player.jump = false;
            vely_max = false;
        }
    }
    if(player.inverted)
    {
        if(player.vely <= player.jumpSpeed && !player.jump && keys[UP])
        {
            player.vely = player.jumpSpeed;
            player.jump = true;
        }
        if(player.jump && !vely_max)
        {
            player.vely -= GRAVITY;
            player.y += player.vely;
        }
        if(player.jump && player.vely == 0)
            vely_max = true;
        if(player.jump && vely_max && player.y - player.boundy > 0)
        {
            player.y -= player.vely;
        }
        if(player.y - player.boundy == 0)
        {
            player.vely = 15;
            player.jump = false;
            vely_max = false;
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
void ResetPlayer(struct Player &player, struct Enemy_red &enemyred)
{
    if(player.lives <= 0)
    {
        player.alive = false;
    }
    if(player.alive == false)
    {
        player.x = WIDTH/2;
        player.y = HEIGHT/2;
        player.lives = 3;
        player.speed = 7;
        player.jumpSpeed = 15;
        player.jump = true;
        player.moving = false;
        player.colision = false;
        player.alive = true;
        player.inverted = false;
        player.velx = 0;
        player.vely = 1;
        player.boundx = 40;
        player.boundy = 70;
        player.score = 0;
        player.death_counter = 0;
        i=0;
        enemyred.alive=false;
    }
}

void TransportPlayer(struct Player &player)
{
    if(player.x + player.boundx < 0 || player.x > WIDTH)
        player.death_counter++;
    if(player.death_counter > 300)
        player.alive = false;
    if(!player.inverted && !player.jump)
    {
        if(player.x < -150)
        {
            player.y = player.boundy;
            player.x = WIDTH - player.boundx/2;
            player.inverted = true;
            player.death_counter = 0;
        }
        if(player.x > WIDTH + 150)
        {
            player.y = player.boundy;
            player.x = -(player.boundx/2);
            player.inverted = true;
            player.death_counter = 0;
        }
    }
    if(player.inverted && !player.jump)
    {
        if(player.x < -150)
        {
            player.y = HEIGHT;
            player.x = WIDTH - player.boundx/2;
            player.inverted = false;
            player.death_counter = 0;
        }
        if(player.x > WIDTH + 150)
        {
            player.y = HEIGHT;
            player.x = -(player.boundx/2);
            player.inverted = false;
            player.death_counter = 0;
        }
    }
}

//fun��es poder indutor "Q"//////////////////////////////////////////////////
void InitShootQ(struct Shoot &shootQ)
{
    shootQ.live = false;
    shootQ.speed = 0;
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
        if(!player.inverted)
        {
            shootQ.x = (player.x) + 20;
            shootQ.y = (player.y) - 70;
            shootQ.live = true;
        }
        if(player.inverted)
        {
            shootQ.x = (player.x) + 20;
            shootQ.y = (player.y) + 20;
            shootQ.live = true;
        }
    }
}

void UpdateShootQ(struct Shoot &shootQ, struct Player &player)
{
    if(shootQ.live)
    {
        if(!player.inverted)
        {
            shootQ.speed += GRAVITY;
            shootQ.y -= shootQ.speed;
            if ((shootQ.y) < 0)
            {
                shootQ.live = false;
                shootQ.speed = 0;
            }
        }
        if(player.inverted)
        {
            shootQ.speed += GRAVITY;
            shootQ.y += shootQ.speed;
            if ((shootQ.y) > HEIGHT)
            {
                shootQ.live = false;
                shootQ.speed = 0;
            }
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
    if (!(shootW.live))
    {
        if(!player.inverted)
        {
            shootW.x = (player.x) + 20;
            shootW.y = (player.y) - 70;
            shootW.live = true;
        }
        if(player.inverted)
        {
            shootW.x = (player.x) + 20;
            shootW.y = (player.y) + 20;
            shootW.live = true;
        }
    }
}
void UpdateShootW(struct Shoot &shootW, struct Player &player)
{
    if(shootW.live)
    {
        if(!player.inverted)
        {
            shootW.speed += GRAVITY;
            shootW.y -= shootW.speed;
            if ((shootW.y) < 0)
            {
                shootW.live = false;
                shootW.speed = 0;
            }
        }
        if(player.inverted)
        {
            shootW.speed += GRAVITY;
            shootW.y += shootW.speed;
            if ((shootW.y) > HEIGHT)
            {
                shootW.live = false;
                shootW.speed = 0;
            }
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
    enemyred.x = 350 + (rand() % WIDTH-700);
    enemyred.y = 200;
    enemyred.speed = 0.4;
    enemyred.velx = 0;
    enemyred.vely = 0;
    enemyred.boundx = 0;
    enemyred.boundy = 0;
    enemyred.real_size_enemy_red = 80;
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
        enemyred.x= 350 + (rand() % WIDTH-700);
        enemyred.y=200;
        size_enemy_red=0;
    }
}

void UpdateEnemyRed(struct Enemy_red &enemyred, float &size_enemy_red, struct Player &player)
{
    if(enemyred.x + enemyred.boundx > WIDTH || enemyred.x - enemyred.boundx < 0)
        enemyred.alive = false;
    if(enemyred.alive)
    {
        if(size_enemy_red<enemyred.real_size_enemy_red)
        {
            size_enemy_red+=enemyred.speed;
            enemyred.y+=1;
            if(enemyred.x<WIDTH/2)
            {
                if(size_enemy_red<enemyred.real_size_enemy_red/2)
                {
                    enemyred.x-=0.6;
                }
                if(size_enemy_red>=enemyred.real_size_enemy_red/2)
                {
                    enemyred.x+=1.5;
                }
            }
            if(enemyred.x>WIDTH/2)
            {
                if(size_enemy_red<enemyred.real_size_enemy_red/2)
                {
                    enemyred.x+=0.6;
                }
                if(size_enemy_red>=enemyred.real_size_enemy_red/2)
                {
                    enemyred.x-=1.5;
                }
            }
        }
    }
}

//fun��o para iniciar inimigo tipo 2 (azul)
void InitEnemyBlue(struct Enemy_blue &enemyblue)
{
    enemyblue.x = 250 + (rand() % WIDTH-500);
    enemyblue.y = rand() % HEIGHT/2;
    enemyblue.speed = 0.5;
    enemyblue.velx = 0;
    enemyblue.vely = 0;
    enemyblue.boundx = 0;
    enemyblue.boundy = 0;
    enemyblue.real_size_enemy_blue = 40;
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
        enemyblue.x= 250 + (rand() % WIDTH-500);
        enemyblue.y= rand() % HEIGHT/2;
        size_enemy_blue=0;
    }
}

void UpdateEnemyBlue(struct Enemy_blue &enemyblue, float &size_enemy_blue, struct Player &player)
{
    if(enemyblue.x + enemyblue.boundx > WIDTH || enemyblue.x - enemyblue.boundx < 0)
        enemyblue.alive = false;
    if(enemyblue.alive && player.score >= 6)
    {
        if(size_enemy_blue<enemyblue.real_size_enemy_blue)
        {
            size_enemy_blue+=enemyblue.speed;
            enemyblue.y+=1;
            if(enemyblue.x<WIDTH/2)
            {
                if(size_enemy_blue<enemyblue.real_size_enemy_blue/2)
                {
                    enemyblue.x-=0.6;
                }
                if(size_enemy_blue>=enemyblue.real_size_enemy_blue/2)
                {
                    enemyblue.x+=1.5;
                }
            }
            if(enemyblue.x>WIDTH/2)
            {
                if(size_enemy_blue<enemyblue.real_size_enemy_blue/2)
                {
                    enemyblue.x+=0.6;
                }
                if(size_enemy_blue>=enemyblue.real_size_enemy_blue/2)
                {
                    enemyblue.x-=1.5;
                }
            }
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

//função para mudar valor de i (referente a cor de texto)
int Change(int i, struct Player &player)
{
    if(i>1)
        i-=1;
    if(i==0)
        i=255;
    return i;
}

//função para iniciar obstáculo
void InitObstacle(Obstacle &obstacle)
{
    obstacle.x = WIDTH/2;
    obstacle.y = HEIGHT/2;
    obstacle.speed = 0.1;
    obstacle.vely = 0;
    obstacle.size_obst = 0;
    obstacle.real_size_obst = 250;
    obstacle.score = 5;
    obstacle.alive = false;
}

//função para desenhar obstáculo
void DrawObject(Obstacle &obstacle)
{
    al_draw_filled_rectangle(obstacle.x, obstacle.y, obstacle.x + obstacle.size_obst, obstacle.y - 20, al_map_rgb(255, 255, 255));
}

//função para atualizar obstáculo
void UpdateObject(Obstacle &obstacle, ALLEGRO_FONT *medium_font, Player &player)
{
    if(player.score >= obstacle.score)
        obstacle.alive = true;
    if(obstacle.alive)
    {
        if(obstacle.y>-21 || obstacle.y < HEIGHT+21)
        {
            if(obstacle.size_obst<obstacle.real_size_obst)
                obstacle.size_obst++;
            if(obstacle.size_obst<obstacle.real_size_obst/2)
                obstacle.x--;
            if(obstacle.size_obst>obstacle.real_size_obst)
                obstacle.x++;
            if(!player.inverted)
            {
                obstacle.vely += obstacle.speed;
                obstacle.y+=obstacle.vely;
                if(obstacle.y>(HEIGHT-50) &&
                        player.x>=obstacle.x &&
                        player.x+player.boundx <= obstacle.x+obstacle.size_obst)
                    al_draw_textf(medium_font, al_map_rgb(255, 0, 0), WIDTH/2, HEIGHT/2, ALLEGRO_ALIGN_CENTER, "JUMP!");
            }
            if(player.inverted)
            {
                obstacle.vely += obstacle.speed;
                obstacle.y-=obstacle.vely;
                if(obstacle.y<(50) &&
                        player.x>=obstacle.x &&
                        player.x+player.boundx <= obstacle.x+obstacle.size_obst)
                    al_draw_textf(medium_font, al_map_rgb(255, 0, 0), WIDTH/2, HEIGHT/2, ALLEGRO_ALIGN_CENTER, "JUMP!");
            }
        }
    }
    if(obstacle.y>HEIGHT+20 || obstacle.y < -20)
    {
        obstacle.x = 200 + (rand() % WIDTH-400);
        obstacle.y = HEIGHT/2;
        obstacle.speed = 0.1;
        obstacle.vely = 0;
        obstacle.size_obst = 0;
        obstacle.real_size_obst = 250;
        obstacle.score = player.score + 5;
        obstacle.alive = false;
        player.score++;
    }
}

void PlayerColisionObstacle(Player &player, Obstacle &obstacle)
{
    if(obstacle.alive && (obstacle.y<10 || obstacle.y > HEIGHT-10))
    {
        if(player.x>=obstacle.x &&
                player.x+player.boundx <= obstacle.x+obstacle.size_obst && !player.jump)
        {
            obstacle.x = 200 + (rand() % WIDTH-50);
        obstacle.y = HEIGHT/2;
        obstacle.speed = 0.1;
        obstacle.vely = 0;
        obstacle.size_obst = 0;
        obstacle.real_size_obst = 250;
            obstacle.alive = false;
            obstacle.score = player.score + 5;
            player.lives-=1;
            player.jump = true;
        }
    }
}


/*void SlowMo(ALLEGRO_TIMER *slowmo, ALLEGRO_TIMER *timer, ALLEGRO_EVENT_QUEUE *event_queue)
{
    if(keys[R] == true)
    {
        slowmo = al_create_timer(1.0 / 60);
        al_register_event_source(event_queue, al_get_timer_event_source(slowmo));
        al_start_timer(slowmo);
    }
    else if(keys[R] == false)
    {
        al_destroy_timer(slowmo);
    }
}*/

/*
void DrawScoreText(struct Player &player)
{
    al_draw_textf(font, al_map_rgb(255, 255, 255), 50, 20, ALLEGRO_ALIGN_LEFT, "Score: %d", player.score);
}*/

#endif // FUNCTIONS_H_INCLUDED
