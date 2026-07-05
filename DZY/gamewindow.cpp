#include "gamewindow.h"
#include "ui_gamewindow.h"
#include "card.h"
#include "map.h"
#include "shop.h"
#include "button.h"
#include "coin.h"
#include "enemy.h"
#include "normalenemy.h"
#include "plusenemy.h"
#include <QApplication>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainter>
#include <QRandomGenerator>
#include <QTimer>
#include <QVBoxLayout>

GameWindow::GameWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameWindow)
{
    ui->setupUi(this);

    setFixedSize(1400, 650);
    setWindowTitle("Tower Defense");

    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 1400, 650);
    scene->setBackgroundBrush(QColor(5, 12, 24));

    QGraphicsView *view = new QGraphicsView(scene, this);
    view->setScene(scene);
    view->setFixedSize(1400, 650);
    view->setStyleSheet("QGraphicsView { border: 0px; background: #050c18; }");
    view->setRenderHint(QPainter::Antialiasing, true);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setStyleSheet(
        "QPushButton {"
        " color: #d9fbff;"
        " background-color: rgba(10, 45, 70, 220);"
        " border: 1px solid #31d7ff;"
        " border-radius: 4px;"
        " font: 700 14px 'Microsoft YaHei';"
        " padding: 4px 10px;"
        "}"
        "QPushButton:hover { background-color: rgba(18, 80, 110, 230); border-color: #8cf6ff; }"
        "QPushButton:pressed { background-color: rgba(5, 25, 40, 230); }"
    );

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(view);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    Shop *shop = new Shop();
    shop->setPos(18, 38);
    shop->setZValue(160);
    scene->addItem(shop);

    const QStringList cardNames = {"coinmaker", "shooter", "protecter", "bomb"};
    for (const QString &cardName : cardNames) {
        Card *card = new Card(cardName);
        card->setPos(Card::pos[Card::id[cardName]]);
        scene->addItem(card);
    }

    Button *freezeBtn = new Button(FREEZE);
    freezeBtn->setPos(800, 40);
    freezeBtn->setZValue(100);
    scene->addItem(freezeBtn);

    Button *powerBtn = new Button(POWER);
    powerBtn->setPos(960, 40);
    powerBtn->setZValue(100);
    scene->addItem(powerBtn);

    Button *shovelBtn = new Button(SHOVEL);
    shovelBtn->setPos(1120, 40);
    shovelBtn->setZValue(100);
    scene->addItem(shovelBtn);

    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 9; col++) {
            Map *map = new Map(1);
            map->setPos(250 + col * 110, 150 + row * 110);
            scene->addItem(map);
        }
    }

    ui->startWaveButton->raise();
    ui->pauseButton->raise();
    ui->backButton->raise();

    currentWave = 1;
    enemiesRemaining = 0;
    spawnFrameCounter = 0;
    spawnInterval = 50;
    isPaused = false;
    isGameRunning = true;
    waveStarted = false;
    Enemy::totalScore = 0;
    Enemy::gameOver = false;

    statusText = new QGraphicsTextItem();
    statusText->setDefaultTextColor(QColor(130, 245, 255));
    statusText->setFont(QFont("Calibri", 16, QFont::Bold));
    statusText->setPos(620, 10);
    statusText->setZValue(200);
    scene->addItem(statusText);

    gameOverText = new QGraphicsTextItem();
    gameOverText->setDefaultTextColor(QColor(180, 40, 40));
    gameOverText->setFont(QFont("Calibri", 36, QFont::Bold));
    gameOverText->setPos(500, 260);
    gameOverText->setZValue(300);
    gameOverText->hide();
    scene->addItem(gameOverText);
    updateStatusText();

    scanLine = scene->addLine(250, 150, 1240, 150, QPen(QColor(80, 245, 255, 90), 2));
    scanLine->setZValue(20);
    scanLine2 = scene->addLine(250, 590, 1240, 590, QPen(QColor(90, 255, 170, 55), 1));
    scanLine2->setZValue(20);
    uiAnimTick = 0;

    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &GameWindow::updateGame);
    gameTimer->start(16);
}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::on_pauseButton_clicked()
{
    isPaused = !isPaused;
    ui->pauseButton->setText(isPaused ? "Resume" : "Pause");
    qDebug() << (isPaused ? "game paused" : "game resumed");
}

