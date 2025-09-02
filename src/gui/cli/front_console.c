#include "front_console.h"

UserAction_t GetSignal() {
  UserAction_t action;
  timeout(100);
  switch (getch()) {
    case ENTER_KEY:
      action = Start;
      break;
    case PAUSE:
      action = Pause;
      break;
    case ESCAPE:
      action = Terminate;
      break;
    case KEY_LEFT:
      action = Left;
      break;
    case KEY_RIGHT:
      action = Right;
      break;
    case KEY_UP:
      action = Up;
      break;
    case KEY_DOWN:
      action = Down;
      break;
    case SPACE:
      action = Action;
      break;
    default:
      action = Start;
  }
  return action;
}

void Overlay() {
  initscr();
  noecho();
  cbreak();
  keypad(stdscr, TRUE);
  curs_set(0);
  start_color();
  init_color(COLOR_WHITE, 950, 950, 950);
  init_pair(10, COLOR_BLACK, COLOR_WHITE);
  bkgd(COLOR_PAIR(10));

  AddRectangle(0, 0, HUD_WIDE - 1, HUD_HIGH - 1);
  AddRectangle(1, 1, WIDE_F + 2, HIGH + 2);

  FPRINT(10, 5, "Press ENTER");
  FPRINT(11, 5, " to start!");

  SPRINT(1, 0, "    SCORE");
  SPRINT(4, 0, " HI-SCORE");
  SPRINT(7, 0, "    LEVEL");
  SPRINT(10, 0, "    SPEED");
  SPRINT(13, 0, "     NEXT");
  AddRectangle(SIDEBAR_BEGIN, 15, SIDEBAR_BEGIN + 9, 18);
}

int Front(GameInfo_t main) {
  int ret = GAME;
  init_pair(1, COLOR_WHITE, COLOR_RED);
  init_pair(2, COLOR_WHITE, COLOR_MAGENTA);
  init_pair(3, COLOR_WHITE, COLOR_YELLOW);
  init_pair(4, COLOR_WHITE, COLOR_BLUE);
  init_pair(5, COLOR_WHITE, COLOR_GREEN);
  init_pair(6, COLOR_WHITE, COLOR_BLUE);
  init_pair(7, COLOR_WHITE, COLOR_CYAN);

  if (main.field != NULL) {
    PrintField(main);
    PrintHUD(main);
  } else {
    ret = OVER;
    for (int i = BUFFER; i < HIGH_ACT - BUFFER; i++) {
      int k = BUFFER;
      for (int j = BUFFER; j < WIDE_ACT - BUFFER; j++, k += 2) {
        FPRINT(i, k, "  ");
      }
    }
    FPRINT(10, 5, "Game Over");
  }
  return ret;
}

void PrintHUD(GameInfo_t main) {
  SPRINT(2, 3, "%.6d", main.score);
  SPRINT(5, 3, "%.6d", main.high_score);
  SPRINT(8, 3, "%.6d", main.level);
  SPRINT(11, 3, "%.6d", main.speed);

  if (!main.pause) {
    attron(COLOR_PAIR(6));
    SPRINT(19, 2, " PAUSE ");
    attroff(COLOR_PAIR(6));
  } else
    SPRINT(19, 2, "       ");

  if (main.next != NULL) {
    for (int i = 0; i < 2; i++) {
      for (int j = 0, k = 1; j < 4; j++, k += 2) {
        int type = main.next[i][j] % 10;
        if (type != 0) {
          attron(COLOR_PAIR(type));
          SPRINT(i + 15, k, "[]");
          attroff(COLOR_PAIR(type));
        } else
          SPRINT(i + 15, k, "  ");
      }
    }
  }
}

void PrintField(GameInfo_t main) {
  for (int i = BUFFER; i < HIGH_ACT - BUFFER; i++) {
    for (int j = BUFFER, k = BUFFER; j < WIDE_ACT - BUFFER; j++, k += 2) {
      int type = main.field[i][j] % 10;
      if (type != 0) {
        attron(COLOR_PAIR(type));
        FPRINT(i, k, "[]");
        attroff(COLOR_PAIR(type));
      } else
        FPRINT(i, k, "  ");
    }
  }
}

void AddRectangle(int x1, int y1, int x2, int y2) {
  mvaddch(y1, x1, ACS_ULCORNER);
  mvaddch(y1, x2, ACS_URCORNER);
  mvaddch(y2, x1, ACS_LLCORNER);
  mvaddch(y2, x2, ACS_LRCORNER);
  for (int i = x1 + 1; i < x2; i++) {
    mvaddch(y1, i, ACS_HLINE);
    mvaddch(y2, i, ACS_HLINE);
  }
  for (int i = y1 + 1; i < y2; i++) {
    mvaddch(i, x1, ACS_VLINE);
    mvaddch(i, x2, ACS_VLINE);
  }
}