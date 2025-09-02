#ifndef C7_BRICKGAME_BACK_FSM_TETRIS_H
#define C7_BRICKGAME_BACK_FSM_TETRIS_H

#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>

#include "../../defines.h"

#define I_TETROMINO {{0, 0, 0, 0}, {11, 11, 11, 11}}
#define J_TETROMINO {{0, 12, 0, 0}, {0, 12, 12, 12}}
#define L_TETROMINO {{0, 0, 0, 13}, {0, 13, 13, 13}}
#define O_TETROMINO {{0, 14, 14, 0}, {0, 14, 14, 0}}
#define S_TETROMINO {{0, 0, 15, 15}, {0, 15, 15, 0}}
#define T_TETROMINO {{0, 0, 16, 0}, {0, 16, 16, 16}}
#define Z_TETROMINO {{0, 17, 17, 0}, {0, 0, 17, 17}}

/** Possible states of "finite state machin"(FSM)*/
typedef enum TetrisState_t{
  START = 0,
  SPAWN,
  MOVING,
  SHIFTING,
  ATTACHING,
  GAMEOVER,
} TetrisState_t;

/** Position of the lower right pixel of the tetromino 
 * needed to define the shape in field*/
typedef struct BlockPos_t {
  int x;
  int y;
} BlockPos_t;

/** All needed parametr that are passed to the function of FSM*/
typedef struct Params_t {
  GameInfo_t *main;
  TetrisState_t *state;
  BlockPos_t *pos;
} Params_t;

/** Type for state machine table*/
typedef void (*StatesMachine_t)(Params_t *prms);

#ifdef __cplusplus
extern "C" {
#endif


/** 
 * ==== FUNCTIONS FOR BACKEND ====
 * */


/** Initializes all basic game parameters to GameInfo_t main */
void StatsInit(GameInfo_t *main);

/** Selects the next tetromino */
void MakePiece(int** next);

/** Reads the highest score from file, if file missing, return 0
 * @returns the highest score
*/ 
int HighScoreRead();

/** Writes the highest score to the file, if file missing create new one */
void HighScoreWrite(const Params_t* prms);


bool IsTime(int speed);

/** Checkes if it possible to turn tetromino */
int TurnPossibility(const Params_t * prms, const int temp[4][4], int px, int py);

/** Checks if the tetromino touching lower border or droped tetromino
 * @returns 0 (NO) if is NO touching or
 * @returns 1 (SUCCESS) if is touching
 */
int CheckTouching(const Params_t* prms);

/** Checks if there are filled rows, destruct all of them
 * @returns number of destructed rows
 */
int RowsDestroyer(Params_t* prms);

/** Сhanges the level and spead when the account is increased more than 600
 * Max level 10
 */
void Leveling(Params_t* prms);

/** Checks if the droped tetromino placed in the higher border
 * @returns 0 (NO) if game is NOT over or
 * @returns 1 (SUCCESS) if game is over
 */
int CheckGameOver(const Params_t* prms);


/** 
 * ==== FUNCTIONS FOR FMS ====
 * */


/** Switches the states of the finite machine.
 * Stores information about the current state of the game and other parametrs.
 * @param update if is 0 (NO) just returns current state of the game, if is 1 (SUCCESS) switches FMS state
 * @param action passes user action to the back (FMS)
 * @returns current state of the game
 */
GameInfo_t MainFsmFunction(int update, UserAction_t action);

/** Chooses and spawn next teromino*/
void Spawn(Params_t *prms);

/** If is no other user action, chooses the next state */
void Moving(Params_t* prms);

/** Switches state to the pause in game */
void MovePause(Params_t* prms);

/** Moves tetromino to the left, if it is possible, chooses the next state */
void MovingLeft(Params_t* prms);

/** Moves tetromino to the right, if it is possible, chooses the next state */
void MovingRight(Params_t* prms);

/** Moves tetromino all the way down, if it is possible, chooses the next state */
void MoveDown(Params_t* prms);

/** Turnes tetromino to the right, if it is possible, chooses the next state */
void MovingTurn(Params_t* prms);

/** Shifting tetromino down one line, if it is possible, timer bepends on speed, chooses the next state */
void Shifting(Params_t *prms);

/** Changes active tetromino to the droped, add score, 
 * increases level and speed, update higher score, chooses the next state */
void Attaching(Params_t* prms);

/** End game, saves higher score, free all matrix */
void GameOver(Params_t* prms);

#ifdef __cplusplus
} // End of extern "C"
#endif

#endif // C7_BRICKGAME_BACK_FSM_TETRIS_H