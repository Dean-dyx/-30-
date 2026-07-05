#include "coin.h"
#include <QPainter>

Coin::Coin(int val_)
    : val(val_)
{
    movie = new QMovie(":/resources/coin.gif");
    movie->start();
}

Coin::~Coin()
{
    delete movie;
}

QRectF Coin::boundingRect() const
{
    return QRectF(-35, -35, 70, 70);
}

void Coin::paint(QPainter *painter,
                 const QStyleOptionGraphicsItem *,
                 QWidget *)
{
    painter->drawImage(boundingRect(),
                       movie->currentImage());
}