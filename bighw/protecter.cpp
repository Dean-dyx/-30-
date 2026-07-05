#include "protecter.h"
#include "button.h"

Protecter::Protecter()
{
    hp = 450;
    maxHp = 450;
    placementCost = 50;
    atk = 0;
    state = 1;
    powered = false;
    powerCounter = 0;
    setMovie(":/resources/tech_protecter.png");
}

void Protecter::advance(int phase)
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

    if (powered)
    {
        hp = qMin<qreal>(maxHp, hp + 8.0);
        if (++powerCounter > 240 || hp >= maxHp)
        {
            powered = false;
            powerCounter = 0;
        }
    }

    const qreal hpRatio = maxHp > 0 ? hp / maxHp : 0;
    const int nextState = hpRatio <= 0.25 ? 3 : (hpRatio <= 0.55 ? 2 : 1);
    if (nextState != state)
    {
        state = nextState;
        if (state == 1)
            setMovie(":/resources/tech_protecter.png");
        else if (state == 2)
            setMovie(":/resources/tech_protecter.png");
        else
            setMovie(":/resources/tech_protecter.png");
    }
}

void Protecter::mousePressEvent(QGraphicsSceneMouseEvent *event)
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
        hp = qMin<qreal>(maxHp, hp + 120);
        powered = true;
        powerCounter = 0;
        hideUpgradeButton();
        return;
    }

    handleUpgradeClick(event);
}

bool Protecter::upgrade()
{
    if (!Paotai::upgrade())
        return false;

    maxHp += 120;
    hp = qMin<qreal>(maxHp, hp + 160);
    return true;
}
