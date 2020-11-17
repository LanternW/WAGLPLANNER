#include "transformer.h"
#include "mainwindow.h"

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
