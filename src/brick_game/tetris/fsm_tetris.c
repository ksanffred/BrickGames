#include "back_fsm_tetris.h"

StatesMachine_t fsm_table[6][8] = {
    {Spawn, Spawn, Spawn, Spawn, Spawn, Spawn, Spawn},
    {Spawn, Spawn, Spawn, Spawn, Spawn, Spawn, Spawn, Spawn},
    {Moving, MovePause, Moving, MovingLeft, MovingRight, Moving, MoveDown,
     MovingTurn},
    {Shifting, Shifting, Shifting, Shifting, Shifting, Shifting, Shifting,
     Shifting},
    {Attaching, Attaching, Attaching, Attaching, Attaching, Attaching,
     Attaching, Attaching},
    {GameOver, GameOver, GameOver, GameOver, GameOver, GameOver, GameOver,
     GameOver}};

GameInfo_t MainFsmFunction(int update, UserAction_t action) {
  static GameInfo_t main = {NULL, NULL, 0, 0, 0, 0, 0};
  static bool first = true;
  if (first == true) StatsInit(&main), first = false;

  static UserAction_t action_m = Start;
  if (action != Start) action_m = action;

  if (update == NO) {
    static TetrisState_t state = START;
    static BlockPos_t pos;
    if (action_m == Pause) main.pause = !main.pause;

    StatesMachine_t act = NULL;
    Params_t prms;
    prms.main = &main;
    prms.state = &state;
    prms.pos = &pos;

    if (action_m == Terminate) state = GAMEOVER;

    act = fsm_table[state][action_m];
    if (state == MOVING) action_m = Start;
    if (main.pause || state == GAMEOVER) act(&prms);
  }
  return main;
}

void Spawn(Params_t* prms) {
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 4; j++) {
      prms->main->field[BUFFER + i][3 + BUFFER + j] = prms->main->next[i][j];
    }
  }
  MakePiece(prms->main->next);
  *prms->state = MOVING;
  prms->pos->y = 2 + BUFFER;
  prms->pos->x = 6 + BUFFER;
}

void Moving(Params_t* prms) {
  *prms->state = CheckTouching(prms) == SUCCESS ? ATTACHING : SHIFTING;
}

void MovePause(Params_t* prms) { prms->main->pause = 1; }

void MovingLeft(Params_t* prms) {
  int is_possible = SUCCESS;
  for (int i = prms->pos->y - 3; i <= prms->pos->y; i++) {
    for (int j = prms->pos->x - 3; j <= prms->pos->x; j++) {
      if (prms->main->field[i][j] / 10 == BLOCK &&
          prms->main->field[i][j - 1] / 10 == DROPPED)
        is_possible = NO;
    }
  }
  for (int i = BUFFER; i < HIGH_ACT - BUFFER; i++) {
    if (prms->main->field[i][BUFFER] / 10 == BLOCK) is_possible = NO;
  }
  if (is_possible == SUCCESS) {
    for (int i = prms->pos->y - 3; i <= prms->pos->y; i++) {
      for (int j = prms->pos->x - 3; j <= prms->pos->x; j++) {
        if (prms->main->field[i][j] / 10 == BLOCK) {
          prms->main->field[i][j - 1] = prms->main->field[i][j];
          prms->main->field[i][j] = EMPTY;
        }
      }
    }
    prms->pos->x--;
  }
  *prms->state = CheckTouching(prms) == SUCCESS ? ATTACHING : SHIFTING;
}

void MovingRight(Params_t* prms) {
  int is_possible = SUCCESS;
  for (int i = prms->pos->y; i >= prms->pos->y - 3; i--) {
    for (int j = prms->pos->x; j >= prms->pos->x - 3; j--) {
      if (prms->main->field[i][j] / 10 == BLOCK &&
          prms->main->field[i][j + 1] / 10 == DROPPED)
        is_possible = NO;
    }
  }
  for (int i = BUFFER; i < HIGH_ACT - BUFFER; i++) {
    if (prms->main->field[i][WIDE_ACT - BUFFER - 1] / 10 == BLOCK)
      is_possible = NO;
  }
  if (is_possible == SUCCESS) {
    for (int i = prms->pos->y; i >= prms->pos->y - 3; i--) {
      for (int j = prms->pos->x; j >= prms->pos->x - 3; j--) {
        if (prms->main->field[i][j] / 10 == BLOCK) {
          prms->main->field[i][j + 1] = prms->main->field[i][j];
          prms->main->field[i][j] = EMPTY;
        }
      }
    }
    prms->pos->x++;
  }
  *prms->state = CheckTouching(prms) == SUCCESS ? ATTACHING : SHIFTING;
}

