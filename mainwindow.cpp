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

    world_main      = new Map();
    hell            = new Map();
    the_end         = new Map();

    world = world_main;

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

    static bool mid_buttton_down = 0; //中键按下
    static int last_mouse_x = 0; //上一次鼠标位置
    static int last_mouse_z = 0;
    if(
            (watched == ui->widget    ||
             watched == ui->widget_2  ||
             watched == ui->widget_3) &&
            event->type() == QEvent::Paint  ){

        widget1Paint(this->getMouseCoordinate()); //响应函数
    }

    if(watched == ui->widget)   //主世界地图面板
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            if(static_cast<QMouseEvent *>(event)->button() == Qt::MidButton) //中键按下
            {
                mid_buttton_down = 1;
                last_mouse_x = static_cast<QMouseEvent *>(event)->x();
                last_mouse_z = static_cast<QMouseEvent *>(event)->y();
            }
        }
        if(event->type() == QEvent::MouseButtonRelease)
        {
            if(static_cast<QMouseEvent *>(event)->button() == Qt::MidButton) //中键释放
            {
                mid_buttton_down = 0;
            }
        }

        if(event->type() == QEvent::MouseMove)
        {
            int the_x = static_cast<QMouseEvent *>(event)->x();
            int the_z = static_cast<QMouseEvent *>(event)->y();
            setMouseCoordinate( the_x, the_z );
            if(mid_buttton_down)
            {
                world->offset_x += (last_mouse_x - the_x) / (10 * world->scale);
                world->offset_z += (last_mouse_z - the_z) / (10 * world->scale);

                qDebug()<<world->offset_x<<"\t"<<world->offset_z<<"\n";

                last_mouse_x = the_x;
                last_mouse_z = the_z;
            }
            this->update();
        }

        if(event->type() == QEvent::Wheel)
        {
            int deltaw = static_cast<QWheelEvent *>(event)->delta();

            if(deltaw > 0){this->world->scale *= 1.1;}
            else {this->world->scale /= 1.1; if(this->world->scale < 0.01) this->world->scale = 0.01;}
            this->update();
        }
    }


    return QWidget::eventFilter(watched,event);
}

//主世界地图绘制
void MainWindow::widget1Paint(QPoint* mouse_pos)
{
    QPainter painter(ui->widget);
    QPoint* pos = getMouseCoordinate();



    painter.setFont(QFont("simhei",10,QFont::Bold , true));
    painter.setPen(Qt::black);

    world->generateGrid(&painter , mouse_pos);
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
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_7_clicked()  //jump键
{
    double x = ui->lineEdit->text().toDouble();
    double z = ui->lineEdit_2->text().toDouble();

    world->offset_x = x;
    world->offset_z = z;
    ui->widget->update();
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

    qDebug()<<"有一个项目被选中";
    QString str = index.data().toString();
    selected_id = str.split('.')[0].toInt();
    qDebug()<<selected_id;

    str.split('.')[1];
    str = str.split('(')[1];
    str = str.split(')')[0];

    qDebug()<<str.split(',')[0]<<str.split(',')[1];
    ui->lineEdit->setText(str.split(',')[0]);
    ui->lineEdit_2->setText(str.split(',')[1]);
    update();

}

void MainWindow::on_listView_2_clicked(const QModelIndex &index) //地标列表
{
    qDebug()<<"有一个项目被选中";
    QString str = index.data().toString();
    selected_id = str.split('.')[0].toInt();
    qDebug()<<selected_id;

    str.split('.')[1];
    str = str.split('(')[1];
    str = str.split(')')[0];

    qDebug()<<str.split(',')[0]<<str.split(',')[1];
    ui->lineEdit->setText(str.split(',')[0]);
    ui->lineEdit_2->setText(str.split(',')[1]);
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
    t <<"WAGPLANNER\n";
    t <<"loadmachine\n";

    for(int i = 0 ; i < world->loadmachine_list.length() ; i++)
    {
        LoadingMachine m = world->loadmachine_list[i];
        t <<m.id<<"#"<<m.co_x<<"#"<<m.co_z<<"#"<<m.block_x<<"#"<<m.block_z<<"#"<<m.weak_load_range<<"#"<<m.strong_load_range<<"\n";
    }

    t <<"landmark\n";
    for(int i = 0 ; i < world->landmark_list.length() ; i++)
    {
        Landmark l = world->landmark_list[i];
        t <<l.id<<"#"<<l.co_x<<"#"<<l.co_z<<"#";
        t<<l.name<<"\n";
    }

    t <<"end";
    f.close();
    QMessageBox::information(NULL, "完成", QString("保存至%1").arg(file_name),
                             QMessageBox::Ok);


}


void MainWindow::on_pushButton_4_clicked() //读取log文件
{
    QTextCodec* code = QTextCodec::codecForName("UTF-8");
    QString fileName = QFileDialog::getOpenFileName(this,
                                                        tr("打开存档！"),
                                                        "F:",
                                                        tr("waglplanner文件(*waglplanner);;"));
    QFile f(fileName);
    f.open(QIODevice::ReadOnly | QIODevice::Text);

    QString line = f.readLine();
    if( line.compare(QString("WAGPLANNER\n")) != 0)
    {
        QMessageBox::information(NULL, "错误", "读取存档失败！",QMessageBox::Ok);
    }
    else {

        line = f.readLine();
        world->clear();
        for(line = f.readLine(); line.compare(QString("landmark\n")) != 0 ; line = f.readLine())
        {
            qDebug()<<line;
            int id = line.split('#')[0].toInt();
            int cx = line.split('#')[1].toInt();
            int cz = line.split('#')[2].toInt();
            int bx = line.split('#')[3].toInt();
            int bz = line.split('#')[4].toInt();
            int wr = line.split('#')[5].toInt();
            int sr = line.split('#')[6].toInt();
            world->addLoadingMachine(LoadingMachine(id,cx,cz,wr,sr));

        }

        for(line = f.readLine(); line.compare(QString("end")) != 0 ; line = f.readLine())
        {

            int id = line.split('#')[0].toInt();
            int cx = line.split('#')[1].toInt();
            int cz = line.split('#')[2].toInt();
            QString name = code->toUnicode(line.split('#')[3].toUtf8());
            qDebug()<<name;
            world->addLandMark(Landmark(id,cx,cz,name));
        }
        updateListView(world);
        update();

    }
}
