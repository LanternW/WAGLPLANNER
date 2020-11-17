#include "loadingmachine.h"
#include "mainwindow.h"
#include "transformer.h"

LoadingMachine::LoadingMachine()
{
    this->co_x = 0;
    this->co_z = 0;
    this->block_x = 0;
    this->block_z = 0;
    this->strong_load_range = 3;
    this->weak_load_range = 5;

    this->id = -1;
}

LoadingMachine::LoadingMachine(int cx,int cz)
{
    this->co_x = cx;
    this->co_z = cz;
    this->block_x = cx/16;
    this->block_z = cz/16;
    this->strong_load_range = 3;
    this->weak_load_range = 5;

    this->id = -1;
}

LoadingMachine::LoadingMachine(int cx,int cz, int wr, int sr)
{
    this->co_x = cx;
    this->co_z = cz;
    this->block_x = cx/16;
    this->block_z = cz/16;
    this->strong_load_range = sr;
    this->weak_load_range = wr;

    this->id = -1;
}

LoadingMachine::LoadingMachine(int id,int cx,int cz, int wr, int sr)
{
    this->co_x = cx;
    this->co_z = cz;
    this->block_x = cx/16;
    this->block_z = cz/16;
    this->strong_load_range = sr;
    this->weak_load_range = wr;

    this->id = id;
}
bool LoadingMachine::isOutrange(int sx, int sy , double scale)
{
    if(sx + 160*  scale + this->weak_load_range < 0 || sy + 160*  scale + this->weak_load_range < 0)
        return true;
    else if(sx > MainWindow::widget_l || sy > MainWindow::widget_h)
        return true;
    return false;
}

void LoadingMachine::show(QPainter* p , double offset_x , double offset_z  , double scale)
{


    double sx = this->block_x * 16 - int(this-> weak_load_range / 2 ) * 16 ; //左上角的世界坐标
    double sz = this->block_z * 16 - int(this-> weak_load_range / 2 ) * 16 ; //左上角的世界坐标
    QPoint pos = Transformer::worldToScreen(offset_x,offset_z, sx,sz,scale);

    int sx_ = pos.rx();
    int sz_ = pos.ry();
    //if(!isOutrange(sx_,sz_,scale))
    //{
        //弱加载区块

        p->setPen(QPen());
        p->setBrush(QBrush(QColor(250,250,220), Qt::Dense3Pattern));
        p->drawRect(QRect(sx_,sz_ , 160*scale*weak_load_range , 160*scale*weak_load_range));

        //强加载区块
        sx = this->block_x * 16 - int(this-> strong_load_range / 2 ) * 16 ; //左上角的世界坐标
        sz = this->block_z * 16 - int(this-> strong_load_range / 2 ) * 16 ; //左上角的世界坐标
        pos = Transformer::worldToScreen(offset_x,offset_z, sx,sz,scale);
        sx_ = pos.rx();
        sz_ = pos.ry();
        p->setBrush(QBrush(QColor(250,220,220), Qt::SolidPattern));
        p->drawRect(QRect(sx_,sz_ , 160*scale*strong_load_range , 160*scale*strong_load_range));
    //}


}
