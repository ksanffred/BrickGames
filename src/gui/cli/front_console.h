#ifndef CPP3_BRICKGAME_FRONT_SNAKE_H
#define CPP3_BRICKGAME_FRONT_SNAKE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <ncurses.h>

#include "../../defines.h"

#define HUD_WIDE     36
#define HUD_HIGH     24

#define BOARDS_BEGIN   2-BUFFER
#define SIDEBAR_BEGIN  WIDE_F+4

#define FPRINT(y, x, ...) mvprintw(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), __VA_ARGS__)
#define FADDCH(y, x, c) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), c)
#define SPRINT(y, x, ...) mvprintw(BOARDS_BEGIN + (y), SIDEBAR_BEGIN + (x), __VA_ARGS__)
#define SADDCH(y, x, c) mvaddch(BOARDS_BEGIN + (y), SIDEBAR_BEGIN + (x), c)

#define ENTER_KEY   10
#define PAUSE       112
#define ESCAPE      27
#define SPACE       32


/** 
 * ==== FUNCTIONS FOR FRONTEND ====
 * */

/** Accepts user input and returns one of 8 possible buttons
 * @return structure with user input
*/
UserAction_t GetSignal();

/** Print console interface*/
void Overlay();

/** Prints field and HUD with all information adout game
 * @return 0 (OVER) if is nothing to print in field (game over)
*/
int Front(GameInfo_t main);

/** Print HUD of the game */
void PrintHUD(GameInfo_t main);

/** Print gaming field */
void PrintField(GameInfo_t main);

/** Print a rectangle with the coordinates x1, y1 in the left top corner
 * and x2, y2 in the right bottom corner
 */
void AddRectangle(int x1, int y1, int x2, int y2);

#ifdef __cplusplus
}
#endif


#endif // CPP3_BRICKGAME_FRONT_SNAKE_H