#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "transformer.h"
#include "addloadingmachine.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->widget->installEventFilter(this);
    ui->widget_2->installEventFilter(this);
    ui->widget_3->installEventFilter(this);

    world_main      = new Map();
    hell            = new Map();
    the_end         = new Map();

    world = world_main;

    QString sav = "saves/WAG.waglplanner";
    readLog(sav);

    updateListView(world);
}

void MainWindow::updateListView(Map* w)
{
    QStringListModel* model = new QStringListModel(w->lmchstr);
    ui->listView->setModel(model);

    QStringListModel* model2 = new QStringListModel(w->ldmkstr);
    ui->listView_2->setModel(model2);

}

void MainWindow::setMouseCoordinate(double x , double z)
{

    this->coordinate_x  = x;
    this->coordinate_z  = z;
}

QPoint *MainWindow::getMouseCoordinate()
{
    QPoint* pos = new QPoint(coordinate_x , coordinate_z);
    return pos;
}


void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter world_p(this);
}



bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{


    static bool left_buttton_down = 0; //左键按下
    static bool right_button_down = 0; //右键按下
    static int last_mouse_x = 0; //上一次鼠标位置
    static int last_mouse_z = 0;
    if(
            (watched == ui->widget    ||
             watched == ui->widget_2  ||
             watched == ui->widget_3) &&
            event->type() == QEvent::Paint  ){
        if(watched == ui->widget){
            widget1Paint(this->getMouseCoordinate() , right_button_down , solid_cor_x , solid_cor_z);}//主世界绘图
        if(watched == ui->widget_2){
            widget2Paint(this->getMouseCoordinate() , right_button_down , solid_cor_x , solid_cor_z);}//地狱绘图
        if(watched == ui->widget_3){
            widget3Paint(this->getMouseCoordinate() , right_button_down , solid_cor_x , solid_cor_z);}//末地绘图
    }

    if(watched == ui->widget  ||
            watched == ui->widget_2  ||
            watched == ui->widget_3    )   //地图面板
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            if(static_cast<QMouseEvent *>(event)->button() == Qt::LeftButton) //左键按下
            {
                left_buttton_down = 1;
                last_mouse_x = static_cast<QMouseEvent *>(event)->x();
                last_mouse_z = static_cast<QMouseEvent *>(event)->y();
            }

            if(static_cast<QMouseEvent *>(event)->button() == Qt::RightButton) //右键按下
            {
                right_button_down = 1;
                solid_cor_x = static_cast<QMouseEvent *>(event)->x();
                solid_cor_z = static_cast<QMouseEvent *>(event)->y();
            }
        }
        if(event->type() == QEvent::MouseButtonRelease)
        {
            if(static_cast<QMouseEvent *>(event)->button() == Qt::LeftButton) //左键释放
            {
                left_buttton_down = 0;
            }
            if(static_cast<QMouseEvent *>(event)->button() == Qt::RightButton) //左键释放
            {
                right_button_down = 0;
            }
        }

        if(event->type() == QEvent::MouseMove)
        {
            int the_x = static_cast<QMouseEvent *>(event)->x();
            int the_z = static_cast<QMouseEvent *>(event)->y();
            setMouseCoordinate( the_x, the_z );
            if(left_buttton_down)
            {
                world->offset_x += (last_mouse_x - the_x) / (10 * world->scale);
                world->offset_z += (last_mouse_z - the_z) / (10 * world->scale);

                world->tem_ofsetx = world->offset_x;
                world->tem_ofsetz = world->offset_z;

                last_mouse_x = the_x;
                last_mouse_z = the_z;
            }
            this->update();
        }

        if(event->type() == QEvent::Wheel)
        {
            int deltaw = static_cast<QWheelEvent *>(event)->delta();

            if(deltaw > 0){this->world->scale *= 1.1; if(this->world->scale > 40) this->world->scale = 40; }
            else {this->world->scale /= 1.1; if(this->world->scale < 0.001) this->world->scale = 0.001;}
            this->update();
        }
    }

    update();

    return QWidget::eventFilter(watched,event);
}

