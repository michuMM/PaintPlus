#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include<QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug()<<"Debug button";

    QSize screenSize = QSize(833, 552);

    image=QImage(screenSize,QImage::Format_RGB32);
    image.fill(Qt::white);
    drawing=false;
    brushColor=Qt::blue;
    brushsize=5;

    setMouseTracking(true);
    ui->centralwidget->setMouseTracking(true);

    setWindowTitle(tr("PaintPlus"));

    ui->statusBar->addPermanentWidget(ui->label);
    ui->label->setText("Współrzędne kursora: ");

    QWidget::setCursor(Qt::CrossCursor);

    ui->statusBar->setStyleSheet("background-color: rgb(240,240,240);");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        drawing=true;
        lastPoint=event->pos();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    ui->label->setText("wspolrzednne: " + QString::number(event->pos().x()) + ", " + QString::number(event->pos().y()) + ")");

    if((event->buttons()  & Qt::LeftButton) && drawing){
        QPainter painter(&image);
        painter.setPen(QPen(brushColor,brushsize,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
        painter.drawLine(lastPoint,event->pos());
        lastPoint=event->pos();
        this->update();
    }

}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawImage(this->rect(),image,image.rect());
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        drawing=false;
    }

}



