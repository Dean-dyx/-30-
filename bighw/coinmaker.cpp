#include "coinmaker.h"
#include "button.h"
#include "shop.h"

CoinMaker::CoinMaker()
{
    hp = 100;
    maxHp = 100;
    placementCost = 50;
    state = 1;
    time = 650;
    counter = 0;
    atk = 0;
    income = 30;
    powered = false;
    powerCounter = 0;
    setMovie(":/resources/tech_coinmaker.png");
}

void CoinMaker::advance(int phase)
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
        if (++powerCounter % 35 == 0)
            addCoins(income);

        if (powerCounter > 260)
        {
            powered = false;
            powerCounter = 0;
        }
    }

    if (state && ++counter >= time)
    {
        counter = 0;
        addCoins(income);
    }
}

void CoinMaker::mousePressEvent(QGraphicsSceneMouseEvent *event)
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
        Button::consumePower(scene());
        powered = true;
        powerCounter = 0;
        hideUpgradeButton();
        return;
    }

    handleUpgradeClick(event);
}

bool CoinMaker::upgrade()
{
    if (!Paotai::upgrade())
        return false;

    income += 20;
    time = qMax(430, time - 80);
    return true;
}

void CoinMaker::addCoins(int amount)
{
    if (Shop* shop = findShop())
    {
        shop->coin_val += amount;
        shop->update();
    }
}
