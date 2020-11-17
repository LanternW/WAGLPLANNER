#ifndef TRANSFORMER_H
#define TRANSFORMER_H

#include <QPointF>
#include <QPoint>

class Transformer
{
public:
    Transformer();
    static QPoint worldToScreen(double offset_x , double offset_z , double sx,double sz, double scale);
    static QPointF screenToWorld(double offset_x , double offset_z , int sx,int sz, double scale);
};

#endif // TRANSFORMER_H
