#ifndef FOOD_H
#define FOOD_H

#include <QObject>
#include <QBrush>
#include <QGraphicsRectItem>


class Food :public QObject, public QGraphicsRectItem
{
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
    Q_PROPERTY(QRectF rect READ rect WRITE setRect)

public:
    Food(qreal w, qreal h, QGraphicsItem *parent = nullptr);

    enum{Type = UserType+1};
    int type() const;

    void implodeOnEating();
private:
    bool m_implosion;


};

#endif // FOOD_H
