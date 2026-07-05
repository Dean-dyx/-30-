#ifndef SHOP_H
#define SHOP_H

#include <QGraphicsObject>

class Shop : public QGraphicsObject
{
public:
    Shop();

    QRectF boundingRect() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    void advance(int phase) override;

    int coin_val;
    int coin_generate_time;
    int counter;
};

#endif // SHOP_H