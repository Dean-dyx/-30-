#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QTimer>
#include <QList>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>

class Enemy;
class NormalEnemy;
class PlusEnemy;

namespace Ui {
class GameWindow;
}

class GameWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

private slots:
    void on_startWaveButton_clicked();

    void on_pauseButton_clicked();

    void on_backButton_clicked();

private:
    Ui::GameWindow *ui;
    QGraphicsScene *scene;
    QTimer *gameTimer;
    int currentWave;
    int enemiesRemaining;
    int spawnFrameCounter;
    int spawnInterval;
    QList<int> spawnQueue;
    QGraphicsTextItem* statusText;
    QGraphicsTextItem* gameOverText;
    QGraphicsLineItem* scanLine;
    QGraphicsLineItem* scanLine2;
    int uiAnimTick;
    bool isPaused;
    bool isGameRunning;
    bool waveStarted;

    void updateGame();
    void spawnWave();
    void spawnNextQueuedEnemy();
    void updateStatusText();
    bool hasLiveEnemies() const;
};

#endif // GAMEWINDOW_H
