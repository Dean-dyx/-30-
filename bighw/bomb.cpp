#include "bomb.h"
#include "button.h"
#include "enemy.h"
#include <QDebug>

Bomb::Bomb()
{
    hp = 1000;
    maxHp = 1000;
    placementCost = 250;
    atk = 400;
    state = 1;
    counter = 0;
    time = 50;
    exploded = false;
    rangeScale = 1.0;
    setMovie(":/resources/cb.gif");
    movie->setSpeed(150);
}

void Bomb::advance(int phase)
{
    if (!phase) return;
    update();

    if (Button::shovel_activate || Button::power_activate)
        setCursor(Qt::PointingHandCursor);
    else
        setCursor(Qt::ArrowCursor);

    if (hp <= 0)
    {
        delete this;
        return;
    }

    if (state == 1 && movie->currentFrameNumber() >= movie->frameCount() - 1)
    {
        explode();
        return;
    }

    if (state == 2 && movie->currentFrameNumber() >= movie->frameCount() - 1)
    {
        qDebug() << "bomb deleted";
        delete this;
    }
}


void Bomb::explode()
{
    if (exploded)
        return;

    exploded = true;
    state = 2;
    setMovie(":/resources/boom.gif");
    movie->setSpeed(rangeScale > 1.0 ? 140 : 100);
/*
    const QList<QGraphicsItem*> items = collidingItems();
    for (QGraphicsItem* item : items)
    {
        Enemy* enemy = qgraphicsitem_cast<Enemy*>(item);
        if (!enemy)
            continue;

        enemy->hp -= atk;
        if (enemy->hp <= 0)
            enemy->state = BURN;
    }
*/
    const QList<QGraphicsItem*> allItems = scene()->items();
    for (QGraphicsItem* item : allItems)
    {
        Enemy* enemy = qgraphicsitem_cast<Enemy*>(item);
        if (!enemy) continue;


        if (collidesWithItem(enemy, Qt::IntersectsItemShape))
        {
            enemy->hp -= atk;
            if (enemy->hp <= 0)
                enemy->state = BURN;
        }
    }
}

bool Bomb::collidesWithItem(const QGraphicsItem* other, Qt::ItemSelectionMode mode) const
{
    Q_UNUSED(mode)
    const qreal dx = other->x() - x();
    const qreal dy = other->y() - y();
    return other->type() == Enemy::Type
           && qAbs(dy) <= 1.5*GAP_Y * rangeScale
           && dx >= -2 * GAP_X * rangeScale
           && dx <= 1.5*GAP_X * rangeScale;
}

void Bomb::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;

    if (Button::shovel_activate)
    {
        Button::shovel_activate = false;
        delete this;
        return;
    }

    if (Button::power_activate)
    {
        if (QGraphicsItem* item = scene()->itemAt(QPointF(960, 40), QTransform()))
        {
            if (Button* button = qgraphicsitem_cast<Button*>(item))
            {
                button->counter = 0;
                button->state = 0;
            }
        }

        Button::consumePower(scene());
        spawnPoweredCopies();
        hideUpgradeButton();
        return;
    }

    if (state == 1)
        handleUpgradeClick(event);
}

QRectF Bomb::boundingRect() const
{
    if (state == 2)
    {
        const qreal size = 330 * rangeScale;
        return QRectF(-size / 2, -size / 2, size, size);
    }
    return Paotai::boundingRect();
}

bool Bomb::upgrade()
{
    if (!Paotai::upgrade())
        return false;

    atk += 180;
    rangeScale += 0.22;
    movie->setSpeed(qMin(120, movie->speed() + 25));
    return true;
}

void Bomb::spawnPoweredCopies()
{
    const QPointF offsets[] = {
        QPointF(-110, 0), QPointF(110, 0), QPointF(0, -110), QPointF(0, 110),
        QPointF(-110, -110), QPointF(110, -110), QPointF(-110, 110), QPointF(110, 110)
    };

    int created = 0;
    for (const QPointF& offset : offsets)
    {
        if (created >= 3)
            break;

        const QPointF target = pos() + offset;
        if (!canPlaceCopyAt(target))
            continue;

        Bomb* copy = new Bomb();
        copy->level = level;
        copy->placementCost = placementCost;
        copy->maxHp = maxHp;
        copy->hp = maxHp;
        copy->atk = atk;
        copy->rangeScale = rangeScale;
        copy->setPos(target);
        copy->setZValue(zValue());
        scene()->addItem(copy);
        created++;
    }
}

bool Bomb::canPlaceCopyAt(const QPointF& target) const
{
    if (!scene())
        return false;

    if (target.x() < 250 || target.x() > 1130 || target.y() < 150 || target.y() > 590)
        return false;

    const QList<QGraphicsItem*> items = scene()->items();
    for (QGraphicsItem* item : items)
    {
        if (dynamic_cast<Paotai*>(item)
            && qAbs(item->x() - target.x()) < 10
            && qAbs(item->y() - target.y()) < 10)
            return false;
    }

    return true;
}
