#include "back_fsm_tetris.h"

// void userInput(UserAction_t action, bool hold) {
//   bool __attribute__((unused)) fake_hold = hold;
//   MainFsmFunction(NO, action);
// }

// GameInfo_t updateCurrentState() { return MainFsmFunction(SUCCESS, Start); }

void StatsInit(GameInfo_t* main) {
  main->field =
      malloc(HIGH_ACT * sizeof(int*) + HIGH_ACT * WIDE_ACT * sizeof(int));
  int* ptrf = (int*)(main->field + HIGH_ACT);
  for (int i = 0; i < HIGH_ACT; i++) {
    main->field[i] = ptrf + WIDE_ACT * i;
  }
  for (int i = 0; i < HIGH_ACT; i++) {
    for (int j = 0; j < WIDE_ACT; j++) {
      main->field[i][j] = EMPTY;
    }
  }
  main->next = malloc(2 * sizeof(int*) + 2 * 4 * sizeof(int));
  int* ptrn = (int*)(main->next + 2);
  for (int i = 0; i < 2; i++) {
    main->next[i] = ptrn + 4 * i;
  }
  MakePiece(main->next);
  main->high_score = HighScoreRead();
  main->score = 0;
  main->level = 0;
  main->speed = 0;
  main->pause = 1;
}

void MakePiece(int** next) {
  const int pieces[7][2][4] = {I_TETROMINO, J_TETROMINO, L_TETROMINO,
                               O_TETROMINO, S_TETROMINO, T_TETROMINO,
                               Z_TETROMINO};
  int num = rand() % 7;
  static int last = 8;
  if (num == last) num = rand() % 7;
  last = num;
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 4; j++) {
      next[i][j] = pieces[num][i][j];
    }
  }
}

int HighScoreRead() {
  int hi_score = 0;
  FILE* fp = fopen("high_score.txt", "r");
  if (fp) {
    char c;
    int i = 0;
    while ((c = getc(fp)) != EOF) {
      if (i != 0) hi_score *= 10;
      hi_score += c - 48;
      i++;
    }
    fclose(fp);
  }
  return hi_score;
}

void HighScoreWrite(const Params_t* prms) {
  FILE* fp = fopen("high_score.txt", "w");
  if (fp) {
    char hi_score[10];
    sprintf(hi_score, "%d", prms->main->high_score);
    fputs(hi_score, fp);
    fclose(fp);
  }
}

bool IsTime(int speed) {
  static struct timeval start_time;
  static bool first_call = true;
  if (first_call) {
    gettimeofday(&start_time, NULL);
    first_call = false;
  }

  struct timeval current_time;
  gettimeofday(&current_time, NULL);

  long elapsed_micros = (current_time.tv_sec - start_time.tv_sec) * 1000000 +
                        (current_time.tv_usec - start_time.tv_usec);

  const int base_speed_ms = 700;
  const int speed_factor_ms = 60;
  long required_micros = (base_speed_ms - speed_factor_ms * speed) * 1000;

  bool result = false;
  if (elapsed_micros > required_micros) {
    result = true;
    gettimeofday(&start_time, NULL);
  }
  return result;
}

int TurnPossibility(const Params_t* prms, const int temp[4][4], int px,
                    int py) {
  int is_possible = SUCCESS;

  if (px == BUFFER || px == WIDE_ACT - BUFFER * 2) is_possible = NO;
  if (temp[1][1] % 10 == 4) is_possible = NO;
  int k = 3;
  for (int i = prms->pos->y; i >= prms->pos->y - 3; i--) {
    int m = 3;
    for (int j = prms->pos->x; j >= prms->pos->x - 3; j--) {
      if (temp[k][m] / 10 == BLOCK) {
        int x2 = py + px - i;
        if (x2 > prms->pos->x) x2 -= 4;
        int y2 = j + py - px;
        if (prms->main->field[y2][x2] / 10 == DROPPED || x2 == 0)
          is_possible = NO;
      }
      m--;
    }
    k--;
  }
  return is_possible;
}

int CheckTouching(const Params_t* prms) {
  int ret = NO;
  for (int i = prms->pos->y; i >= prms->pos->y - 3; i--) {
    for (int j = prms->pos->x; j >= prms->pos->x - 3; j--) {
      if (prms->main->field[i][j] / 10 == BLOCK) {
        if (i == HIGH_ACT - BUFFER * 2 ||
            prms->main->field[i + 1][j] / 10 == DROPPED)
          ret = SUCCESS;
      }
    }
  }
  return ret;
}

int RowsDestroyer(Params_t* prms) {
  int how_many_rows = 0;
  for (int i = HIGH_ACT - BUFFER; i >= BUFFER; i--) {
    int is_full = SUCCESS;
    for (int j = BUFFER; j < WIDE_ACT - BUFFER; j++) {
      if (prms->main->field[i][j] / 10 != DROPPED) is_full = NO;
    }
    if (is_full == SUCCESS) {
      for (int j = BUFFER; j < WIDE_ACT - BUFFER; j++) {
        prms->main->field[i][j] = EMPTY;
      }
      for (int k = i - 1; k > BUFFER; k--) {
        for (int m = BUFFER; m < WIDE_ACT; m++) {
          prms->main->field[k + 1][m] = prms->main->field[k][m];
        }
      }
      how_many_rows++;
      i++;
    }
  }
  return how_many_rows;
}

void Leveling(Params_t* prms) {
  while (prms->main->score / 600 != prms->main->level &&
         prms->main->level < 10) {
    prms->main->level++;
    prms->main->speed++;
  }
}

int CheckGameOver(const Params_t* prms) {
  int ret = NO;
  for (int j = BUFFER; j < WIDE_ACT - BUFFER; j++) {
    if (prms->main->field[BUFFER][j] / 10 == DROPPED) ret = SUCCESS;
  }
  return ret;
}