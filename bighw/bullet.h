#ifndef BULLET_H
#define BULLET_H
#include "other.h"

class Bullet:public other
{
public:
    Bullet(int atk_num,int snowor);
    ~Bullet() override;
    void advance(int phase) override;
    bool collidesWithItem(const QGraphicsItem*other,Qt::ItemSelectionMode mode)const override;
    void paint(QPainter*painter,const QStyleOptionGraphicsItem* option,QWidget*widget) override;
    QRectF boundingRect() const override;
private:
    qreal speed;
    int atk;
    int snow;
};

#endif // BULLET_H
