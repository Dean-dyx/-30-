#include "map.h"
#include "CoinMaker.h"
#include "bomb.h"
#include "card.h"
#include "paotai.h"
#include "protecter.h"
#include "shooter.h"
#include "shop.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>

Map::Map(int gridType)
    : m_gridState(1)
    , m_gridType(gridType)
{
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    setZValue(1);
}

void Map::advance(int phase)
{
    if (m_gridType != 1 || !phase)
        return;

    setCursor(Card::card_activate ? Qt::PointingHandCursor : Qt::ArrowCursor);
}

void Map::paint(QPainter *painter,
                const QStyleOptionGraphicsItem *option,
                QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    const QRectF rect = boundingRect().adjusted(5, 5, -5, -5);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setBrush(QColor(12, 30, 48, 185));
    painter->setPen(QPen(QColor(44, 210, 230, 135), 1.2));
    painter->drawRoundedRect(rect, 6, 6);

    painter->setPen(QPen(QColor(95, 250, 255, 45), 1));
    painter->drawLine(QPointF(rect.left() + 12, rect.center().y()),
                      QPointF(rect.right() - 12, rect.center().y()));
    painter->drawLine(QPointF(rect.center().x(), rect.top() + 12),
                      QPointF(rect.center().x(), rect.bottom() - 12));

    painter->setPen(QPen(QColor(130, 255, 210, 80), 2));
    const qreal c = 10;
    painter->drawLine(rect.topLeft(), rect.topLeft() + QPointF(c, 0));
    painter->drawLine(rect.topLeft(), rect.topLeft() + QPointF(0, c));
    painter->drawLine(rect.topRight(), rect.topRight() + QPointF(-c, 0));
    painter->drawLine(rect.topRight(), rect.topRight() + QPointF(0, c));
    painter->drawLine(rect.bottomLeft(), rect.bottomLeft() + QPointF(c, 0));
    painter->drawLine(rect.bottomLeft(), rect.bottomLeft() + QPointF(0, -c));
    painter->drawLine(rect.bottomRight(), rect.bottomRight() + QPointF(-c, 0));
    painter->drawLine(rect.bottomRight(), rect.bottomRight() + QPointF(0, -c));
}

void Map::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_gridType != 1)
        return;

    if (event->button() == Qt::RightButton)
    {
        Card::card_activate = false;
        return;
    }

    if (event->button() != Qt::LeftButton || !Card::card_activate)
        return;

    const int cardId = Card::card_id;
    if (!Card::mapping.contains(cardId))
    {
        Card::card_activate = false;
        return;
    }

    qDebug() << "map clicked with card" << cardId;

    Shop *shop = nullptr;
    const QList<QGraphicsItem*> allItems = scene()->items();
    for (QGraphicsItem *item : allItems)
    {
        shop = dynamic_cast<Shop*>(item);
        if (shop)
            break;
    }
    if (!shop)
    {
        qDebug() << "plant failed: shop not found";
        return;
    }

    const QString plantName = Card::mapping[cardId];
    const int plantCost = Card::cost[plantName];
    if (shop->coin_val < plantCost)
    {
        m_gridState = 0;
        Card::card_activate = false;
        qDebug() << "plant failed: not enough coins" << shop->coin_val << plantCost;
        return;
    }

    const QPointF scenePos = mapToScene(event->pos());
    const int plantX = 250 + qRound((scenePos.x() - 250) / 110.0) * 110;
    const int plantY = 150 + qRound((scenePos.y() - 150) / 110.0) * 110;

    for (QGraphicsItem *item : scene()->items())
    {
        if (dynamic_cast<Paotai*>(item)
            && qAbs(item->x() - plantX) < 10
            && qAbs(item->y() - plantY) < 10)
        {
            Card::card_activate = false;
            qDebug() << "plant failed: occupied" << plantX << plantY;
            return;
        }
    }

    Paotai *newPlant = nullptr;
    switch (cardId)
    {
    case 0: newPlant = new CoinMaker; break;
    case 1: newPlant = new Shooter; break;
    case 2: newPlant = new Protecter; break;
    case 3: newPlant = new Bomb; break;
    default:
        qDebug() << "invalid plant id" << cardId;
        Card::card_activate = false;
        return;
    }

    shop->coin_val -= plantCost;
    shop->update();

    for (QGraphicsItem *item : scene()->items(QPointF(Card::pos[cardId])))
    {
        if (Card *card = dynamic_cast<Card *>(item))
        {
            card->state = 0;
            card->counter = 0;
            break;
        }
    }
    Card::card_activate = false;

    newPlant->setPos(plantX, plantY);
    newPlant->setZValue(2);
    scene()->addItem(newPlant);
    qDebug() << "plant placed" << plantName << plantX << plantY;
}

QRectF Map::boundingRect() const
{
    if (m_gridType == 1)
        return QRectF(-55, -55, 110, 110);

    return QRectF(0, 0, WIDTH, HEIGHT);
}
