#ifndef BOMB_H
#define BOMB_H

#include "paotai.h"

#define GAP_Y 100
#define GAP_X 110

class Bomb : public Paotai
{
public:
    Bomb();
    void advance(int phase) override;
    bool collidesWithItem(const QGraphicsItem* other, Qt::ItemSelectionMode mode) const override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    QRectF boundingRect() const override;
    bool upgrade() override;

private:
    bool hasEnemyInRange() const;
    void explode();
    void spawnPoweredCopies();
    bool canPlaceCopyAt(const QPointF& pos) const;
    bool exploded;
    int explosionCounter;
    qreal rangeScale;
};

#endif // BOMB_H
