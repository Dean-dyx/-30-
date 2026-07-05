#ifndef SHOOTER_H
#define SHOOTER_H

#include "paotai.h"

class Shooter : public Paotai
{
public:
    Shooter();

    void advance(int phase) override;
    bool collidesWithItem(const QGraphicsItem *other, Qt::ItemSelectionMode mode) const override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    bool upgrade() override;

private:
    int powerCounter;
    int baseAtk;
    int baseTime;
};

#endif // SHOOTER_H
