#include "landmark.h"
#include "mainwindow.h"
#include "transformer.h"

Landmark::Landmark()
{
    this->co_x = 0;
    this->co_z = 0;
    this->block_x = 0;
    this->block_z = 0;

    this->id = -1;
}

Landmark::Landmark(int cx,int cz , QString name)
{
    this->co_x = cx;
    this->co_z = cz;
    this->block_x = cx/16;
    this->block_z = cz/16;
    this->name = name;

    this->id = -1;
}

Landmark::Landmark(int id, int cx,int cz , QString name)
{
    this->co_x = cx;
    this->co_z = cz;
    this->block_x = cx/16;
    this->block_z = cz/16;
    this->name = name;

    this->id = id;
}


void Landmark::show(QPainter* p , double offset_x , double offset_z , double scale)
{


    QPoint pos = Transformer::worldToScreen(offset_x,offset_z,this->co_x,this->co_z,scale);
    int sx_ = pos.rx();
    int sz_ = pos.ry();

    p->setPen(Qt::blue);
    p->drawPoint(QPoint(sx_,sz_));
    p->drawText(QPoint(sx_,sz_),this->name);

    p->drawPixmap(sx_ - 32 , sz_,32,32 , QPixmap(":/new/prefix1/imgs/landmark.png") );
}