//主世界地图绘制
void MainWindow::widget1Paint(QPoint *mouse_pos  , bool measure , int solidx , int solidz)
{
    QPainter painter(ui->widget);



    painter.setFont(QFont("simhei",10,QFont::Bold , true));
    painter.setPen(Qt::black);

    world->generateGrid(&painter , mouse_pos,0);
    painter.setPen(QColor(50,150,50));
    painter.drawText(QPoint(550,30),"X:");
    QPointF mpos = Transformer::screenToWorld(world->offset_x , world->offset_z, coordinate_x , coordinate_z,world->scale);

    painter.drawText(QPoint(580,30),QString::number(int(mpos.rx()),10));
    painter.drawText(QPoint(550,50),"Z:");
    painter.drawText(QPoint(580,50),QString::number(int(mpos.ry()),10));

    //painter.drawText(QPoint(550,70),"Chunk:");
    painter.drawText(QPoint(510,70),QString("Chunk: %1 , %2").arg(
                         (mpos.rx() > 0)?(int(mpos.rx()) / 16):(int(mpos.rx()) / 16 - 1)
                         ).arg(
                         (mpos.ry() > 0)?(int(mpos.ry()) / 16):(int(mpos.ry()) / 16 - 1))
                     );

    if(measure)
    {
        painter.setPen(QPen(Qt::gray, 3, Qt::DashLine , Qt::RoundCap));
        double posx_solid_world = Transformer::screenToWorld(world->offset_x , world->offset_z , solidx , solidz , world->scale).rx();
        double posz_solid_world = Transformer::screenToWorld(world->offset_x , world->offset_z , solidx , solidz , world->scale).ry();
        int pox_solid = Transformer::worldToScreen(world->offset_x , world->offset_z , posx_solid_world , posz_solid_world , world->scale).rx();
        int poz_solid = Transformer::worldToScreen(world->offset_x , world->offset_z , posx_solid_world , posz_solid_world , world->scale).ry();

        painter.drawLine(QLineF(QPoint(pox_solid , poz_solid) , QPointF(mouse_pos->rx() , mouse_pos->ry()) ));
        double dis_screen = Transformer::distance(QPoint(solidx , solidz) , QPoint(mouse_pos->rx() , mouse_pos->ry()));
        double dis_world = dis_screen / (10*world->scale);
        QString dis = QString("%1 m").arg(dis_world);
        painter.setPen(Qt::black);
        painter.drawText(QPoint( 0.5 * (mouse_pos->rx() + solidx ) , 0.5 * (mouse_pos->ry() + solidz )) , dis);

    }

    delete mouse_pos;
}

