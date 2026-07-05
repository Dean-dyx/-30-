#include "shop.h"
#include <QPainter>

Shop::Shop()
{
    coin_val = 90;
    counter = 0;
    coin_generate_time = 900;
}

QRectF Shop::boundingRect() const
{
    return QRectF(0, 0, 240, 82);
}

void Shop::paint(QPainter *painter,
                 const QStyleOptionGraphicsItem *,
                 QWidget *)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(QPen(QColor(45, 220, 255), 1.6));
    painter->setBrush(QColor(7, 28, 48, 235));
    painter->drawRoundedRect(QRectF(0, 0, 238, 80), 6, 6);

    painter->setPen(QPen(QColor(100, 255, 230, 70), 1));
    painter->drawLine(QPointF(14, 12), QPointF(224, 12));
    painter->drawLine(QPointF(14, 68), QPointF(224, 68));

    painter->setPen(QPen(QColor(255, 215, 65), 2));
    painter->setBrush(QColor(70, 50, 10, 240));
    painter->drawEllipse(QRectF(14, 24, 34, 34));
    painter->setPen(QColor(255, 245, 170));
    QFont coinFont("Calibri", 20, QFont::Bold);
    painter->setFont(coinFont);
    painter->drawText(QRectF(14, 24, 34, 34), Qt::AlignCenter, "$");

    QFont labelFont("Calibri", 11, QFont::Bold);
    painter->setFont(labelFont);
    painter->setPen(QColor(120, 245, 255));
    painter->drawText(QRectF(58, 15, 150, 18), Qt::AlignLeft | Qt::AlignVCenter, "ENERGY COINS");

    QFont font("Consolas", 30, QFont::Bold);
    painter->setFont(font);
    painter->setPen(QColor(255, 230, 95));
    painter->drawText(QRectF(58, 31, 150, 38),
                      Qt::AlignLeft | Qt::AlignVCenter,
                      QString::number(coin_val));
}

void Shop::advance(int phase)
{
    if (!phase) return;
    update();

    if (++counter >= coin_generate_time)
    {
        counter = 0;
        coin_val += 15;
    }
}
