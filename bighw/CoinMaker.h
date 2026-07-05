#ifndef COINMAKER_H
#define COINMAKER_H

#include "paotai.h"

class CoinMaker : public Paotai
{
public:
    CoinMaker();
    void advance(int phase) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    bool upgrade() override;

private:
    int income;
    bool powered;
    int powerCounter;
    void addCoins(int amount);
};

#endif // COINMAKER_H
