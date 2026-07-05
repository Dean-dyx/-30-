#ifndef COIN_H
#define COIN_H

#include <QGraphicsObject>
#include <QMovie>

class Coin : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit Coin(int val_ = 50);
    ~Coin();

    QRectF boundingRect() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

private:
    int val;
    QMovie *movie;
};

#endif // COIN_H