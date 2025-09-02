#include "snake_back.h"

using my::Game;
using my::GameState;

Game::Snake::Body::Body() : x_(5), y_(10), part_(BodyPart::Head) {}

Game::Snake::Body::Body(int x, int y) : x_(x), y_(y), part_(BodyPart::Tail) {}

Game::Snake::Body::Body(int x, int y, BodyPart part)
    : x_(x), y_(y), part_(part) {}

Game::Snake::Snake() {
  Body head;
  Body second{head.x_, head.y_ + 1, BodyPart::Tail};
  Body third{head.x_, head.y_ + 2, BodyPart::Tail};
  Body tail{head.x_, head.y_ + 3, BodyPart::Tail};
  Body extra{head.x_, head.y_ + 4, BodyPart::Extra};
  full_snake_.insert(full_snake_.begin(), {head, second, third, tail, extra});
  dir_ = Direction::Up;
}

void Game::Snake::MoveForward() {
  int new_x{GiveSnakeHeadX()}, new_y{GiveSnakeHeadY()};
  switch (dir_) {
    case Direction::Left:
      --new_x;
      break;
    case Direction::Right:
      ++new_x;
      break;
    case Direction::Up:
      --new_y;
      break;
    case Direction::Down:
      ++new_y;
      break;
  }
  for (int i{0}, end{SnakeSize()}; i < end; ++i) {
    int temp_x = full_snake_[i].x_;
    full_snake_[i].x_ = new_x;
    new_x = temp_x;

    int temp_y = full_snake_[i].y_;
    full_snake_[i].y_ = new_y;
    new_y = temp_y;
  }
  if (full_snake_.back().part_ != BodyPart::Extra) {
    Body new_extra{new_x, new_y, BodyPart::Extra};
    full_snake_.push_back(new_extra);
  }
}

void Game::Snake::IncreaseLength() {
  full_snake_.back().part_ = BodyPart::Tail;
}

int Game::Snake::GiveSnakeHeadX() { return full_snake_.front().x_; }

int Game::Snake::GiveSnakeHeadY() { return full_snake_.front().y_; }

bool Game::Snake::IsHeadOnExtra() {
  return (full_snake_.back().x_ == GiveSnakeHeadX() &&
          full_snake_.back().y_ == GiveSnakeHeadY());
}

void Game::Snake::SetDirection(Direction dir) { dir_ = dir; }

int Game::Snake::SnakeSize() { return static_cast<int>(full_snake_.size()); }

Game::Apple::Apple() : x_(5), y_(10) {}

void Game::Apple::GenerateApple() {
  std::mt19937 engine;
  engine.seed(std::time(nullptr));
  x_ = engine() % WIDE + BUFFER;
  y_ = engine() % HIGH + BUFFER;
}

int Game::Apple::GiveAppleX() { return x_; }

int Game::Apple::GiveAppleY() { return y_; }

Game::Field::Field() {
  main_.field = new int* [HIGH_ACT] {};
  for (int i{0}; i < HIGH_ACT; ++i) {
    main_.field[i] = new int[WIDE_ACT]{0};
  }
  main_.next = nullptr;
  main_.high_score = HighScoreRead();
  main_.score = STANDART_SIZE;
  main_.level = 0;
  main_.speed = 0;
  main_.pause = 1;
}

Game::Field::~Field() { ClearField(); }

void Game::Field::ClearField() {
  if (main_.field != nullptr) {
    for (int i{0}; i < HIGH_ACT; ++i) {
      delete[] main_.field[i];
    }
    delete[] main_.field;
    main_.field = nullptr;
  }
}

void Game::Field::SetFieldXY(int x, int y, int value) {
  main_.field[y][x] = value;
}

int Game::Field::HighScoreRead() {
  int score = STANDART_SIZE;
  std::ifstream in("HighScore.txt");
  if (in.is_open()) {
    std::string line;
    std::getline(in, line);
    score = std::stoi(line) > score ? std::stoi(line) : score;
  }
  in.close();
  return score;
}

void Game::Field::HighScoreWrite() {
  std::ofstream out;
  out.open("HighScore.txt");
  if (out.is_open()) {
    out << main_.high_score << std::endl;
  }
  out.close();
}

GameInfo_t Game::GiveGameInfo() { return field_.main_; }

my::Direction Game::ChangeDirection(const UserAction_t& action) {
  static Direction dir = Direction::Up;
  switch (action) {
    case Left:
      if (snake_.dir_ != Direction::Right) dir = Direction::Left;
      break;
    case Right:
      if (snake_.dir_ != Direction::Left) dir = Direction::Right;
      break;
    case Up:
      if (snake_.dir_ != Direction::Down) dir = Direction::Up;
      break;
    case Down:
      if (snake_.dir_ != Direction::Up) dir = Direction::Down;
      break;
    default:
      break;
  }
  return dir;
}

bool Game::IsAppleOnSnake() {
  bool res = false;
  for (Snake::Body n : snake_.full_snake_) {
    if (n.x_ == apple_.GiveAppleX() && n.y_ == apple_.GiveAppleY()) res = true;
  }
  return res;
}

