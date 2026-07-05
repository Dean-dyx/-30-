#ifndef ENEMY_H
#define ENEMY_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QMovie>
#include <QPixmap>
#include <QPainter>
#include <QDebug>

#define WALK  0
#define ATTACK 1
#define DEAD  2
#define BURN  3

#define SLOW    1
#define NO_SLOW 0

#define WIDTH  1400
#define HEIGHT 602

class Enemy : public QGraphicsItem
{
public:
    qreal hp;
    qreal maxHp;
    qreal atk;
    int state;
    int slow;
    qreal speed;
    int time;
    int counter;
    int difficultyValue;
    bool scoreAwarded;

    static int totalScore;
    static bool gameOver;

    enum { Type = UserType + 2 };

    Enemy();
    ~Enemy() override;

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    bool collidesWithItem(const QGraphicsItem* other, Qt::ItemSelectionMode mode) const override;
    int type() const override;

    void SetMovie(const QString &path);
    void Sethead(const QString &path);
    virtual void snowfree() = 0;

    QMovie* movie;
    QPixmap staticPixmap;
    bool super;

protected:
    QMovie* head;
    QPixmap headPixmap;
};

#endif // ENEMY_H
