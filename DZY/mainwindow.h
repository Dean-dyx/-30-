#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_startGameBtn_clicked();
    void animateMenu();

private:
    void setupCyberMenu();

    Ui::MainWindow *ui;
    QTimer* menuAnimTimer;
    QList<QWidget*> scanLines;
    QList<QWidget*> pulseNodes;
    int menuTick;
};

#endif // MAINWINDOW_H
