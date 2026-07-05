#ifndef CARD_H
#define CARD_H

#include "other.h"
#include "paotai.h"
#include "shop.h"
#include <QMap>

// 植物卡片类：商店里可点击的植物卡片
// 负责冷却、选中、消耗阳光判断、与地图交互种植
class Card : public other
{
public:
    // ===================== 静态全局卡片配置 =====================
    static const QMap<QString, int> cool;       // 每种卡片的冷却时间
    static const QMap<QString, int> id;         // 炮台名 → 卡片ID
    static const QMap<int, QPoint> pos;         // 卡片ID → 界面显示位置
    static const QMap<QString, int> cost;       // 炮台名 → 所需金币
    static const QMap<int, QString> mapping;    // 卡片ID → 植物名（配对用）

    // ===================== 静态全局状态 =====================
    static bool card_activate;   // 全局：是否有卡片被选中（准备种植）
    static int  card_id;         // 全局：当前选中的卡片ID

public:
    // 构造函数：传入卡片对应的植物名称
    Card(const QString& name_);

    // 每帧更新：冷却倒计时、状态刷新
    void advance(int phase) override;

    // 绘制卡片：图片、冷却遮罩、金币数字
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    // 卡片的点击范围
    QRectF boundingRect() const override;

    // 鼠标点击：选中卡片、开始冷却
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

public:
    int     state;      // 卡片状态 0=可用 1=冷却中
    int     counter;    // 冷却计时器
    QString name;       // 卡片对应的炮台名称
};

#endif // CARD_H