void GameWindow::on_backButton_clicked()
{
    qDebug() << "back button clicked";
    const QList<QWidget*> widgets = qApp->topLevelWidgets();
    for (QWidget *widget : widgets) {
        if (widget->inherits("MainWindow")) {
            widget->show();
            break;
        }
    }

    close();
}

void GameWindow::on_startWaveButton_clicked()
{
    if (isPaused || !isGameRunning) return;

    if (!hasLiveEnemies() && spawnQueue.isEmpty()) {
        waveStarted = true;
        spawnWave();
    } else {
        qDebug() << "wait until current enemies are cleared";
    }
}

void GameWindow::updateGame()
{
    if (isPaused || !isGameRunning) return;

    scene->advance();
    uiAnimTick++;
    const qreal scanY = 150 + (uiAnimTick * 1.2);
    const qreal wrappedY = 150 + int(scanY - 150) % 440;
    scanLine->setLine(250, wrappedY, 1240, wrappedY);
    const qreal scanY2 = 590 - (uiAnimTick * 0.75);
    const qreal wrappedY2 = 150 + int(scanY2 - 150 + 440 * 20) % 440;
    scanLine2->setLine(250, wrappedY2, 1240, wrappedY2);

    if (Enemy::gameOver)
    {
        isGameRunning = false;
        gameOverText->setPlainText(QString("GAME OVER\nFinal Score: %1").arg(Enemy::totalScore));
        gameOverText->show();
        updateStatusText();
        return;
    }

    if (!spawnQueue.isEmpty())
    {
        if (++spawnFrameCounter >= spawnInterval)
        {
            spawnFrameCounter = 0;
            spawnNextQueuedEnemy();
        }
    }
    else if (waveStarted && !hasLiveEnemies())
    {
        currentWave++;
        spawnWave();
    }

    updateStatusText();
}

void GameWindow::spawnWave()
{
    qDebug() << "========== wave" << currentWave << "start ==========";

    spawnQueue.clear();
    int budget = 4 + currentWave * 3;
    const int waveBudget = budget;

    while (budget > 0)
    {
        const bool canSpawnPlus = currentWave >= 3 && budget >= 4;
        const bool choosePlus = canSpawnPlus && QRandomGenerator::global()->bounded(100) < 30;
        if (choosePlus)
        {
            spawnQueue.append(1);
            budget -= 4;
        }
        else
        {
            spawnQueue.append(0);
            budget -= 1;
        }
    }

    enemiesRemaining = spawnQueue.size();
    spawnFrameCounter = spawnInterval;
    qDebug() << "wave budget" << waveBudget << "enemy count" << enemiesRemaining;
    updateStatusText();
}

void GameWindow::spawnNextQueuedEnemy()
{
    if (spawnQueue.isEmpty())
        return;

    const int enemyType = spawnQueue.takeFirst();
    const int rows[] = {150, 260, 370, 480, 590};
    const int row = QRandomGenerator::global()->bounded(5);

    Enemy *enemy = enemyType == 1
                       ? static_cast<Enemy*>(new PlusEnemy())
                       : static_cast<Enemy*>(new NormalEnemy());
    enemy->setPos(1350, rows[row]);
    enemy->setZValue(50);
    scene->addItem(enemy);
}

void GameWindow::updateStatusText()
{
    if (!statusText)
        return;

    statusText->setPlainText(QString("Wave: %1   Score: %2   Queue: %3")
                                 .arg(currentWave)
                                 .arg(Enemy::totalScore)
                                 .arg(spawnQueue.size()));
}

bool GameWindow::hasLiveEnemies() const
{
    const QList<QGraphicsItem*> items = scene->items();
    for (QGraphicsItem *item : items)
    {
        Enemy* enemy = qgraphicsitem_cast<Enemy*>(item);
        if (enemy && enemy->state != DEAD)
            return true;
    }
    return false;
}
