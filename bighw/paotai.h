#ifndef PAOTAI_H
#define PAOTAI_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QMovie>
#include <QPixmap>
#include <QPainter>
#include <QPainterPath>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

#define NO_SNOW false
#define SNOW true

#define WIDTH 1400
#define HEIGHT 602

class Shop;

class Paotai : public QGraphicsItem
{
public:
    qreal hp;
    qreal maxHp;
    int level;
    int placementCost;
    int state;

    Paotai();
    ~Paotai() override;

    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    enum { Type = UserType + 1 };

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    bool collidesWithItem(const QGraphicsItem* other, Qt::ItemSelectionMode mode) const override;
    void setMovie(QString path);
    int type() const override;

    virtual int upgradeCost() const;
    virtual bool upgrade();
    bool handleUpgradeClick(QGraphicsSceneMouseEvent* event);
    void hideUpgradeButton();

protected:
    void showUpgradeButton();
    Shop* findShop() const;
    bool spendCoins(int amount) const;

    QMovie* movie;
    QPixmap staticPixmap;
    int atk;
    int counter;
    int time;
    bool upgradeButtonVisible;
};

#endif // PAOTAI_H
