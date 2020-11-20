#ifndef TRANSFORMER_H
#define TRANSFORMER_H

#include <QPointF>
#include <QPoint>
#include <math.h>

class Transformer
{
public:
    Transformer();
    static QPoint worldToScreen(double offset_x , double offset_z , double sx,double sz, double scale);
    static QPointF screenToWorld(double offset_x , double offset_z , int sx,int sz, double scale);
    static double sigmoid(double t); //非线性动画生成器
    static double distance(QPoint pos1 , QPoint pos2);
};

#endif // TRANSFORMER_H
