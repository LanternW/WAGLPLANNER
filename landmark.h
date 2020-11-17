#ifndef LANDMARK_H
#define LANDMARK_H


#include <QPainter>
#include <QPaintEvent>
#include <QPainter>
#include <QDebug>
#include <QPoint>
#include <QColor>
#include <QPen>
#include <QObject>
#include <QFont>
#include <QRectF>

class Landmark
{

public:
    double co_x ; //x坐标
    double co_z ; //z坐标
    int block_x; //区块坐标
    int block_z;
    QString name; //地标名称

    int id;
public:
    Landmark();
    Landmark(int cx,int cz,QString name);
    Landmark(int id, int cx,int cz,QString name);
    void show(QPainter* p , double offset_x , double offset_z , double scale); //render itself according to the map information
};

#endif // LANDMARK_H