void MoveDown(Params_t* prms) {
  int min = HIGH_ACT;
  for (int i = prms->pos->y; i >= prms->pos->y - 3; i--) {
    for (int j = prms->pos->x; j >= prms->pos->x - 3; j--) {
      if (prms->main->field[i][j] / 10 == BLOCK) {
        int k = 0;
        while (prms->main->field[i + k + 1][j] / 10 != DROPPED &&
               i + k != HIGH_ACT - BUFFER * 2)
          k++;
        if (k < min) min = k;
      }
    }
  }
  if (min > 0) {
    for (int i = prms->pos->y; i >= prms->pos->y - 3; i--) {
      for (int j = prms->pos->x; j >= prms->pos->x - 3; j--) {
        if (prms->main->field[i][j] / 10 == BLOCK) {
          prms->main->field[i + min][j] = prms->main->field[i][j];
          prms->main->field[i][j] = EMPTY;
        }
      }
    }
  }
  prms->pos->y += min;
  *prms->state = CheckTouching(prms) == SUCCESS ? ATTACHING : MOVING;
}

void MovingTurn(Params_t* prms) {
  int py = prms->pos->y - 1;
  int px = prms->pos->x - 1;

  int temp[4][4];
  for (int i = prms->pos->y, k = 3; i >= prms->pos->y - 3; i--, k--) {
    for (int j = prms->pos->x, m = 3; j >= prms->pos->x - 3; j--, m--) {
      temp[k][m] = prms->main->field[i][j];
    }
  }
  if (TurnPossibility(prms, temp, px, py) == SUCCESS) {
    for (int i = prms->pos->y; i >= prms->pos->y - 3; i--) {
      for (int j = prms->pos->x; j >= prms->pos->x - 3; j--) {
        if (prms->main->field[i][j] / 10 == BLOCK)
          prms->main->field[i][j] = EMPTY;
      }
    }
    for (int i = prms->pos->y, k = 3; i >= prms->pos->y - 3; i--, k--) {
      for (int j = prms->pos->x, m = 3; j >= prms->pos->x - 3; j--, m--) {
        if (temp[k][m] / 10 == BLOCK) {
          int x2 = py + px - i;
          if (x2 > prms->pos->x) x2 -= 4;
          int y2 = j + py - px;
          prms->main->field[y2][x2] = temp[k][m];
        }
      }
    }
  }
  *prms->state = CheckTouching(prms) == SUCCESS ? ATTACHING : SHIFTING;
}

void Shifting(Params_t* prms) {
  if (IsTime(prms->main->speed)) {
    for (int i = prms->pos->y; i >= prms->pos->y - 3; i--) {
      for (int j = prms->pos->x; j >= prms->pos->x - 3; j--) {
        if (prms->main->field[i][j] / 10 == BLOCK) {
          prms->main->field[i + 1][j] = prms->main->field[i][j];
          prms->main->field[i][j] = EMPTY;
        }
      }
    }
    prms->pos->y++;
  }
  *prms->state = CheckTouching(prms) == SUCCESS ? ATTACHING : MOVING;
}

void Attaching(Params_t* prms) {
  for (int i = prms->pos->y; i >= prms->pos->y - 3; i--) {
    for (int j = prms->pos->x; j >= prms->pos->x - 3; j--) {
      if (prms->main->field[i][j] / 10 == BLOCK) {
        prms->main->field[i][j] = DROPPED * 10 + prms->main->field[i][j] % 10;
      }
    }
  }
  switch (RowsDestroyer(prms)) {
    case 1:
      prms->main->score += 100;
      break;
    case 2:
      prms->main->score += 300;
      break;
    case 3:
      prms->main->score += 700;
      break;
    case 4:
      prms->main->score += 1500;
      break;
    default:
      break;
  }
  if (prms->main->score > prms->main->high_score)
    prms->main->high_score = prms->main->score;
  Leveling(prms);
  *prms->state = CheckGameOver(prms) == SUCCESS ? GAMEOVER : SPAWN;
}

void GameOver(Params_t* prms) {
  HighScoreWrite(prms);
  free(prms->main->field);
  prms->main->field = NULL;
  free(prms->main->next);
  prms->main->next = NULL;
}
