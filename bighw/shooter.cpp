#include "shooter.h"
#include "bullet.h"
#include "button.h"
#include "enemy.h"

Shooter::Shooter()
{
    hp = 100;
    maxHp = 100;
    placementCost = 100;
    state = 1;
    baseTime = 100;
    time = baseTime;
    counter = 0;
    baseAtk = 20;
    atk = baseAtk;
    powerCounter = 0;
    setMovie(":/resources/tech_shooter.png");
}

void Shooter::advance(int phase)
{
    if (Button::shovel_activate || Button::power_activate)
        setCursor(Qt::PointingHandCursor);
    else
        setCursor(Qt::ArrowCursor);

    if (!phase) return;
    update();

    if (hp <= 0)
    {
        delete this;
        return;
    }

    if (state == 2 && ++powerCounter > 500)
    {
        state = 1;
        powerCounter = 0;
        time = baseTime;
        atk = baseAtk;
        if (movie)
            movie->setSpeed(50);
    }

    if (++counter < time)
        return;

    counter = 0;
    const QList<QGraphicsItem*> items = scene()->items();
    for (QGraphicsItem *item : items)
    {
        Enemy *enemy = qgraphicsitem_cast<Enemy*>(item);
        if (!enemy)
            continue;

        if (enemy->x() <= x() || qAbs(enemy->y() - y()) >= 45)
            continue;

        Bullet* bullet = new Bullet(atk, NO_SNOW);
        bullet->setPos(x() + 45, y()+15);
        bullet->setZValue(60);
        scene()->addItem(bullet);
        return;
    }
}

bool Shooter::collidesWithItem(const QGraphicsItem *other, Qt::ItemSelectionMode mode) const
{
    Q_UNUSED(mode)
    return other->type() == Enemy::Type
           && other->x() > x()
           && qAbs(other->y() - y()) < 45;
}

void Shooter::mousePressEvent(QGraphicsSceneMouseEvent *event)
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
        state = 2;
        powerCounter = 0;
        counter = 0;
        time = 8;
        atk = baseAtk + 15;
        if (movie)
            movie->setSpeed(100);
        hideUpgradeButton();
        return;
    }

    handleUpgradeClick(event);
}

bool Shooter::upgrade()
{
    if (!Paotai::upgrade())
        return false;

    baseAtk += 12;
    baseTime = qMax(45, baseTime - 18);
    if (state != 2)
    {
        atk = baseAtk;
        time = baseTime;
    }
    return true;
}
