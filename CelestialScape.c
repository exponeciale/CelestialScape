#include <stdio.h>
#include <math.h>
#include <curses.h>
#include <stdlib.h>
#include <time.h>

#define MOON_HEIGHT 17
#define MOON_WIDTH 50
#define MOON_ASPECT_RATIO 2
#define MOON_RADIUS 5
#define MOON_CENTER_X (MOON_WIDTH / 2.0)
#define MOON_CENTER_Y (MOON_HEIGHT / 2.0)

#define NUM_STARS 140

#define MAP_HEIGHT 34
#define MAP_WIDTH 140
#define ASPECT_RATIO 2
#define RADIUS 17
#define CENTER_X (MAP_WIDTH / 2.0)
#define CENTER_Y (MAP_HEIGHT / 2.0)

const char *mapMoon[MOON_HEIGHT] = {
  "oooooooooooooooooooooooooooooooooooooooooooooooooo",
  "oooooooooooooooooooooooooooooooooooooooooooooooooo",
  "oooooooooooooooooooooooooooooooooooooooooooooooooo",
  "oooooooooooooooooooooooooooooooooooooooooooooooooo",
  "oooooooooooooooooooooooooooooooooooooooooooooooooo",
  "oooooooooooooooooooooooooooooooooooooooooooooooooo",
  "oooooooooooooooooooooooooooooooooooooooooooooooooo",
  "oooooooooooooooooooooooooooooooooooooooooooooooooo",
  "oooooooooooooooooooooooooooooooooooooooooooooooooo",
  "oooooooooooooooooooooooooooooooooooooooooooooooooo",
  "oooooooooooooooooooooooooooooooooooooooooooooooooo",
  "oooooooooooooooooooooooooooooooooooooooooooooooooo",
  "oooooooooooooooooooooooooooooooooooooooooooooooooo",
  "oooooooooooooooooooooooooooooooooooooooooooooooooo",
  "oooooooooooooooooooooooooooooooooooooooooooooooooo",
  "oooooooooooooooooooooooooooooooooooooooooooooooooo",
  "                                                  "
};

const char *mapWorld[MAP_HEIGHT] = {
  "--------------------------------------------------------------------------------------------------------------------------------------------",
  "--------------------------------------------------------------------------------------------------------------------------------------------",
  "----------------------------------+++++++--+++++++++++++++++++------------------------------------------------------------------------------",
  "----------------------+-+++++--+-+-+++++--------++++++++++++++-----------------------------+----------++++++++++++++--+-----++--------------",
  "------++++++++++++++++++++++++++++++++--++++-----++++++++++-----------------++++++++-----+++++++++++++++++++++++++++++++++++++++++++++++++++",
  "------+++++++++++++++++++++++++++++-----++++------++++-------------------++++-+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++-",
  "--------++-------+++++++++++++++++------++++++----------------------+----++++--++++++++++++++++++++++++++++++++++++++++++++++------++-------",
  "--------------------++++++++++++++++++-+++++++++--------------------++--++++++++++++++++++++++++++++++++++++++++++++++++++++-------+--------",
  "----------------------+++++++++++++++++++++++------------------------+++++++++++++++++++++++++++++++++++++++++++++++++++++++----------------",
  "----------------------+++++++++++++++++++++------------------------++++---+-+++++----++++-+++++++++++++++++++++++++++++++-------------------",
  "----------------------+++++++++++++++++++--------------------------+++--------+--++++++++--+++++++++++++++++++++++++---+----+---------------",
  "-------------------------++++++++++++++----------------------------+++++++++--+----++++++++++++++++++++++++++++++++++-----------------------",
  "--------------------------++++++------+--------------------------++++++++++++++++++++++++++++++++++++++++++++++++++++-----------------------",
  "-----------------------------+++--------------------------------++++++++++++++++++++-++++++++----+++++++++++++++++--------------------------",
  "-------------------------------++-++----------------------------+++++++++++++++++++++-+++++-------++++-----+++++----------------------------",
  "------------------------------------++--------------------------+++++++++++++++++++++++++----------++-------+-++----------------------------",
  "----------------------------------------+++++++------------------++++++++++++++++++++++++---------------------------------------------------",
  "---------------------------------------+++++++++++------------------------++++++++++++++---------------------+---+++------------------------",
  "---------------------------------------++++++++++++++++-------------------++++++++++++-----------------------++--++--------++---------------",
  "---------------------------------------+++++++++++++++++-------------------++++++++++---------------------------------------++--------------",
  "----------------------------------------+++++++++++++++--------------------+++++++++++----------------------------------+++--+--------------",
  "------------------------------------------+++++++++++++--------------------+++++++++---++----------------------------++++++++++-------------",
  "-------------------------------------------+++++++++------------------------++++++++---+--------------------------+++++++++++++++-----------",
  "------------------------------------------+++++++++-------------------------++++++--------------------------------+++++++++++++++-----------",
  "------------------------------------------+++++++----------------------------+++-----------------------------------+++----+++++++-----------",
  "------------------------------------------++++--------------------------------------------------------------------------------+-------------",
  "-----------------------------------------++++-----------------------------------------------------------------------------------------------",
  "-----------------------------------------+++------------------------------------------------------------------------------------------------",
  "--------------------------------------------------------------------------------------------------------------------------------------------",
  "--------------------------------------------------------------------------------------------------------------------------------------------",
  "--------------------------------------------------------------------------------------------------------------------------------------------",
  "----------------------------------------------------------------------***------************--------*************----------------------------",
  "-------******-------------**********---------------------------***********************************************************------------------",
  "----------*******************************----------*----*****************************************************************************-------"
};