//地狱地图绘制
void MainWindow::widget2Paint(QPoint *mouse_pos  , bool measure , int solidx , int solidz)
{
    QPainter painter(ui->widget_2);



    painter.setFont(QFont("simhei",10,QFont::Bold , true));
    painter.setPen(Qt::black);

    world->generateGrid(&painter , mouse_pos, 1);
    painter.setPen(QColor(0,0,0));
    painter.drawText(QPoint(550,30),"X:");
    QPointF mpos = Transformer::screenToWorld(world->offset_x , world->offset_z, coordinate_x , coordinate_z,world->scale);

    painter.drawText(QPoint(580,30),QString::number(int(mpos.rx()),10));
    painter.drawText(QPoint(550,50),"Z:");
    painter.drawText(QPoint(580,50),QString::number(int(mpos.ry()),10));

    //painter.drawText(QPoint(550,70),"Chunk:");
    painter.drawText(QPoint(510,70),QString("Chunk: %1 , %2").arg(
                         (mpos.rx() > 0)?(int(mpos.rx()) / 16):(int(mpos.rx()) / 16 - 1)
                         ).arg(
                         (mpos.ry() > 0)?(int(mpos.ry()) / 16):(int(mpos.ry()) / 16 - 1))
                     );

    if(measure)
    {
        painter.setPen(QPen(Qt::gray, 3, Qt::DashLine , Qt::RoundCap));
        double posx_solid_world = Transformer::screenToWorld(world->offset_x , world->offset_z , solidx , solidz , world->scale).rx();
        double posz_solid_world = Transformer::screenToWorld(world->offset_x , world->offset_z , solidx , solidz , world->scale).ry();
        int pox_solid = Transformer::worldToScreen(world->offset_x , world->offset_z , posx_solid_world , posz_solid_world , world->scale).rx();
        int poz_solid = Transformer::worldToScreen(world->offset_x , world->offset_z , posx_solid_world , posz_solid_world , world->scale).ry();

        painter.drawLine(QLineF(QPoint(pox_solid , poz_solid) , QPointF(mouse_pos->rx() , mouse_pos->ry()) ));
        double dis_screen = Transformer::distance(QPoint(solidx , solidz) , QPoint(mouse_pos->rx() , mouse_pos->ry()));
        double dis_world = dis_screen / (10*world->scale);
        QString dis = QString("%1 m").arg(dis_world);
        painter.setPen(Qt::black);
        painter.drawText(QPoint( 0.5 * (mouse_pos->rx() + solidx ) , 0.5 * (mouse_pos->ry() + solidz )) , dis);

    }

    delete mouse_pos;
}

