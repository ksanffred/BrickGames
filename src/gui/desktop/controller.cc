#include "controller.h"

void userInput(UserAction_t action, bool hold) {
  bool __attribute__((unused)) fake_hold = hold;
  MainFsmFunction(SUCCESS, action);
}

GameInfo_t updateCurrentState() { return MainFsmFunction(NO, Start); }

namespace my {

SnakeController::SnakeController(QObject *parent) : QObject(parent) {
  m_gameTimer = new QTimer(this);
  connect(m_gameTimer, &QTimer::timeout, this, &SnakeController::updateGame);
  updateGame();
}

SnakeController::~SnakeController() { stopGameLoop(); }

void SnakeController::sendAction(int action) {
  userInput(static_cast<UserAction_t>(action), false);
  updateGame();
}

void SnakeController::startGameLoop() {
  sendAction(Start);
  m_gameTimer->start(10);
}

void SnakeController::stopGameLoop() { m_gameTimer->stop(); }

QList<QVariant> SnakeController::gameData() const { return m_gameData; }

QList<QVariant> SnakeController::gameNext() const { return m_gameNext; }

QList<QVariant> SnakeController::gameField() const { return m_gameField; }

void SnakeController::setGameData(const QList<QVariant> &field_list,
                                  const QList<QVariant> &next_list,
                                  const QList<QVariant> &data) {
  if (m_gameField != field_list) {
    m_gameField = field_list;
    emit gameFieldChanged();
  }
  if (m_gameNext != next_list) {
    m_gameNext = next_list;
    emit gameNextChanged();
  }
  if (m_gameData != data) {
    m_gameData = data;
    emit gameDataChanged();
  }
}

void SnakeController::updateGame() {
  GameInfo_t info = updateCurrentState();
  QList<QVariant> QInfo;
  QInfo << info.score << info.high_score << info.level << info.speed
        << info.pause;

  QList<QVariant> next_list;
  if (info.next != nullptr) {
    for (int r = 0; r < 4; ++r) {
      for (int c = 0; c < 4; ++c) {
        if (r == 0 || r == 3)
          next_list.append(0);
        else
          next_list.append(info.next[r - 1][c]);
      }
    }
  }

  QList<QVariant> field_list;
  if (info.field != nullptr) {
    for (int r = BUFFER; r < HIGH_ACT - BUFFER; ++r) {
      for (int c = BUFFER; c < WIDE_ACT - BUFFER; ++c) {
        field_list.append(info.field[r][c]);
      }
    }
  } else {
    stopGameLoop();
  }
  setGameData(field_list, next_list, QInfo);
}

};  // namespace my

#ifdef BUILD_SNAKE
#define URL_STR "qrc:/front_SnakeGame/front.qml"
#elif defined(BUILD_TETRIS)
#define URL_STR "qrc:/front_TetrisGame/front.qml"
#endif

int main(int argc, char *argv[]) {
  QGuiApplication app{argc, argv};

  my::SnakeController snakeController;
  QQmlApplicationEngine engine;

  engine.rootContext()->setContextProperty("snakeController", &snakeController);

  const QUrl url{URL_STR};
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);
  engine.load(url);

  return app.exec();
}