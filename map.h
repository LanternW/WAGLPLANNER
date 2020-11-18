#ifndef MAP_H
#define MAP_H

#include <QMainWindow>
#include <QPaintEvent>
#include <QPainter>
#include <QDebug>
#include <QPoint>
#include <QColor>
#include <QPen>
#include <QObject>
#include <QEvent>
#include <QFont>
#include <QList>
#include <QStandardItemModel>
#include <QStandardItem>
#include "loadingmachine.h"
#include "landmark.h"


class Map
{
public:
    static QPoint coordinateToBlock(int cox , int coz);  //坐标转换为区块坐标
public:
    double scale = 1.0 ; //比例尺, 比例尺为1时 ， 屏幕像素坐标10 = minecraft中1米
    double offset_x = 0 ; //偏移量x,表示屏幕正中心的世界坐标
    double offset_z = 0; //偏移量z

    QList<LoadingMachine> loadmachine_list;
    QList<Landmark> landmark_list;

    LoadingMachine* getMachineById(int id);
    bool isExistLoadingMachine(int id);

    Landmark* getMarkById(int id);
    bool isExistLandMark(int id);

    QStringList lmchstr;
    QStringList ldmkstr;



public:
    Map();
    void generateGrid(QPainter* p , QPoint* mouse_pos, int type);
    void addLoadingMachine(LoadingMachine m);
    void addLandMark(Landmark l);

    void deleteLoadingMachine(int id);
    void deleteLandMark(int id);

    void clear();

    int g(int dx ,int a); //辅助计算函数
};

#endif // MAP_H
