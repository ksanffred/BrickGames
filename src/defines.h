#ifndef BRICKGAME_DEFINES_H
#define BRICKGAME_DEFINES_H

#define SUCCESS   1
#define NO        0

#define GAME      1
#define OVER      0

#define EMPTY    0
#define BLOCK    1
#define DROPPED   2

#define WIDE     10
#define WIDE_ACT 12
#define WIDE_F   20
#define HIGH     20
#define HIGH_ACT 22

#define BUFFER 1

typedef enum UserAction_t {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
}UserAction_t;

typedef struct GameInfo_t {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

#endif // BRICKGAME_DEFINES_H
