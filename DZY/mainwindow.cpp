#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gamewindow.h"

#include <QFrame>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , menuAnimTimer(new QTimer(this))
    , menuTick(0)
{
    ui->setupUi(this);
    setupCyberMenu();
    connect(menuAnimTimer, &QTimer::timeout, this, &MainWindow::animateMenu);
    menuAnimTimer->start(33);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupCyberMenu()
{
    setFixedSize(800, 600);
    setWindowTitle("DZY Defense");

    ui->menuArea->setStyleSheet(
        "QWidget#menuArea {"
        "  background-image: url(:/resources/menu_starfield.png);"
        "  background-position: center;"
        "  border: 1px solid #2ae8ff;"
        "}"
        "QPushButton {"
        "  color: #e8fdff;"
        "  background-color: rgba(5, 26, 48, 225);"
        "  border: 2px solid #35efff;"
        "  border-left: 6px solid #85ffdf;"
        "  border-right: 6px solid #5d7dff;"
        "  border-radius: 3px;"
        "  font: 800 15px 'Microsoft YaHei';"
        "  padding: 8px 20px;"
        "}"
        "QPushButton:hover {"
        "  color: #ffffff;"
        "  background-color: rgba(18, 88, 118, 240);"
        "  border-color: #b7ffff;"
        "}"
        "QPushButton:pressed {"
        "  background-color: rgba(3, 17, 36, 245);"
        "}"
        "QLabel#title {"
        "  color: #dbfdff;"
        "  background: transparent;"
        "  font: 900 36px 'Microsoft YaHei';"
        "}"
    );

    QLabel* topBar = new QLabel(ui->menuArea);
    topBar->setGeometry(95, 78, 610, 3);
    topBar->setStyleSheet("background-color: rgba(105, 250, 255, 170); border: 0px;");
    topBar->show();

    QLabel* bottomBar = new QLabel(ui->menuArea);
    bottomBar->setGeometry(150, 510, 500, 2);
    bottomBar->setStyleSheet("background-color: rgba(130, 255, 210, 110); border: 0px;");
    bottomBar->show();

    ui->title->setText("DZY 星域防御矩阵");
    ui->title->setGeometry(185, 105, 430, 64);
    ui->title->setAlignment(Qt::AlignCenter);
    ui->title->raise();

    QLabel* subtitle = new QLabel("STELLAR TACTICAL GRID ONLINE", ui->menuArea);
    subtitle->setGeometry(185, 170, 430, 24);
    subtitle->setAlignment(Qt::AlignCenter);
    subtitle->setStyleSheet("color: #85ffdf; background: transparent; font: 800 13px 'Consolas';");
    subtitle->show();

    QLabel* core = new QLabel(ui->menuArea);
    core->setGeometry(232, 205, 336, 96);
    core->setStyleSheet(
        "background-color: rgba(4, 17, 38, 172);"
        "border: 1px solid rgba(115, 255, 245, 190);"
        "border-radius: 6px;"
    );
    core->show();

    QLabel* coreText = new QLabel("ORBITAL COMMAND LINK", ui->menuArea);
    coreText->setGeometry(232, 227, 336, 26);
    coreText->setAlignment(Qt::AlignCenter);
    coreText->setStyleSheet("color: #d8ffff; background: transparent; font: 800 15px 'Consolas';");
    coreText->show();

    QLabel* coreSubText = new QLabel("wave scanner / energy economy / defense upgrade", ui->menuArea);
    coreSubText->setGeometry(232, 257, 336, 22);
    coreSubText->setAlignment(Qt::AlignCenter);
    coreSubText->setStyleSheet("color: rgba(150, 235, 255, 190); background: transparent; font: 11px 'Consolas';");
    coreSubText->show();

    ui->startGameBtn->setText("开始游戏");
    ui->startGameBtn->setGeometry(310, 334, 180, 46);
    ui->startGameBtn->raise();

    QPushButton* rulesBtn = new QPushButton("游戏规则", ui->menuArea);
    rulesBtn->setGeometry(310, 394, 180, 46);
    rulesBtn->show();

    connect(rulesBtn, &QPushButton::clicked, this, [this]() {
        QMessageBox::information(
            this,
            "游戏规则",
            "1. 选择卡牌后点击网格放置炮台。\n"
            "2. 左键点击炮台会显示升级按钮，升级费用逐级递增，最高三级。\n"
            "3. FREEZE 减速敌人，POWER 强化一个炮台，SHOVEL 移除炮台。\n"
            "4. 击杀敌人获得积分，清空一波后自动进入下一波。\n"
            "5. 敌人首次突破最左侧防线时游戏结束。"
        );
    });

    for (int i = 0; i < 6; ++i)
    {
        QFrame* line = new QFrame(ui->menuArea);
        line->setGeometry(-160 + i * 150, 82 + i * 66, 260, 2);
        line->setStyleSheet("background-color: rgba(65, 240, 255, 100); border: 0px;");
        line->show();
        scanLines.append(line);
    }

    const QPoint nodePositions[] = {
        QPoint(125, 120), QPoint(650, 135), QPoint(150, 430),
        QPoint(612, 444), QPoint(400, 86), QPoint(395, 502),
        QPoint(265, 285), QPoint(535, 285)
    };
    for (const QPoint& p : nodePositions)
    {
        QWidget* node = new QWidget(ui->menuArea);
        node->setGeometry(p.x(), p.y(), 10, 10);
        node->setStyleSheet("background-color: #7dfff2; border: 1px solid #ffffff; border-radius: 5px;");
        node->show();
        pulseNodes.append(node);
    }
}

void MainWindow::animateMenu()
{
    menuTick++;
    for (int i = 0; i < scanLines.size(); ++i)
    {
        QWidget* line = scanLines[i];
        const int x = (line->x() + 3 + i) % 960 - 160;
        line->move(x, line->y());
    }

    for (int i = 0; i < pulseNodes.size(); ++i)
    {
        const int phase = (menuTick + i * 17) % 60;
        const int size = phase < 30 ? 8 + phase / 6 : 18 - phase / 6;
        QWidget* node = pulseNodes[i];
        const QPoint center = node->geometry().center();
        node->setGeometry(center.x() - size / 2, center.y() - size / 2, size, size);
    }
}

void MainWindow::on_startGameBtn_clicked()
{
    GameWindow *gameWin = new GameWindow(nullptr);
    gameWin->show();
    hide();
}