bool Game::IsTime() {
  using namespace std::chrono_literals;
  bool res = false;
  static auto start = std::chrono::steady_clock::now();
  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> diff = end - start;
  auto speed = 221ms - 20ms * Speed(SpeedType::Get);
  if (diff > speed) {
    res = true;
    start = end;
  }
  return res;
}

void Game::RefreshSnake() {
  bool isHeadOnExtra = snake_.IsHeadOnExtra();
  for (Game::Snake::Body n : snake_.full_snake_) {
    if (n.part_ == BodyPart::Extra && !isHeadOnExtra)
      field_.SetFieldXY(n.x_, n.y_, EMPTY);
    else
      field_.SetFieldXY(n.x_, n.y_, 5);
  }
}

void Game::Leveling() {
  if (field_.main_.score % 5 == 0 || field_.main_.level == 10) {
    field_.main_.level++;
    Speed(SpeedType::Increase);
  }
}

void Game::IncreaseScore() {
  field_.main_.score++;
  if (field_.main_.score > field_.main_.high_score) {
    field_.main_.high_score = field_.main_.score;
  }
}

int Game::Speed(SpeedType type) {
  static int act_speed = 0;
  switch (type) {
    case SpeedType::NitroON:
      if (field_.main_.speed <= act_speed) {
        int nitro = 5;
        field_.main_.speed = act_speed + nitro;
      }
      break;
    case SpeedType::NitroOFF:
      field_.main_.speed = act_speed;
      break;
    case SpeedType::Increase:
      ++act_speed;
      break;
    case SpeedType::Get:
      break;
  }
  return field_.main_.speed;
}

bool Game::Pause() {
  field_.main_.pause = !field_.main_.pause;
  return field_.main_.pause;
}

bool Game::SnakeTouchSnake() {
  bool res = false;
  int head_x{snake_.GiveSnakeHeadX()}, head_y{snake_.GiveSnakeHeadY()};
  for (Snake::Body n : snake_.full_snake_) {
    if (n.part_ == BodyPart::Head || n.part_ == BodyPart::Extra) continue;
    if (n.x_ == head_x && n.y_ == head_y) res = true;
  }
  return res;
}

bool Game::SnakeTouchBoard() {
  int head_x{snake_.GiveSnakeHeadX()}, head_y{snake_.GiveSnakeHeadY()};
  bool res = (head_x == 0 || head_x == WIDE_ACT - BUFFER || head_y == 0 ||
              head_y == HIGH_ACT - BUFFER);
  return res;
}

bool Game::IsTouching() {
  bool res = (SnakeTouchSnake() || SnakeTouchBoard());
  return res;
}

bool Game::IsWin() {
  bool res = static_cast<int>(snake_.full_snake_.size()) == 200;
  return res;
}

GameState Game::SpawnSnake() {
  RefreshSnake();
  return GameState::SpawnApple;
}

GameState Game::SpawnApple() {
  while (IsAppleOnSnake()) apple_.GenerateApple();
  field_.SetFieldXY(apple_.GiveAppleX(), apple_.GiveAppleY(), 1);
  return GameState::Moving;
}

GameState Game::Moving(const UserAction_t& action) {
  if (action == Action) {
    Speed(SpeedType::NitroON);
  } else {
    ChangeDirection(action);
    Speed(SpeedType::NitroOFF);
  }
  return GameState::Shifting;
}

GameState Game::Eating() {
  snake_.IncreaseLength();
  IncreaseScore();
  Leveling();
  return IsWin() ? GameState::Attaching : GameState::SpawnApple;
}

GameState Game::Shifting() {
  if (IsTime()) {
    snake_.SetDirection(ChangeDirection(Start));
    snake_.MoveForward();
    RefreshSnake();
  }
  GameState state = GameState::Moving;
  if (IsTouching()) state = GameState::Attaching;
  if (IsAppleOnSnake()) state = GameState::Eating;
  return state;
}

void Game::GameOver() {
  field_.ClearField();
  field_.HighScoreWrite();
}

GameInfo_t MainFsmFunction(int update, UserAction_t action) {
  static my::Game game;
  static my::GameState state = my::GameState::SpawnSnake;
  static std::queue<UserAction_t> action_queue;
  static bool not_pause = true;

  if (action == Pause)
    not_pause = game.Pause();
  else if (action == Terminate)
    game.GameOver();
  else if (action != Start)
    action_queue.push(action);

  if (update == NO && not_pause) {
    if (!action_queue.empty()) action = action_queue.front();
    switch (state) {
      case my::GameState::SpawnSnake:
        state = game.SpawnSnake();
        break;
      case my::GameState::SpawnApple:
        state = game.SpawnApple();
        break;
      case my::GameState::Moving:
        state = game.Moving(action);
        if (!action_queue.empty()) action_queue.pop();
        break;
      case my::GameState::Eating:
        state = game.Eating();
        break;
      case my::GameState::Shifting:
        state = game.Shifting();
        break;
      case my::GameState::Attaching:
        game.GameOver();
        break;
      default:
        break;
    }
  }
  return game.GiveGameInfo();
}
