#include "enemy.h"
#include "paotai.h"
#include <QDebug>

int Enemy::totalScore = 0;
bool Enemy::gameOver = false;

Enemy::Enemy()
{
    movie = nullptr;
    head = nullptr;
    hp = 0;
    maxHp = 0;
    atk = 0;
    state = WALK;
    slow = NO_SLOW;
    speed = 0.0;
    time = 0;
    counter = 0;
    difficultyValue = 1;
    scoreAwarded = false;
    super = false;
}

Enemy::~Enemy()
{
    delete head;
    delete movie;
}

QRectF Enemy::boundingRect() const
{
    return QRectF(-70, -108, 140, 148);
}

void Enemy::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    const QRectF drawRect(-70, -100, 140, 140);

    if (!headPixmap.isNull())
        painter->drawPixmap(drawRect.toRect(), headPixmap);
    else if (head)
        painter->drawImage(drawRect, head->currentImage());

    if (!staticPixmap.isNull())
        painter->drawPixmap(drawRect.toRect(), staticPixmap);
    else if (movie)
        painter->drawImage(drawRect, movie->currentImage());

    if (slow)
    {
        painter->save();
        painter->setCompositionMode(QPainter::CompositionMode_SourceAtop);
        painter->fillRect(drawRect, slow == 2 ? QColor(90, 190, 255, 120)
                                              : QColor(90, 150, 255, 75));
        painter->restore();
    }

    const qreal ratio = maxHp > 0 ? qBound<qreal>(0, hp / maxHp, 1) : 0;
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(80, 25, 25, 210));
    painter->drawRoundedRect(QRectF(-38, -104, 76, 7), 2, 2);
    painter->setBrush(QColor(210, 55, 55, 235));
    painter->drawRoundedRect(QRectF(-38, -104, 76 * ratio, 7), 2, 2);
}

bool Enemy::collidesWithItem(const QGraphicsItem* other, Qt::ItemSelectionMode mode) const
{
    Q_UNUSED(mode)
    return qAbs(other->x() - x()) <= 45
           && qAbs(other->y() - y()) <= 45
           && other->type() == Paotai::Type;
}

int Enemy::type() const
{
    return Type;
}

void Enemy::SetMovie(const QString &path)
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
            qDebug() << "Image loading failed:" << path;
        update();
        return;
    }

    movie = new QMovie(path);
    movie->setCacheMode(QMovie::CacheAll);

    if (!movie->isValid())
    {
        qDebug() << "Movie loading failed:" << path;
        return;
    }

    if (super)
    {
        movie->setSpeed(300);
        speed = 3.0;
    }

    movie->start();
}

void Enemy::Sethead(const QString &path)
{
    if (head)
    {
        delete head;
        head = nullptr;
    }
    headPixmap = QPixmap();

    if (path.endsWith(".png", Qt::CaseInsensitive))
    {
        headPixmap.load(path);
        if (headPixmap.isNull())
            qDebug() << "Head image loading failed:" << path;
        update();
        return;
    }

    head = new QMovie(path);
    head->setCacheMode(QMovie::CacheAll);

    if (!head->isValid())
    {
        qDebug() << "Head movie loading failed:" << path;
        return;
    }

    if (super)
        head->setSpeed(300);

    head->start();
}
