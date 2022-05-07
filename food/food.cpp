#include "food.h"
#include <QRandomGenerator>
#include <QDebug>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>

Food::Food(qreal w, qreal h, QGraphicsItem* parent)
    :QGraphicsRectItem(parent)
    ,m_implosion(false)
{
    QRandomGenerator *r = new QRandomGenerator();
    qint32 random_x = r->global()->bounded(0, (int)w);
    qint32 random_y = r->global()->bounded(0, (int)h);
    delete r;
    setRect(random_x, random_y, 10,10);
    setBrush(Qt::yellow);

}

int Food::type() const
{
 return Type;
}

void Food::implodeOnEating()
{
    if(m_implosion){
        return;
    }

    m_implosion = true;
    QParallelAnimationGroup *group = new QParallelAnimationGroup(this);

    QPropertyAnimation *scaleAnimation = new QPropertyAnimation(this, "rect");
    scaleAnimation->setDuration(1000);
    QRectF r = rect();
    scaleAnimation->setStartValue(r);
    scaleAnimation->setEndValue(QRectF(r.topLeft(), r.size()*0));
    scaleAnimation->setEasingCurve(QEasingCurve::OutQuart);
    group->addAnimation(scaleAnimation);

    QPropertyAnimation *fadeAnimation = new QPropertyAnimation(this, "opacity");
    fadeAnimation->setDuration(700);
    fadeAnimation->setStartValue(1);
    fadeAnimation->setEndValue(0);
    fadeAnimation->setEasingCurve(QEasingCurve::Linear);
    group->addAnimation(fadeAnimation);

    connect(group, &QParallelAnimationGroup::finished,
            this, &Food::deleteLater);

//    connect(fadeAnimation, &QPropertyAnimation::finished,
//            this, &Food::deleteLater);

//    fadeAnimation->start();
    group->start();
}


