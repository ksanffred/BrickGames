#include <check.h>
#include <time.h>

#include "../brick_game/tetris/back_fsm_tetris.h"
#include "../gui/cli/controller.h"

START_TEST(START_stats_init_file) {
  userInput(Start, false);
  GameInfo_t main = updateCurrentState();

  ck_assert_ptr_ne(main.field, NULL);
  ck_assert_ptr_ne(main.next, NULL);
  ck_assert_int_eq(main.score, 0);
  ck_assert_int_eq(main.high_score, 6600);
  ck_assert_int_eq(main.level, 0);
  ck_assert_int_eq(main.speed, 0);
  ck_assert_int_eq(main.pause, 1);

  userInput(Terminate, false);
}
END_TEST

START_TEST(SPAWN_first) {
  userInput(Start, false);
  GameInfo_t main = updateCurrentState();
  int test = 0;
  for (int i = BUFFER; i < 2 + BUFFER; i++) {
    for (int j = BUFFER; j < WIDE_ACT - BUFFER; j++) {
      test += main.field[i][j] / 10;
    }
  }
  ck_assert_int_eq(test, 4);

  userInput(Terminate, false);
}
END_TEST

START_TEST(MOVING_PAUSE) {
  userInput(Start, false);
  GameInfo_t main = updateCurrentState();

  userInput(Pause, false);
  main = updateCurrentState();
  ck_assert_int_eq(main.pause, 0);

  userInput(Pause, false);
  main = updateCurrentState();
  ck_assert_int_eq(main.pause, 1);

  userInput(Terminate, false);
}
END_TEST

START_TEST(MOVING_GAMEOVER_terminate) {
  userInput(Start, false);
  GameInfo_t main = updateCurrentState();
  userInput(Terminate, false);
  main = updateCurrentState();
  ck_assert_ptr_eq(main.field, NULL);
  ck_assert_ptr_eq(main.next, NULL);
}
END_TEST

START_TEST(MOVING_SHIFTING) {
  userInput(Start, false);
  GameInfo_t main = updateCurrentState();
  userInput(Start, false);
  while (clock() / 4000 < 1);
  userInput(Start, false);
  main = updateCurrentState();
  int test = 0;
  for (int i = BUFFER + 1; i < 2 + BUFFER + 1; i++) {
    for (int j = BUFFER; j < WIDE_ACT - BUFFER; j++) {
      test += main.field[i][j] / 10;
    }
  }
  ck_assert_int_eq(test, 4);

  userInput(Terminate, false);
}
END_TEST

START_TEST(MOVING_LEFT) {
  userInput(Start, false);
  GameInfo_t main = updateCurrentState();
  int leftmost_spawn = WIDE_ACT;
  int rightmost_spawn = 0;
  for (int i = BUFFER; i < 2 + BUFFER; i++) {
    for (int j = BUFFER; j < WIDE_ACT - BUFFER; j++) {
      if (main.field[i][j] / 10 == BLOCK) {
        leftmost_spawn = j < leftmost_spawn ? j : leftmost_spawn;
        rightmost_spawn = j > rightmost_spawn ? j : rightmost_spawn;
      }
    }
  }
  userInput(Left, false);
  main = updateCurrentState();
  int leftmost_moving = WIDE_ACT;
  int rightmost_moving = 0;
  for (int i = BUFFER; i < 2 + BUFFER; i++) {
    for (int j = BUFFER; j < WIDE_ACT - BUFFER; j++) {
      if (main.field[i][j] / 10 == BLOCK) {
        leftmost_moving = j < leftmost_moving ? j : leftmost_moving;
        rightmost_moving = j > rightmost_moving ? j : rightmost_moving;
      }
    }
  }
  ck_assert_int_lt(leftmost_moving, leftmost_spawn);
  ck_assert_int_lt(rightmost_moving, rightmost_spawn);

  userInput(Terminate, false);
}
END_TEST

