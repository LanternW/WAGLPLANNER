#include "map.h"
#include "mainwindow.h"
#include "transformer.h"
#include <QPainter>

Map::Map()
{
    addLoadingMachine(LoadingMachine(0,0,5,3));
    addLandMark(Landmark(0,0,QString("世界原点")));
    //loadmachine_list.append(LoadingMachine(0,0,3,5)); //出生点强加载
    //landmark_list.append(Landmark(0,0,QString("世界原点")));
}

int Map:: g(int dx , int a)
{
    int k = a - dx % a;
    return k;
}

void Map::addLoadingMachine(LoadingMachine m)
{

    for (int j = 1; j < 100 ; j++) {
        if(!isExistLoadingMachine(j))
        { m.id = j; break;}
    }
    loadmachine_list.append(m);
    lmchstr.append(QString("%1. ( %2 , %3 ) , w:%4 | s:%5").arg(m.id).arg(m.co_x).arg(m.co_z).arg(m.weak_load_range).arg(m.strong_load_range));

}

void Map::addLandMark(Landmark l)
{
    for (int j = 1; j < 100 ; j++) {
        if(!isExistLandMark(j))
        { l.id = j; break;}
    }
    landmark_list.append(l);
    ldmkstr.append(QString("%1. ( %2 , %3 );%4").arg(l.id).arg(l.co_x).arg(l.co_z).arg(l.name));
}

void Map::deleteLoadingMachine(int id)
{
    LoadingMachine m = *getMachineById(id);
    int index = -1;
    for (int i = 0; i<loadmachine_list.length();i++) {
        if(loadmachine_list[i].id == m.id){index = i;}
    }

    if(index != -1){loadmachine_list.removeAt(index);}
    lmchstr.removeOne(QString("%1. ( %2 , %3 ) , w:%4 | s:%5").arg(m.id).arg(m.co_x).arg(m.co_z).arg(m.weak_load_range).arg(m.strong_load_range));

}


void Map::deleteLandMark(int id)
{
    Landmark l = *getMarkById(id);
    int index = -1;
    for (int i = 0; i<landmark_list.length();i++) {
        if(landmark_list[i].id == l.id){index = i;}
    }
    ldmkstr.removeOne(QString("%1. ( %2 , %3 );%4").arg(l.id).arg(l.co_x).arg(l.co_z).arg(l.name));
    if(index != -1){landmark_list.removeAt(index);}
}

QPoint Map::coordinateToBlock(int cox, int coz)
{
    return QPoint(cox/16 , coz/16);
}

LoadingMachine* Map::getMachineById(int id)
{
    for (int i = 0 ; i < loadmachine_list.length(); i++) {
        if(loadmachine_list[i].id == id){return &loadmachine_list[i];}
    }
    return nullptr;
}
bool Map::isExistLoadingMachine(int id)
{
    for (int i = 0 ; i < loadmachine_list.length(); i++) {
        if(loadmachine_list[i].id == id){return true;}
    }
    return false;
}

Landmark* Map::getMarkById(int id)
{
    for (int i = 0 ; i < landmark_list.length(); i++) {
        if(landmark_list[i].id == id){return &landmark_list[i];}
    }
    return nullptr;
}
bool Map::isExistLandMark(int id)
{
    for (int i = 0 ; i < landmark_list.length(); i++) {
        if(landmark_list[i].id == id){return true;}
    }
    return false;
}

void Map::clear()
{
    for (int i = 0; i < loadmachine_list.length(); i++) {
        deleteLoadingMachine(loadmachine_list[i].id);
    }
    for (int i = 0; i < landmark_list.length(); i++) {
        deleteLandMark(landmark_list[i].id);
    }
    offset_x = 0;
    offset_z = 0;
    scale = 1;
}

