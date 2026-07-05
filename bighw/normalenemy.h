#ifndef NORMALENEMY_H
#define NORMALENEMY_H

#include "enemy.h"
#include "paotai.h"
#include "button.h"

class NormalEnemy:public Enemy
{
public:
    NormalEnemy();
    void advance(int phase) override;
    void snowfree() override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
};


#endif // NORMALENEMY_H
