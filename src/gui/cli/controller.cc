#include "controller.h"

int main() {
  Overlay();
  GameLoop();
  refresh();
  endwin();
  return OVER;
}

void GameLoop() {
  if (getch() == ENTER_KEY) {
    int game_over = GAME;
    while (game_over) {
      userInput(GetSignal(), false);
      game_over = Front(updateCurrentState());
    }
  }
  timeout(2100);
  getch();
}

void userInput(UserAction_t action, bool hold) {
  bool __attribute__((unused)) fake_hold = hold;
  MainFsmFunction(SUCCESS, action);
}

GameInfo_t updateCurrentState() { return MainFsmFunction(NO, Start); }