int clip(int v, int max)
{
    return v < max ? v : max - 1;
}

int main()
{
    initscr();
    start_color();
    cbreak();
    noecho();
    curs_set(0);
    srand(time(NULL));
        struct Star {
        int x;
        int y;
        char ch;
    };
    struct Star stars[NUM_STARS];

    // Inicializa as estrelas aleatoriamente
    for (int i = 0; i < NUM_STARS; i++) {
        // Gera coordenadas aleatórias para a estrela
        stars[i].x = rand() % COLS;
        stars[i].y = rand() % LINES;

        // Gera o primeiro caractere aleatório para a estrela
        stars[i].ch = (rand() % 3 == 0) ? '.' : (rand() % 2 == 0) ? '+' : '*';
    }
    int maxX, maxY;
    getmaxyx(stdscr, maxY, maxX);

    float a = MOON_RADIUS * 22.0;
    float b = MOON_RADIUS * 0.5 * MOON_ASPECT_RATIO;
    float moonAngle = 0.0;
    float moonAngleIncrement = 4.0;

    float worldAngle = 0.0;
    float worldAngleIncrement = 6.0;

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, 20, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, 231, COLOR_BLACK);
    init_pair(5, 236, COLOR_BLACK);
    init_pair(6, 223, COLOR_BLACK);
    init_pair(7, 222, COLOR_BLACK);
    init_pair(8, 221, COLOR_BLACK);
    init_pair(9, 255, COLOR_BLACK);

    while (1)
    {
        erase();
        //estrela animação
        for (int i = 0; i < NUM_STARS; i++) {
            // Define a cor apropriada para o caractere
            switch (stars[i].ch) {
                case '.':
                    attron(COLOR_PAIR(6));
                    break;
                case '+':
                    attron(COLOR_PAIR(7));
                    break;
                case '*':
                    attron(COLOR_PAIR(8));
                    break;
            }

            // Imprime o caractere atual da estrela na posição correspondente
            mvaddch(stars[i].y, stars[i].x, stars[i].ch);

            // Desativa a cor após a impressão do caractere
            attroff(COLOR_PAIR(6));
            attroff(COLOR_PAIR(7));
            attroff(COLOR_PAIR(8));

            // Alterna o caractere para o próximo na sequência
            if (stars[i].ch == '.') {
                stars[i].ch = '+';
            } else if (stars[i].ch == '+') {
                stars[i].ch = '*';
            } else {
                stars[i].ch = '.';
            }
        }

        // animação da terra
        int worldOffset = (int)(MAP_WIDTH * worldAngle / (2.0 * M_PI));
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            for (int y = 0; y < MAP_HEIGHT; y++)
            {
                float distance = sqrt(pow((x - CENTER_X) / ASPECT_RATIO, 2) + pow(y - CENTER_Y, 2));

                if (distance <= RADIUS)
                {
                    if (mapWorld[y][(x - worldOffset + MAP_WIDTH) % MAP_WIDTH] == '-')
                    {
                        attron(COLOR_PAIR(2));
                    }
                    else if (mapWorld[y][(x - worldOffset + MAP_WIDTH) % MAP_WIDTH] == '+')
                    {
                        attron(COLOR_PAIR(3));
                    } else if (mapWorld[y][(x - worldOffset + MAP_WIDTH) % MAP_WIDTH] == '*')
                    {
                        attron(COLOR_PAIR(9));
                    }

                    // Check if the point is in the right half of the circle
                    if (x <= CENTER_X - 1)
                    {
                        // Set the color to white
                        if (mapWorld[y][(x - worldOffset + MAP_WIDTH) % MAP_WIDTH] == '+')
                        {
                            attron(COLOR_PAIR(4));
                        }
                        else if (mapWorld[y][(x - worldOffset + MAP_WIDTH) % MAP_WIDTH] == '-')
                        {
                        attron(COLOR_PAIR(5));
                        } else if (mapWorld[y][(x - worldOffset + MAP_WIDTH) % MAP_WIDTH] == '*')
                        {
                        attron(COLOR_PAIR(4));
                        }
                }

                // Imprimir o caractere correspondente
                mvprintw(y + 12, x, "%c", mapWorld[y][(x - worldOffset + MAP_WIDTH) % MAP_WIDTH]);

                //desativar cores
                attroff(COLOR_PAIR(2));
                attroff(COLOR_PAIR(3));
                attroff(COLOR_PAIR(4));
                attroff(COLOR_PAIR(5));
                attroff(COLOR_PAIR(9));
            }
        }
    }

    //animação da lua
    float t = moonAngle / (2.0 * M_PI);
    int moonOffsetX = (int)(a * cos(2.0 * M_PI * t) / MOON_ASPECT_RATIO);
    int moonOffsetY = (int)(b * sin(2.0 * M_PI * t));

    for (int y = 0; y < MOON_HEIGHT; y++)
    {
        for (int x = 0; x < MOON_WIDTH; x++)
        {
            float distance = sqrt(pow((x - MOON_CENTER_X) / MOON_ASPECT_RATIO, 2) + pow(y - MOON_CENTER_Y, 2));

            if (distance <= MOON_RADIUS)
            {
                if (mapMoon[y][(x + moonOffsetX + MOON_WIDTH) % MOON_WIDTH] == 'o')
                {
                    attron(COLOR_PAIR(1));
                }

                int printX = MOON_WIDTH - 1 - (x + moonOffsetX) + 44;
                int printY = y + moonOffsetY + 9;

                printX = clip(printX, maxX);
                printY = clip(printY, maxY);

                distance = sqrt(pow((printX - CENTER_X) / ASPECT_RATIO, 2) + pow(printY - CENTER_Y, 2));

                if ((moonOffsetY > 0) || (distance > RADIUS))
                {
                    mvprintw(printY + 12, printX, "%c", mapMoon[y][(x + moonOffsetX + MOON_WIDTH) % MOON_WIDTH]);
                }


                attroff(COLOR_PAIR(1));
            }
        }
    }

    refresh();
    napms(100);
    moonAngle += moonAngleIncrement * (M_PI / 180.0);
    if (moonAngle >= 2 * M_PI)
    {
        moonAngle -= 2 * M_PI;
        }
        worldAngle += worldAngleIncrement * (M_PI / 180.0);
        if (worldAngle >= 2 * M_PI)
        {
            worldAngle -= 2 * M_PI;
        }
    }

    endwin();
    return 0;
}
