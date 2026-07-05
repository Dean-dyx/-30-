#ifndef MAP_H
#define MAP_H

#include "card.h"
#include "other.h"
#include "paotai.h"
#include "shop.h"

 //Map 类：代表游戏地图上的一个格子

class Map : public other
{
public:
// type 格子类型（0=普通草地，可扩展：1=水池、2=屋顶等）

    Map(int type);

    //每帧更新逻辑
    //图形项更新阶段（0=准备，1=执行）

    void advance(int phase) override;

  //绘制格子背景图（草地纹理）

    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;
    //鼠标左键点击事件：处理种植物、铲子移除等

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;


//定义格子的点击/碰撞范围

    QRectF boundingRect() const override;

protected:
    int m_gridState;  ///< 格子状态：0=空（可种），1=已种植物（不可种）
    int m_gridType;   ///< 格子类型：0=普通草地，1=水池，2=屋顶（预留扩展）
};

#endif // MAP_H