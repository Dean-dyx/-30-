#ifndef BUTTON_H
#define BUTTON_H

#include "other.h"
#include <QCursor>
#include <QGraphicsScene>

#define FREEZE 1
#define POWER  2
#define SHOVEL 3

class Button : public other
{
public:
    Button(int t);

    void advance(int phase) override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    QRectF boundingRect() const override;

    static bool shovel_activate;
    static bool power_activate;
    static void consumePower(QGraphicsScene* scene);

    int counter;
    int state;

protected:
    int cool_time;
    int type;
};

#endif // BUTTON_H