void Map::generateGrid(QPainter* p ,QPoint* mouse_pos)
{
    int h0 = MainWindow::widget_h / 2;
    int l0 = MainWindow::widget_l / 2;

    int a_unit = 10 * this->scale; //单元格边长（单位：像素）
    int a_block = 16 * a_unit; //区块边长

    int delta_z = offset_z * scale * 10;
    int delta_x = offset_x * scale * 10;


    //绘制背景
    p->setPen(QColor(230,250,230));
    p->setBrush(QBrush(QColor(230,250,230), Qt::SolidPattern));
    p->drawRect(QRect(0,0,MainWindow::widget_l,MainWindow::widget_h));

    //绘制加载器
    for (int i = 0 ; i < loadmachine_list.length(); i++) {
       loadmachine_list[i].show(p,offset_x,offset_z, scale);
    }

    if(scale > 0.9)
    {
        p->setPen(QColor(230,230,230));
        int wz = int(offset_z) ; //距离屏幕中心世界坐标很近的一个整数坐标。
        int wx = int(offset_x) ;
        QPoint pos = Transformer::worldToScreen(offset_x,offset_z,wx,wz,scale);
        while (pos.rx() > 0 || pos.ry() > 0) {
            int y = pos.ry();
            int x = pos.rx();
            p->drawLine(QPoint(0,y) , QPoint(MainWindow::widget_l,y));
            p->drawLine(QPoint(x,0) , QPoint(x,MainWindow::widget_h));
            wz--;
            wx--;
            pos = Transformer::worldToScreen(offset_x,offset_z,wx,wz,scale);
        }

        wz = int(offset_z) ; //距离屏幕中心世界坐标很近的一个整数坐标。
        wx = int(offset_x) ;
        pos = Transformer::worldToScreen(offset_x,offset_z,wx,wz,scale);
        while (pos.rx() < MainWindow::widget_l || pos.ry() < MainWindow::widget_h) {
            int y = pos.ry();
            int x = pos.rx();
            p->drawLine(QPoint(0,y) , QPoint(MainWindow::widget_l,y));
            p->drawLine(QPoint(x,0) , QPoint(x,MainWindow::widget_h));
            wz++;
            wx++;
            pos = Transformer::worldToScreen(offset_x,offset_z,wx,wz,scale);
        }
    }

    p->setPen(QColor(170,170,170));
    int wz = 16 * (int(offset_z) / 16) ; //距离屏幕中心世界坐标很近的一个整数坐标。
    int wx = 16 * (int(offset_x) / 16) ;
    QPoint pos = Transformer::worldToScreen(offset_x,offset_z,wx,wz,scale);
    while (pos.rx() > 0 || pos.ry() > 0) {
        int y = pos.ry();
        int x = pos.rx();
        p->drawLine(QPoint(0,y) , QPoint(MainWindow::widget_l,y));
        p->drawLine(QPoint(x,0) , QPoint(x,MainWindow::widget_h));
        wz -= 16;
        wx -= 16;
        pos = Transformer::worldToScreen(offset_x,offset_z,wx,wz,scale);
    }

    wz = 16 * (int(offset_z) / 16) ; //距离屏幕中心世界坐标很近的一个整数坐标。
    wx = 16 * (int(offset_x) / 16) ;
    pos = Transformer::worldToScreen(offset_x,offset_z,wx,wz,scale);
    while (pos.rx() < MainWindow::widget_l || pos.ry() < MainWindow::widget_h) {
        int y = pos.ry();
        int x = pos.rx();
        p->drawLine(QPoint(0,y) , QPoint(MainWindow::widget_l,y));
        p->drawLine(QPoint(x,0) , QPoint(x,MainWindow::widget_h));
        wz += 16;
        wx += 16;
        pos = Transformer::worldToScreen(offset_x,offset_z,wx,wz,scale);
    }


    //绘制地标
    for (int i = 0 ; i < landmark_list.length(); i++) {
       landmark_list[i].show(p,offset_x,offset_z,scale);
    }

    p->drawPixmap(MainWindow::widget_l/2 - 5 , MainWindow::widget_h/2 - 5 ,10,10 , QPixmap(":/new/prefix1/imgs/center.png") );



}
