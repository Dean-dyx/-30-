#include "paotai.h"
#include "enemy.h"
#include "shop.h"

Paotai::Paotai()
{
    hp = 0;
    maxHp = 0;
    level = 1;
    placementCost = 75;
    state = 0;
    movie = nullptr;
    atk = 0;
    counter = 0;
    time = 0;
    upgradeButtonVisible = false;
}

Paotai::~Paotai()
{
    delete movie;
}

QRectF Paotai::boundingRect() const
{
    return QRectF(-48, -74, 96, 112);
}

QPainterPath Paotai::shape() const
{
    QPainterPath path;
    path.addRect(QRectF(-35, -35, 70, 70));
    path.addRect(QRectF(-42, -70, 84, 20));
    return path;
}

void Paotai::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    if (!staticPixmap.isNull())
        painter->drawPixmap(QRectF(-35, -35, 70, 70).toRect(), staticPixmap);
    else if (movie)
        painter->drawImage(QRectF(-35, -35, 70, 70), movie->currentImage());

    const qreal ratio = maxHp > 0 ? qBound<qreal>(0, hp / maxHp, 1) : 0;
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(80, 30, 30, 210));
    painter->drawRoundedRect(QRectF(-32, -47, 64, 6), 2, 2);
    painter->setBrush(QColor(70, 210, 80, 230));
    painter->drawRoundedRect(QRectF(-32, -47, 64 * ratio, 6), 2, 2);

    painter->setPen(QColor(30, 50, 30));
    QFont font = painter->font();
    font.setPointSizeF(8);
    font.setBold(true);
    painter->setFont(font);
    painter->drawText(QRectF(-34, 30, 68, 14), Qt::AlignCenter,
                      QString("Lv%1").arg(level));

    if (upgradeButtonVisible)
    {
        const QRectF buttonRect(-42, -70, 84, 20);
        painter->setPen(QColor(70, 100, 50));
        painter->setBrush(level >= 3 ? QColor(190, 190, 190, 230)
                                     : QColor(245, 225, 110, 240));
        painter->drawRoundedRect(buttonRect, 4, 4);
        painter->setPen(QColor(40, 45, 35));
        font.setPointSizeF(8);
        font.setBold(true);
        painter->setFont(font);
        painter->drawText(buttonRect, Qt::AlignCenter,
                          level >= 3 ? "MAX" : QString("UP %1").arg(upgradeCost()));
    }
}

bool Paotai::collidesWithItem(const QGraphicsItem* other, Qt::ItemSelectionMode mode) const
{
    Q_UNUSED(mode)
    return qAbs(other->x() - x()) <= 45
           && qAbs(other->y() - y()) <= 45
           && other->type() == Enemy::Type;
}

void Paotai::setMovie(QString path)
{
    if (movie)
    {
        delete movie;
        movie = nullptr;
    }
    staticPixmap = QPixmap();

    if (path.endsWith(".png", Qt::CaseInsensitive))
    {
        staticPixmap.load(path);
        if (staticPixmap.isNull())
            qDebug() << "tower image loading failed:" << path;
        update();
        return;
    }

    movie = new QMovie(path);
    movie->setCacheMode(QMovie::CacheAll);
    if (!movie->isValid())
        qDebug() << "tower movie loading failed:" << path;

    movie->setSpeed(50);
    movie->start();
}

int Paotai::type() const
{
    return Type;
}

int Paotai::upgradeCost() const
{
    return level >= 3 ? 0 : placementCost * level;
}

bool Paotai::upgrade()
{
    if (level >= 3)
    {
        qDebug() << "upgrade failed: max level";
        return false;
    }

    const int cost = upgradeCost();
    if (!spendCoins(cost))
    {
        qDebug() << "upgrade failed: not enough coins" << cost;
        return false;
    }

    level++;
    maxHp += 40;
    hp = qMin(maxHp, hp + 40);
    qDebug() << "tower upgraded to level" << level;
    update();
    return true;
}

bool Paotai::handleUpgradeClick(QGraphicsSceneMouseEvent* event)
{
    if (!event || event->button() != Qt::LeftButton)
        return false;

    const QRectF buttonRect(-42, -70, 84, 20);
    if (upgradeButtonVisible && buttonRect.contains(event->pos()))
    {
        upgrade();
        hideUpgradeButton();
        return true;
    }

    showUpgradeButton();
    return true;
}

void Paotai::showUpgradeButton()
{
    if (!scene())
        return;

    const QList<QGraphicsItem*> items = scene()->items();
    for (QGraphicsItem* item : items)
    {
        Paotai* tower = dynamic_cast<Paotai*>(item);
        if (tower && tower != this)
            tower->hideUpgradeButton();
    }

    upgradeButtonVisible = true;
    update();
}

void Paotai::hideUpgradeButton()
{
    if (!upgradeButtonVisible)
        return;

    upgradeButtonVisible = false;
    update();
}

Shop* Paotai::findShop() const
{
    if (!scene())
        return nullptr;

    const QList<QGraphicsItem*> items = scene()->items();
    for (QGraphicsItem* item : items)
    {
        if (Shop* shop = dynamic_cast<Shop*>(item))
            return shop;
    }
    return nullptr;
}

bool Paotai::spendCoins(int amount) const
{
    Shop* shop = findShop();
    if (!shop || shop->coin_val < amount)
        return false;

    shop->coin_val -= amount;
    shop->update();
    return true;
}