START_TEST(MOVING_RIGHT) {
  userInput(Start, false);
  GameInfo_t main = updateCurrentState();
  int leftmost_spawn = WIDE_ACT;
  int rightmost_spawn = 0;
  for (int i = BUFFER; i < 2 + BUFFER; i++) {
    for (int j = BUFFER; j < WIDE_ACT - BUFFER; j++) {
      if (main.field[i][j] / 10 == BLOCK) {
        leftmost_spawn = j < leftmost_spawn ? j : leftmost_spawn;
        rightmost_spawn = j > rightmost_spawn ? j : rightmost_spawn;
      }
    }
  }
  userInput(Right, false);
  main = updateCurrentState();
  int leftmost_moving = WIDE_ACT;
  int rightmost_moving = 0;
  for (int i = BUFFER; i < 2 + BUFFER; i++) {
    for (int j = BUFFER; j < WIDE_ACT - BUFFER; j++) {
      if (main.field[i][j] / 10 == BLOCK) {
        leftmost_moving = j < leftmost_moving ? j : leftmost_moving;
        rightmost_moving = j > rightmost_moving ? j : rightmost_moving;
      }
    }
  }
  ck_assert_int_gt(leftmost_moving, leftmost_spawn);
  ck_assert_int_gt(rightmost_moving, rightmost_spawn);

  userInput(Terminate, false);
}
END_TEST

START_TEST(MOVING_DOWN) {
  userInput(Start, false);
  GameInfo_t main = updateCurrentState();
  int is_blocks_top_spawn = 0;
  for (int i = BUFFER; i < 2 + BUFFER; i++) {
    for (int j = BUFFER; j < WIDE_ACT - BUFFER; j++) {
      is_blocks_top_spawn += main.field[i][j] / 10;
    }
  }
  int is_blocks_bottom_spawn = 0;
  for (int i = HIGH_ACT - BUFFER * 2; i > HIGH_ACT - BUFFER * 2 - 2; i--) {
    for (int j = BUFFER; j < WIDE_ACT - BUFFER; j++) {
      is_blocks_bottom_spawn += main.field[i][j] / 10;
    }
  }
  userInput(Down, false);
  main = updateCurrentState();
  int is_blocks_top_down = 0;
  for (int i = BUFFER; i < 2 + BUFFER; i++) {
    for (int j = BUFFER; j < WIDE_ACT - BUFFER; j++) {
      is_blocks_top_down += main.field[i][j] / 10;
    }
  }
  int is_blocks_bottom_down = 0;
  for (int i = HIGH_ACT - BUFFER * 2; i >= HIGH_ACT - BUFFER * 2 - 2; i--) {
    for (int j = BUFFER; j < WIDE_ACT - BUFFER; j++) {
      is_blocks_bottom_down += main.field[i][j] / 10;
    }
  }
  ck_assert_int_eq(is_blocks_top_spawn, 4);
  ck_assert_int_eq(is_blocks_bottom_spawn, 0);
  ck_assert_int_eq(is_blocks_top_down, 0);
  ck_assert_int_eq(is_blocks_bottom_down, 4);

  userInput(Terminate, false);
}
END_TEST

START_TEST(MOVING_TURN) {
  userInput(Start, false);
  GameInfo_t main = updateCurrentState();
  int is_blocks_spawn = 0;
  for (int i = 0; i < 4; i++) {
    for (int j = BUFFER; j < WIDE_ACT - BUFFER; j++) {
      is_blocks_spawn += main.field[i][j] / 10;
    }
  }
  userInput(Action, false);
  main = updateCurrentState();
  int is_blocks_turn = 0;
  for (int i = 0; i < 4; i++) {
    for (int j = BUFFER; j < WIDE_ACT - BUFFER; j++) {
      is_blocks_turn += main.field[i][j] / 10;
    }
  }
  ck_assert_int_eq(is_blocks_spawn, is_blocks_turn);

  userInput(Terminate, false);
}
END_TEST

