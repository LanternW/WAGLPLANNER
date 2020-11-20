#include "transformer.h"
#include "mainwindow.h"
#include <math.h>
Transformer::Transformer()
{

}

QPoint Transformer::worldToScreen(double offset_x , double offset_z , double sx,double sz, double scale)
{
    int sx_ = MainWindow::widget_l/2 + (sx - offset_x) * (10*scale);
    int sz_ = MainWindow::widget_h/2 + (sz - offset_z) * (10*scale);
    return QPoint(sx_,sz_);
}


QPointF Transformer::screenToWorld(double offset_x , double offset_z , int sx,int sz, double scale)
{
    double sx_ = offset_x + double(sx - MainWindow::widget_l/2) / (10*scale);
    double sz_ = offset_z + double(sz - MainWindow::widget_h/2) / (10*scale);
    if(sx_ < 0){sx_ -= 1;}
    if(sz_ < 0){sz_ -= 1;}
    return QPointF(sx_,sz_);
}

double Transformer::sigmoid(double t)
{
    if(t >= 80){return 1;}
    if(t <= 0.2){return 0;}
    else {
        return 0.5 * ((1 - 1 / (abs(t - 40) + 1) ) * ((t - 40) / abs(t - 40)) + 1);
    }
}

double Transformer::distance(QPoint pos1 , QPoint pos2)
{
    return sqrt(pow((pos1.rx() - pos2.rx()) , 2) + pow((pos1.ry() - pos2.ry()) , 2) );
}
