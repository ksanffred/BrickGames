#ifndef CONROLLER_H
#define CONROLLER_H

#include "../../defines.h"
#include "front_console.h"

#ifdef BUILD_SNAKE
#include "../../brick_game/snake/snake_back.h"
#elif defined(BUILD_TETRIS)
#include "../../brick_game/tetris/back_fsm_tetris.h"
#endif

void GameLoop(); 

/** Takes the user action and hold and passes to the back (tast requirement function). */
void userInput(UserAction_t action, bool hold);

/** (tast requirement function). 
 * @returns Structure containing information about the current state of the game 
 * */
GameInfo_t updateCurrentState();

#endif // CONROLLER_H
