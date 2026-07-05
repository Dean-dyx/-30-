#ifndef PROTECTER_H
#define PROTECTER_H

#include "paotai.h"

class Protecter : public Paotai
{
public:
    Protecter();
    void advance(int phase) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    bool upgrade() override;

private:
    bool powered;
    int powerCounter;
};

#endif // PROTECTER_H
