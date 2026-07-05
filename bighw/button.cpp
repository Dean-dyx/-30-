#include "button.h"
#include "enemy.h"

bool Button::power_activate = false;
bool Button::shovel_activate = false;

Button::Button(int t)
{
    counter = 0;
    cool_time = 100;
    state = 0;
    type = t;
}

void Button::advance(int phase)
{
    if (!phase) return;
    update();

    if (state)
    {
        setCursor(Qt::PointingHandCursor);
        return;
    }

    setCursor(Qt::ArrowCursor);
    if (++counter > cool_time)
        state = 1;
}

void Button::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    static const QPixmap shovelPixmap(":/resources/Shovel.png");
    painter->setRenderHint(QPainter::Antialiasing, true);

    if (type == FREEZE || type == POWER)
    {
        const QRectF rect(-80, -20, 160, 40);
        const bool active = (type == POWER && power_activate);
        painter->setPen(QPen(active ? QColor(255, 230, 95) : QColor(55, 220, 255), 1.4));
        painter->setBrush(active ? QColor(75, 60, 15, 230) : QColor(8, 42, 66, 230));
        painter->drawRoundedRect(rect, 5, 5);

        painter->setPen(QPen(QColor(130, 255, 255, 80), 1));
        painter->drawLine(QPointF(rect.left() + 10, rect.top() + 7),
                          QPointF(rect.right() - 10, rect.top() + 7));

        QFont font("Calibri", 18, QFont::Bold, true);
        painter->setFont(font);
        painter->setPen(type == FREEZE ? QColor(140, 235, 255) : QColor(120, 255, 150));
        painter->drawText(rect, Qt::AlignCenter, type == FREEZE ? "FREEZE" : "POWER");

        if (counter < cool_time)
        {
            painter->setPen(Qt::NoPen);
            painter->setBrush(QColor(0, 0, 0, 175));
            painter->drawRect(QRectF(-78, -19, 156, 38 * (1 - qreal(counter) / cool_time)));
        }
        return;
    }

    if (type == SHOVEL)
    {
        painter->setPen(QPen(QColor(55, 220, 255), 1.2));
        painter->setBrush(QColor(8, 42, 66, 220));
        painter->drawRoundedRect(QRectF(-40, -40, 80, 80), 5, 5);
        painter->drawPixmap(QRect(-30, -30, 60, 60), shovelPixmap);

        if (counter < cool_time)
        {
            painter->setPen(Qt::NoPen);
            painter->setBrush(QColor(0, 0, 0, 175));
            painter->drawRect(QRectF(-40, -38, 80, 76 * (1 - qreal(counter) / cool_time)));
        }
    }
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (!state || event->button() != Qt::LeftButton)
        return;

    if (type == FREEZE)
    {
        if (shovel_activate || power_activate)
        {
            shovel_activate = false;
            power_activate = false;
            return;
        }

        counter = 0;
        state = 0;

        const QList<QGraphicsItem*> items = scene()->items();
        for (QGraphicsItem* item : items)
        {
            Enemy* enemy = qgraphicsitem_cast<Enemy*>(item);
            if (enemy)
            {
                enemy->slow = SLOW;
                enemy->counter = 0;
            }
        }
    }
    else if (type == POWER)
    {
        shovel_activate = false;
        power_activate = !power_activate;
        qDebug() << (power_activate ? "power" : "no_power");
    }
    else if (type == SHOVEL)
    {
        power_activate = false;
        shovel_activate = !shovel_activate;
        qDebug() << (shovel_activate ? "shovel_activate" : "shovel_not_activate");
    }
}

QRectF Button::boundingRect() const
{
    if (type == SHOVEL)
        return QRectF(-42, -36, 80, 72);

    return QRectF(-80, -20, 160, 40);
}

void Button::consumePower(QGraphicsScene* scene)
{
    power_activate = false;
    if (!scene)
        return;

    const QList<QGraphicsItem*> items = scene->items();
    for (QGraphicsItem* item : items)
    {
        Button* button = dynamic_cast<Button*>(item);
        if (button && button->type == POWER)
        {
            button->counter = 0;
            button->state = 0;
            button->update();
            return;
        }
    }
}
