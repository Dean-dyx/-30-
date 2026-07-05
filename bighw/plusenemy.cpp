#include "plusenemy.h"
#include "button.h"

PlusEnemy::PlusEnemy()
{
    hp = 350;
    maxHp = 350;
    atk = 0.5;
    state = WALK;
    speed = 0.7;
    slow = NO_SLOW;
    time = 120;
    counter = 0;
    difficultyValue = 3;
    rangedCounter = 0;

    SetMovie(":/resources/tech_enemy_warrior_plus.png");
}

void PlusEnemy::advance(int phase)
{
    if (!phase) return;
    update();

    setCursor(Button::power_activate ? Qt::PointingHandCursor : Qt::ArrowCursor);

    if (hp <= 0)
    {
        if (state != DEAD)
        {
            state = DEAD;
            if (!scoreAwarded)
            {
                Enemy::totalScore += difficultyValue;
                scoreAwarded = true;
            }
            speed = 0;
            slow = NO_SLOW;
            counter = 0;
            setOpacity(0.55);
        }

        if (++counter > 8)
            delete this;
        return;
    }

    if (super)
    {
        if (movie)
            movie->setSpeed(250);
        speed = 2.5;
        atk = 0.8;
    }

    attackBackline();

    const QList<QGraphicsItem*> items = collidingItems();
    for (QGraphicsItem *item : items)
    {
        Paotai *plant = qgraphicsitem_cast<Paotai *>(item);
        if (!plant)
            continue;

        plant->hp -= atk;
        if (state != ATTACK)
        {
            SetMovie(":/resources/tech_enemy_warrior_plus.png");
            state = ATTACK;
        }
        return;
    }

    if (state == ATTACK)
    {
        state = WALK;
        SetMovie(":/resources/tech_enemy_warrior_plus.png");
    }

    if (slow)
    {
        if (++counter > time)
        {
            snowfree();
            return;
        }

        if (slow == 1)
        {
            if (movie)
                movie->setSpeed(50);
            speed = 0.35;
        }
        else if (slow == 2)
        {
            if (movie)
                movie->setSpeed(0);
            speed = 0;
        }
    }

    if (x() < 195)
    {
        Enemy::gameOver = true;
        delete this;
        return;
    }

    setX(x() - speed);
}

void PlusEnemy::snowfree()
{
    slow = NO_SLOW;
    counter = 0;
    speed = super ? 2.5 : 0.7;
    if (movie)
        movie->setSpeed(super ? 250 : 100);
}

void PlusEnemy::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event)
}

void PlusEnemy::attackBackline()
{
    if (!scene() || state == DEAD)
        return;

    if (++rangedCounter < 135)
        return;
    rangedCounter = 0;

    Paotai* target = nullptr;
    const QList<QGraphicsItem*> items = scene()->items();
    for (QGraphicsItem* item : items)
    {
        Paotai* tower = qgraphicsitem_cast<Paotai*>(item);
        if (!tower)
            continue;

        if (qAbs(tower->y() - y()) >= 45)
            continue;

        if (tower->x() >= x())
            continue;

        if (!target || tower->x() < target->x())
            target = tower;
    }

    if (target)
    {
        target->hp -= 8;
        if (state != ATTACK)
        {
            SetMovie(":/resources/tech_enemy_warrior_plus.png");
            state = ATTACK;
        }
    }
}
