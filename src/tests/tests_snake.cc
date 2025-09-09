#include "tests_entry.h"

TEST(SnakeGameFSM, GameInit) {
  GameInfo_t info = MainFsmFunction(SUCCESS, Start);
  EXPECT_TRUE(info.field != nullptr);
  EXPECT_TRUE(info.next == nullptr);
  EXPECT_EQ(info.score, STANDART_SIZE);
  EXPECT_TRUE(info.high_score >= info.score);
  EXPECT_EQ(info.speed, 0);
  EXPECT_EQ(info.level, 0);
  EXPECT_EQ(info.pause, 1);
}

TEST(SnakeGameFSM, SpawnSnake) {
  GameInfo_t info = MainFsmFunction(NO, Start);
  EXPECT_TRUE(info.field[10][5]);
  EXPECT_TRUE(info.field[11][5]);
  EXPECT_TRUE(info.field[12][5]);
  EXPECT_TRUE(info.field[13][5]);
}

TEST(SnakeGameFSM, SpawnApple) {
  GameInfo_t info = MainFsmFunction(NO, Start);
  bool isApple = false;
  for (int i{BUFFER}; i < HIGH_ACT - BUFFER; ++i) {
    for (int j{BUFFER}; j < WIDE_ACT - BUFFER; ++j) {
      if (info.field[i][j] == 1) isApple = true;
    }
  }
  EXPECT_TRUE(isApple);
}

TEST(SnakeGameFSM, Shifting) {
  using namespace std::chrono_literals;
  MainFsmFunction(NO, Start);
  MainFsmFunction(NO, Start);
  std::this_thread::sleep_for(300ms);
  MainFsmFunction(NO, Start);
  GameInfo_t info = MainFsmFunction(NO, Start);
  EXPECT_TRUE(info.field[9][5]);
  EXPECT_TRUE(info.field[10][5]);
  EXPECT_TRUE(info.field[11][5]);
  EXPECT_TRUE(info.field[12][5]);
}

TEST(SnakeGameFSM, MovingLeft) {
  using namespace std::chrono_literals;
  std::this_thread::sleep_for(300ms);
  MainFsmFunction(NO, Left);
  GameInfo_t info = MainFsmFunction(NO, Start);
  EXPECT_TRUE(info.field[9][4]);
  EXPECT_TRUE(info.field[9][5]);
  EXPECT_TRUE(info.field[10][5]);
  EXPECT_TRUE(info.field[11][5]);
}

TEST(SnakeGameFSM, MovingUp) {
  using namespace std::chrono_literals;
  std::this_thread::sleep_for(300ms);
  MainFsmFunction(NO, Up);
  GameInfo_t info = MainFsmFunction(NO, Start);
  EXPECT_TRUE(info.field[8][4]);
  EXPECT_TRUE(info.field[9][4]);
  EXPECT_TRUE(info.field[9][5]);
  EXPECT_TRUE(info.field[10][5]);
}

TEST(SnakeGameFSM, MovingRight) {
  using namespace std::chrono_literals;
  std::this_thread::sleep_for(300ms);
  MainFsmFunction(NO, Right);
  GameInfo_t info = MainFsmFunction(NO, Start);
  EXPECT_TRUE(info.field[8][5]);
  EXPECT_TRUE(info.field[8][4]);
  EXPECT_TRUE(info.field[9][4]);
  EXPECT_TRUE(info.field[9][5]);
}

TEST(SnakeGameFSM, MovingDown) {
  using namespace std::chrono_literals;
  std::this_thread::sleep_for(300ms);
  MainFsmFunction(NO, Down);
  GameInfo_t info = MainFsmFunction(NO, Start);
  EXPECT_TRUE(info.field[9][5]);
  EXPECT_TRUE(info.field[8][5]);
  EXPECT_TRUE(info.field[8][4]);
  EXPECT_TRUE(info.field[9][4]);
}

TEST(SnakeGameFSM, Pause_case) {
  GameInfo_t info = MainFsmFunction(SUCCESS, Start);
  EXPECT_EQ(info.pause, 1);
  MainFsmFunction(NO, Pause);
  using namespace std::chrono_literals;
  std::this_thread::sleep_for(300ms);
  info = MainFsmFunction(NO, Start);
  EXPECT_EQ(info.pause, 0);
  EXPECT_TRUE(info.field[9][5]);
  EXPECT_TRUE(info.field[8][5]);
  EXPECT_TRUE(info.field[8][4]);
  EXPECT_TRUE(info.field[9][4]);
  MainFsmFunction(NO, Pause);
  info = MainFsmFunction(NO, Start);
  EXPECT_EQ(info.pause, 1);
  EXPECT_TRUE(info.field[10][5]);
  EXPECT_TRUE(info.field[9][5]);
  EXPECT_TRUE(info.field[8][5]);
  EXPECT_TRUE(info.field[8][4]);
}

TEST(SnakeGameFSM, MovingAction) {
  using namespace std::chrono_literals;
  std::this_thread::sleep_for(180ms);
  MainFsmFunction(NO, Action);
  GameInfo_t info = MainFsmFunction(NO, Start);
  EXPECT_TRUE(info.field[11][5]);
  EXPECT_TRUE(info.field[10][5]);
  EXPECT_TRUE(info.field[9][5]);
  EXPECT_TRUE(info.field[8][5]);
}

TEST(SnakeGameFSM, GameOver_ESC) {
  GameInfo_t info = MainFsmFunction(NO, Start);
  EXPECT_TRUE(info.field != nullptr);
  info = MainFsmFunction(NO, Terminate);
  EXPECT_TRUE(info.field == nullptr);
}

TEST(SnakeGameOutside, Eating) {
  my::Game game;
  my::GameState state = game.Eating();
  EXPECT_EQ(state, my::GameState::SpawnApple);
  game.RefreshSnake();
  GameInfo_t info = game.GiveGameInfo();
  EXPECT_EQ(5, info.score);
  EXPECT_EQ(1, info.level);
  EXPECT_TRUE(info.field[10][5]);
  EXPECT_TRUE(info.field[11][5]);
  EXPECT_TRUE(info.field[12][5]);
  EXPECT_TRUE(info.field[13][5]);
  EXPECT_TRUE(info.field[14][5]);
}