//地狱地图绘制
void MainWindow::widget3Paint(QPoint *mouse_pos, bool measure  , int solidx , int solidz)
{
    QPainter painter(ui->widget_3);



    painter.setFont(QFont("simhei",10,QFont::Bold , true));
    painter.setPen(Qt::black);

    world->generateGrid(&painter , mouse_pos, 2);
    painter.setPen(QColor(250,0,250));
    painter.drawText(QPoint(550,30),"X:");
    QPointF mpos = Transformer::screenToWorld(world->offset_x , world->offset_z, coordinate_x , coordinate_z,world->scale);

    painter.drawText(QPoint(580,30),QString::number(int(mpos.rx()),10));
    painter.drawText(QPoint(550,50),"Z:");
    painter.drawText(QPoint(580,50),QString::number(int(mpos.ry()),10));

    //painter.drawText(QPoint(550,70),"Chunk:");
    painter.drawText(QPoint(510,70),QString("Chunk: %1 , %2").arg(
                         (mpos.rx() > 0)?(int(mpos.rx()) / 16):(int(mpos.rx()) / 16 - 1)
                         ).arg(
                         (mpos.ry() > 0)?(int(mpos.ry()) / 16):(int(mpos.ry()) / 16 - 1))
                     );

    if(measure)
    {
        painter.setPen(QPen(Qt::gray, 3, Qt::DashLine , Qt::RoundCap));
        double posx_solid_world = Transformer::screenToWorld(world->offset_x , world->offset_z , solidx , solidz , world->scale).rx();
        double posz_solid_world = Transformer::screenToWorld(world->offset_x , world->offset_z , solidx , solidz , world->scale).ry();
        int pox_solid = Transformer::worldToScreen(world->offset_x , world->offset_z , posx_solid_world , posz_solid_world , world->scale).rx();
        int poz_solid = Transformer::worldToScreen(world->offset_x , world->offset_z , posx_solid_world , posz_solid_world , world->scale).ry();

        painter.drawLine(QLineF(QPoint(pox_solid , poz_solid) , QPointF(mouse_pos->rx() , mouse_pos->ry()) ));
        double dis_screen = Transformer::distance(QPoint(solidx , solidz) , QPoint(mouse_pos->rx() , mouse_pos->ry()));
        double dis_world = dis_screen / (10*world->scale);
        QString dis = QString("%1 m").arg(dis_world);
        painter.setPen(Qt::black);
        painter.drawText(QPoint( 0.5 * (mouse_pos->rx() + solidx ) , 0.5 * (mouse_pos->ry() + solidz )) , dis);

    }

    delete mouse_pos;
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_7_clicked()  //jump键
{

    double x = ui->lineEdit->text().toDouble();
    double z = ui->lineEdit_2->text().toDouble();

    world->tem_ofsetx = world->offset_x;
    world->tem_ofsetz = world->offset_z;

    /*
    for(double t = 0 ; t < 81 ; t += 0.1)
    {
        tx = (x - old_x) * Transformer::sigmoid(t) + old_x;
        tz = (z - old_z) * Transformer::sigmoid(t) + old_z;
        world->offset_x = tx;
        world->offset_z = tz;
        update();
        Sleep(1);
    }
    */ //非线性动画添加失败

    world->offset_x = x;
    world->offset_z = z;
    update();
}

void MainWindow::on_pushButton_5_clicked()  //加载器的create键
{
    int x = ui->lineEdit_3->text().toInt();
    int z = ui->lineEdit_4->text().toInt();
    int wr = ui->lineEdit_5->text().toInt();
    int sr = ui->lineEdit_6->text().toInt();

    world->addLoadingMachine(LoadingMachine(x,z,wr,sr));
    updateListView(world);
    update();
}

void MainWindow::on_listView_clicked(const QModelIndex &index) //加载器的列表
{

    QString str = index.data().toString();
    selected_id = str.split('.')[0].toInt();
    qDebug()<<selected_id;

    str.split('.')[1];
    str = str.split('(')[1];
    str = str.split(')')[0];

    ui->lineEdit->setText(str.split(',')[0]);
    ui->lineEdit_2->setText(str.split(',')[1]);

    ui->lineEdit_3->setText(str.split(',')[0]);
    ui->lineEdit_4->setText(str.split(',')[1]);

    update();

}

void MainWindow::on_listView_2_clicked(const QModelIndex &index) //地标列表
{

    QString str = index.data().toString();
    selected_id = str.split('.')[0].toInt();
    qDebug()<<selected_id;

    str.split('.')[1];
    str = str.split('(')[1];
    str = str.split(')')[0];

    ui->lineEdit->setText(str.split(',')[0]);
    ui->lineEdit_2->setText(str.split(',')[1]);

    ui->lineEdit_10->setText(str.split(',')[0]);
    ui->lineEdit_9->setText(str.split(',')[1]);

    update();

}

void MainWindow::on_pushButton_clicked() //加载器的remove键
{
    world->deleteLoadingMachine(selected_id);
    updateListView(world);

    selected_id = -1;
    update();
}

void MainWindow::on_pushButton_6_clicked() //地标的create键
{
    int x = ui->lineEdit_10->text().toInt();
    int z = ui->lineEdit_9->text().toInt();
    QString name = ui->lineEdit_8->text();

    world->addLandMark(Landmark(x,z,name));
    updateListView(world);
    update();
}

void MainWindow::on_pushButton_2_clicked() //地标的remove键
{
    world->deleteLandMark(selected_id);
    updateListView(world);

    selected_id = -1;
    update();
}



void MainWindow::on_pushButton_3_clicked() //保存log文件
{
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy_MM_dd_hh_mm_ss");

    QString file_name = QString("saves/%1.waglplanner") .arg(current_date);
    QFile f(file_name);
    f.open(QIODevice::WriteOnly | QIODevice::Text );

    QTextStream t(&f);
    t.setCodec("UTF-8");

    t <<"WAGPLANNER\n";
    t <<"loadmachine\n";

    for(int i = 0 ; i < world_main->loadmachine_list.length() ; i++)
    {
        LoadingMachine m = world_main->loadmachine_list[i];
        t <<m.id<<"#"<<m.co_x<<"#"<<m.co_z<<"#"<<m.block_x<<"#"<<m.block_z<<"#"<<m.weak_load_range<<"#"<<m.strong_load_range<<"#\n";
    }

    t <<"landmark\n";
    for(int i = 0 ; i < world_main->landmark_list.length() ; i++)
    {
        Landmark l = world_main->landmark_list[i];
        t <<l.id<<"#"<<l.co_x<<"#"<<l.co_z<<"#";
        t<<l.name<<"#\n";
    }

    t <<"loadmachine\n";

    for(int i = 0 ; i < hell->loadmachine_list.length() ; i++)
    {
        LoadingMachine m = hell->loadmachine_list[i];
        t <<m.id<<"#"<<m.co_x<<"#"<<m.co_z<<"#"<<m.block_x<<"#"<<m.block_z<<"#"<<m.weak_load_range<<"#"<<m.strong_load_range<<"#\n";
    }

    t <<"landmark\n";
    for(int i = 0 ; i < hell->landmark_list.length() ; i++)
    {
        Landmark l = hell->landmark_list[i];
        t <<l.id<<"#"<<l.co_x<<"#"<<l.co_z<<"#";
        t<<l.name<<"#\n";
    }

    t <<"loadmachine\n";

    for(int i = 0 ; i < the_end->loadmachine_list.length() ; i++)
    {
        LoadingMachine m = the_end->loadmachine_list[i];
        t <<m.id<<"#"<<m.co_x<<"#"<<m.co_z<<"#"<<m.block_x<<"#"<<m.block_z<<"#"<<m.weak_load_range<<"#"<<m.strong_load_range<<"#\n";
    }

    t <<"landmark\n";
    for(int i = 0 ; i < the_end->landmark_list.length() ; i++)
    {
        Landmark l = the_end->landmark_list[i];
        t <<l.id<<"#"<<l.co_x<<"#"<<l.co_z<<"#";
        t<<l.name<<"#\n";
    }

    t <<"end";
    f.close();
    QMessageBox::information(NULL, "完成", QString("保存至%1").arg(file_name),
                             QMessageBox::Ok);


}


void MainWindow::on_pushButton_4_clicked() //读取log文件
{

    QString fileName = QFileDialog::getOpenFileName(this,
                                                        tr("打开存档！"),
                                                        "F:",
                                                        tr("waglplanner文件(*waglplanner);;"));
   readLog(fileName);
}

void MainWindow::readLog(QString filename)
{
    QFile f(filename);

    f.open(QIODevice::ReadOnly | QIODevice::Text);

    QString line = f.readLine();
    if( line.compare(QString("WAGPLANNER\n")) != 0)
    {
        QMessageBox::information(NULL, "错误", "读取存档失败！",QMessageBox::Ok);
    }
    else {

        line = f.readLine();
        world_main->clear();
        for(line = f.readLine(); line.compare(QString("landmark\n")) != 0 ; line = f.readLine())
        {
            int id = line.split('#')[0].toInt();
            int cx = line.split('#')[1].toInt();
            int cz = line.split('#')[2].toInt();
            int bx = line.split('#')[3].toInt();
            int bz = line.split('#')[4].toInt();
            int wr = line.split('#')[5].toInt();
            int sr = line.split('#')[6].toInt();
            world_main->addLoadingMachine(LoadingMachine(id,cx,cz,wr,sr));

        }

        for(line = f.readLine(); line.compare(QString("end")) != 0 && line.compare(QString("loadmachine\n")) != 0; line = f.readLine())
        {
            int id = line.split('#')[0].toInt();
            int cx = line.split('#')[1].toInt();
            int cz = line.split('#')[2].toInt();
            QString name = line.split('#')[3];

            world_main->addLandMark(Landmark(id,cx,cz,name));
        }

        hell->clear();
        for(line = f.readLine(); line.compare(QString("landmark\n")) != 0 ; line = f.readLine())
        {
            int id = line.split('#')[0].toInt();
            int cx = line.split('#')[1].toInt();
            int cz = line.split('#')[2].toInt();
            int bx = line.split('#')[3].toInt();
            int bz = line.split('#')[4].toInt();
            int wr = line.split('#')[5].toInt();
            int sr = line.split('#')[6].toInt();
            hell->addLoadingMachine(LoadingMachine(id,cx,cz,wr,sr));

        }

        for(line = f.readLine(); line.compare(QString("end")) != 0 && line.compare(QString("loadmachine\n")) != 0; line = f.readLine())
        {

            int id = line.split('#')[0].toInt();
            int cx = line.split('#')[1].toInt();
            int cz = line.split('#')[2].toInt();
            QString name = line.split('#')[3];
            hell->addLandMark(Landmark(id,cx,cz,name));
        }

        the_end->clear();
        for(line = f.readLine(); line.compare(QString("landmark\n")) != 0 ; line = f.readLine())
        {
            int id = line.split('#')[0].toInt();
            int cx = line.split('#')[1].toInt();
            int cz = line.split('#')[2].toInt();
            int bx = line.split('#')[3].toInt();
            int bz = line.split('#')[4].toInt();
            int wr = line.split('#')[5].toInt();
            int sr = line.split('#')[6].toInt();
            the_end->addLoadingMachine(LoadingMachine(id,cx,cz,wr,sr));

        }

        for(line = f.readLine(); line.compare(QString("end")) != 0 && line.compare(QString("loadmachine\n")) != 0; line = f.readLine())
        {

            int id = line.split('#')[0].toInt();
            int cx = line.split('#')[1].toInt();
            int cz = line.split('#')[2].toInt();
            QString name = line.split('#')[3];
            the_end->addLandMark(Landmark(id,cx,cz,name));
        }

        world = world_main;
        updateListView(world);
        update();

    }
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    selected_world = index + 1;
    switch(index)
    {
    case 0:{world = world_main;break;}
    case 1:{world = hell;break;}
    case 2:{world = the_end;break;}
    }
    updateListView(world);
    update();
}



void MainWindow::on_listView_2_doubleClicked(const QModelIndex &index) //地标列表双击
{
    QString str = index.data().toString();
    selected_id = str.split('.')[0].toInt();
    qDebug()<<selected_id;

    str.split('.')[1];
    str = str.split('(')[1];
    str = str.split(')')[0];

    ui->lineEdit->setText(str.split(',')[0]);
    ui->lineEdit_2->setText(str.split(',')[1]);

    double x = ui->lineEdit->text().toDouble();
    double z = ui->lineEdit_2->text().toDouble();

    world->offset_x = x;
    world->offset_z = z;
    world->scale = 0.7;
    update();

}

void MainWindow::searchLandMark(QString name)
{
    QStringList result;
    QModelIndex index;
    result = world->ldmkstr.filter(name);
    if(result.length() > 0){
        QString n = result[search_index % result.length()];
        int i = world->ldmkstr.indexOf(n);
        qDebug()<<i;
        index = ui->listView_2->model()->index(i,0);
        ui->listView_2->setCurrentIndex(index);
    }
}

void MainWindow::on_lineEdit_7_textChanged(const QString &arg1)
{
    qDebug()<<"搜索";
    search_index = 0;
    searchLandMark(arg1);
}

void MainWindow::on_pushButton_8_clicked()
{
    search_index++;
    searchLandMark(ui->lineEdit_7->text());
}

void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{
    QString str = index.data().toString();
    selected_id = str.split('.')[0].toInt();

    str.split('.')[1];
    str = str.split('(')[1];
    str = str.split(')')[0];

    ui->lineEdit->setText(str.split(',')[0]);
    ui->lineEdit_2->setText(str.split(',')[1]);

    double x = ui->lineEdit->text().toDouble();
    double z = ui->lineEdit_2->text().toDouble();

    world->offset_x = x;
    world->offset_z = z;
    world->scale = 0.7;
    update();
}
