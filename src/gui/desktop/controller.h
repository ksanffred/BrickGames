#ifndef SNAKE_CONTROLLER_H
#define SNAKE_CONTROLLER_H

#include <QCoreApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QObject>
#include <QVariantList>
#include <QTimer>
#include <QDebug>

#include "../../defines.h"

#ifdef BUILD_SNAKE
#include "../../brick_game/snake/snake_back.h"
#elif defined(BUILD_TETRIS)
#include "../../brick_game/tetris/back_fsm_tetris.h"
#endif

void userInput(UserAction_t action, bool hold);
GameInfo_t updateCurrentState();

namespace my {

class SnakeController : public QObject {
    Q_OBJECT

    // Expose a single property for all game data
    Q_PROPERTY(QList<QVariant> gameField READ gameField NOTIFY gameFieldChanged)
    Q_PROPERTY(QList<QVariant> gameNext READ gameNext NOTIFY gameNextChanged)
    Q_PROPERTY(QList<QVariant> gameData READ gameData NOTIFY gameDataChanged)

public:
    explicit SnakeController(QObject *parent = nullptr);
    ~SnakeController();

    // Методы, которые будет вызывать QML для отправки команд
    Q_INVOKABLE void sendAction(int action);

    // Q_INVOKABLE методы для запуска/остановки игрового цикла
    Q_INVOKABLE void startGameLoop();
    Q_INVOKABLE void stopGameLoop();

    // Геттер для свойства Q_PROPERTY
    QList<QVariant> gameField() const;
    QList<QVariant> gameNext() const;
    QList<QVariant> gameData() const;

signals:
    // Сигнал для уведомления QML об изменении данных
    void gameDataChanged();
    void gameNextChanged();
    void gameFieldChanged();


private slots:
    // Слот, который вызывается таймером для обновления
    void updateGame();

private:
    QTimer *m_gameTimer;
    QList<QVariant> m_gameField;
    QList<QVariant> m_gameNext;
    QList<QVariant> m_gameData;

    void setGameData(const QList<QVariant> &field_list, const QList<QVariant> &next_list, const QList<QVariant> &data);
};

}; // namespace my

#endif // SNAKE_CONTROLLER_H
