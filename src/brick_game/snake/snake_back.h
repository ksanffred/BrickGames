#ifndef SNAKE_BACK_H
#define SNAKE_BACK_H

#include <random>
#include <chrono>
#include <queue>
#include <fstream>
#include <string>

#include "../../defines.h"

namespace my {

#define STANDART_SIZE 4
/* Parts of snake */
enum class BodyPart {Head, Tail, Extra};
/* Snake possible movement direction */
enum class Direction {Left, Right, Up, Down};
/* Possible speed situations */
enum class SpeedType {Get, Increase, NitroON, NitroOFF};
/* States of the Game according to FSM */
enum class GameState {Start, SpawnSnake, SpawnApple, Moving, Eating, Shifting, Attaching, GameOver};

/* Main SnakeGame class */
class Game {
 private:
  class Snake {
   public:
    class Body {
     public:
      int x_;
      int y_;
      BodyPart part_;

     public:
      Body();
      Body(int x, int y);
      Body(int x, int y, BodyPart part);
    };
    
   public:
    std::vector<Body> full_snake_;
    Direction dir_;

   public:
    Snake();

   public:
    void MoveForward();
    void IncreaseLength();
    int GiveSnakeHeadX();
    int GiveSnakeHeadY();
    bool IsHeadOnExtra();
    void SetDirection(Direction dir);
    int SnakeSize();
  };

 private:
  class Apple {
   public:
    int x_;
    int y_;

   public:
    Apple();

   public:
    void GenerateApple();
    int GiveAppleX();
    int GiveAppleY();

  };
 private:
  class Field {
   public:
    GameInfo_t main_;

   public:
    Field();
    ~Field();

   public:
    void ClearField();
    void SetFieldXY(int x, int y, int value);
    int HighScoreRead();
    void HighScoreWrite();

  };

 private:
  Snake snake_;
  Apple apple_;
  Field field_;

 public:
  Game() = default;

 public:
  GameState SpawnSnake();
  GameState SpawnApple();
  GameState Moving(const UserAction_t& action);
  GameState Eating();
  GameState Shifting();
  void GameOver();

 public: 
  GameInfo_t GiveGameInfo();
  /** Changes snake direction if possible, keep next direction 
   * @returns next direction
   */
  Direction ChangeDirection(const UserAction_t& action);
  /* Checks is Apple x & y is snake boby part x & y */
  bool IsAppleOnSnake();
  /* Сhecks if enough time has passed before the next shift */
  bool IsTime();
  /* Renew snake possition on field according to snake x & y */  
  void RefreshSnake();
  void IncreaseScore();
  void Leveling();
  /** Store actual snake speed, increase speed by level increase,
   * increase speed by pussing Action button
   * @param type choose possible way to interact with speed from enum
   * @returns current speed
   */
  int Speed(SpeedType type);
  bool Pause();
  /* Checks if snake's head touch another snake's part */
  bool SnakeTouchSnake();
  /* Checks if snake's head touch board */
  bool SnakeTouchBoard();
  /* Checks if snake's head touch another snake's part or board*/
  bool IsTouching();
  /* Checks if it is 200 score */
  bool IsWin();

};

} // namespace my

/** Switches the states of the finite machine.
 * Stores information about the current state of the game and other parametrs.
 * @param update if is 1 (SUCCESS) just returns current state of the game, if is 0 (NO) switches FMS state
 * @param action passes user action to the back (FMS)
 * @returns current state of the game
 */
GameInfo_t MainFsmFunction(int update, UserAction_t action);

#endif // SNAKE_BACK_H