START_TEST(Attaching_DOWN) {
  userInput(Start, false);
  GameInfo_t main = updateCurrentState();
  userInput(Down, false);
  userInput(Start, false);
  main = updateCurrentState();
  int is_blocks_bottom = 0;
  int is_dropped = 0;
  for (int i = HIGH_ACT - BUFFER * 2; i > HIGH_ACT - BUFFER * 2 - 2; i--) {
    for (int j = BUFFER; j < WIDE_ACT - BUFFER; j++) {
      is_blocks_bottom += main.field[i][j] / 10 != 0 ? 1 : 0;
      is_dropped = main.field[i][j] / 10 > is_dropped ? main.field[i][j] / 10
                                                      : is_dropped;
    }
  }
  ck_assert_int_eq(is_blocks_bottom, 4);
  ck_assert_int_eq(is_dropped, DROPPED);
  userInput(Terminate, false);
}
END_TEST

START_TEST(RowsDestroy_artificial) {
  userInput(Start, false);
  GameInfo_t main = updateCurrentState();
  for (int i = HIGH_ACT - BUFFER * 2; i > HIGH_ACT - BUFFER * 2 - 2; i--) {
    for (int j = BUFFER; j < WIDE_ACT - BUFFER; j++) {
      main.field[i][j] = 21;
    }
  }
  Params_t prms;
  prms.main = &main;
  ck_assert_int_eq(RowsDestroyer(&prms), 2);
  int is_full = 0;
  for (int i = HIGH_ACT - BUFFER * 2; i > HIGH_ACT - BUFFER * 2 - 2; i--) {
    for (int j = BUFFER; j < WIDE_ACT - BUFFER; j++) {
      if (main.field[i][j] == 21) is_full = 1;
    }
  }
  ck_assert_int_eq(is_full, EMPTY);
  userInput(Terminate, false);
}
END_TEST

START_TEST(Attaching_artificial) {
  userInput(Start, false);
  GameInfo_t main = updateCurrentState();
  for (int i = HIGH_ACT - BUFFER * 2; i > HIGH_ACT - BUFFER * 2 - 4; i--) {
    for (int j = BUFFER; j < WIDE_ACT - BUFFER; j++) {
      main.field[i][j] = 21;
    }
  }
  Params_t prms;
  BlockPos_t pos;
  TetrisState_t state = START;
  pos.x = 6;
  pos.y = 6;
  prms.main = &main;
  prms.pos = &pos;
  prms.state = &state;
  Attaching(&prms);

  int is_full = 0;
  for (int i = HIGH_ACT - BUFFER * 2; i > HIGH_ACT - BUFFER * 2 - 4; i--) {
    for (int j = BUFFER; j < WIDE_ACT - BUFFER; j++) {
      if (main.field[i][j] == 21) is_full = 1;
    }
  }
  ck_assert_int_eq(is_full, EMPTY);
  ck_assert_int_eq(main.level, 2);
  ck_assert_int_eq(main.speed, 2);
  userInput(Terminate, false);
}
END_TEST

START_TEST(Leveling_artificial) {
  userInput(Start, false);
  GameInfo_t main = updateCurrentState();
  main.score += 1500;
  Params_t prms;
  prms.main = &main;
  Leveling(&prms);
  ck_assert_int_eq(main.level, 2);
  ck_assert_int_eq(main.speed, 2);
  userInput(Terminate, false);
}
END_TEST

Suite *root_suite(void) {
  Suite *t = suite_create("tetris");

  TCase *base;

  base = tcase_create("base_tests_TCase");
  tcase_add_test(base, START_stats_init_file);
  tcase_add_test(base, SPAWN_first);
  tcase_add_test(base, MOVING_PAUSE);
  tcase_add_test(base, MOVING_GAMEOVER_terminate);
  tcase_add_test(base, MOVING_SHIFTING);
  tcase_add_test(base, MOVING_LEFT);
  tcase_add_test(base, MOVING_RIGHT);
  tcase_add_test(base, MOVING_DOWN);
  tcase_add_test(base, MOVING_TURN);
  tcase_add_test(base, Attaching_DOWN);
  tcase_add_test(base, RowsDestroy_artificial);
  tcase_add_test(base, Attaching_artificial);
  tcase_add_test(base, Leveling_artificial);

  suite_add_tcase(t, base);

  return t;
}

int main(void) {
  int failed = 0;
  Suite *s = root_suite();
  SRunner *runner = srunner_create(s);

  srunner_run_all(runner, CK_NORMAL);
  failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return (failed == 0) ? 0 : -1;
}
