#ifndef LOADINGMACHINE_H
#define LOADINGMACHINE_H

#include <QPainter>
#include <QPaintEvent>
#include <QPainter>
#include <QDebug>
#include <QPoint>
#include <QColor>
#include <QPen>
#include <QObject>
#include <QFont>

class LoadingMachine
{

public:
    int co_x ; //x坐标
    int co_z ; //z坐标
    int block_x; //区块坐标
    int block_z;
    int strong_load_range ; //强加载范围
    int weak_load_range ;  //弱加载范围

    int id;

public:
    LoadingMachine();
    LoadingMachine(int cx,int cz);
    LoadingMachine(int cx,int cz, int wr, int sr);
    LoadingMachine(int id,int cx,int cz, int wr, int sr);
    bool isOutrange(int sx, int sy , double scale); //通过左上角坐标判断是否超出屏幕范围
    void show(QPainter* p , double offset_x , double offset_z , double scale); //把自己画出来，需要获悉地图偏移量和比例尺
};

#endif // LOADINGMACHINE_H
