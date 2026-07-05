#include "card.h"
#include "button.h"
#include <QDebug>

const QMap<QString, int> Card::id = {
    {"coinmaker", 0},
    {"shooter", 1},
    {"protecter", 2},
    {"bomb", 3}
};

const QMap<int, QPoint> Card::pos = {
    {0, QPoint(350, 45)},
    {1, QPoint(420, 45)},
    {2, QPoint(490, 45)},
    {3, QPoint(560, 45)}
};

const QMap<int, QString> Card::mapping = {
    {0, "coinmaker"},
    {1, "shooter"},
    {2, "protecter"},
    {3, "bomb"}
};

const QMap<QString, int> Card::cool = {
    {"coinmaker", 150},
    {"shooter", 200},
    {"protecter", 300},
    {"bomb", 450}
};

const QMap<QString, int> Card::cost = {
    {"coinmaker", 50},
    {"shooter", 100},
    {"protecter", 50},
    {"bomb", 250}
};

int Card::card_id = -1;
bool Card::card_activate = false;

Card::Card(const QString& name_)
{
    name = name_;
    counter = cool[name];
    state = 1;
}

void Card::advance(int phase)
{
    if (!phase) return;
    update();

    if (counter < cool[name])
    {
        counter++;
        state = 0;
        setCursor(Qt::ArrowCursor);
    }
    else
    {
        state = 1;
        setCursor(Qt::PointingHandCursor);
    }
}

void Card::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    static QMap<QString, QPixmap> iconCache;
    if (!iconCache.contains(name))
    {
        QString iconPath = ":/resources/" + name + ".png";
        if (name == "shooter") iconPath = ":/resources/tech_shooter.png";
        else if (name == "coinmaker") iconPath = ":/resources/tech_coinmaker.png";
        else if (name == "protecter") iconPath = ":/resources/tech_protecter.png";
        else if (name == "bomb") iconPath = ":/resources/tech_bomb.png";
        iconCache.insert(name, QPixmap(iconPath));
    }

    painter->scale(0.6, 0.58);
    const QRectF cardRect(-50, -70, 100, 140);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(QPen(QColor(45, 220, 255), 1.4));
    painter->setBrush(QColor(9, 32, 52, 230));
    painter->drawRoundedRect(cardRect, 5, 5);
    painter->setPen(QPen(QColor(120, 255, 230, 65), 1));
    painter->drawLine(QPointF(-42, -58), QPointF(42, -58));
    painter->drawLine(QPointF(-42, 40), QPointF(42, 40));
    painter->drawPixmap(QRect(-35, -42, 70, 70), iconCache[name]);

    QFont font;
    font.setPointSizeF(18);
    font.setBold(true);
    painter->setFont(font);
    painter->setPen(QColor(145, 255, 190));
    painter->drawText(QPoint(-30, 62), QString::number(cost[name]));

    if (counter < cool[name])
    {
        painter->setBrush(QColor(0, 0, 0, 200));
        painter->setPen(Qt::transparent);
        painter->drawRoundedRect(QRectF(-48, -68, 96, 132 * (1 - qreal(counter) / cool[name])), 4, 4);
    }
}

QRectF Card::boundingRect() const
{
    return QRectF(-20, -70, 40, 120);
}

void Card::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;

    if (Button::power_activate || Button::shovel_activate)
    {
        Button::power_activate = false;
        Button::shovel_activate = false;
        return;
    }

    if (!state)
        return;

    if (!card_activate)
    {
        card_activate = true;
        Card::card_id = id[name];
        qDebug() << "card selected" << name << "id" << card_id;
    }
    else
    {
        card_activate = false;
        Card::card_id = -1;
        qDebug() << "card selection canceled";
    }
}
