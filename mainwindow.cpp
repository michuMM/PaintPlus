#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include<QDebug>
#include "colors.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug()<<"Debug button";
    QSize screenSize = QSize(size().rwidth(), size().rheight());

    image=QImage(screenSize,QImage::Format_RGB32);
    image.fill(Qt::white);
    drawing=false;
    brushColor=Qt::black;
    brushsize=5;

    setMouseTracking(true);
    ui->centralwidget->setMouseTracking(true);

    setWindowTitle(tr("PaintPlus"));

    ui->statusBar->addPermanentWidget(ui->label);
    ui->label->setText("Współrzędne kursora: ");

    ui->statusBar->setStyleSheet("background-color: rgb(240,240,240);");

    QList<QPushButton*> buttons = MainWindow::findChildren<QPushButton *>();
    for (int i = 0; i < 30; ++i) {
        buttons.at(i)->setStyleSheet(QString("QPushButton { background-color: %1; }").arg(mainColors.at(i)));
        buttons.at(i)->setObjectName(mainColors.at(i));
        connect(buttons.at(i),SIGNAL(clicked()),this,SLOT(pushButtonClicked()));
    }
    buttons.at(30)->setStyleSheet(QString("QPushButton { background-color: black; }"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::pushButtonClicked() {

    QPushButton* buttonSender = qobject_cast<QPushButton*>(sender());
    ui->currentColorButton->setStyleSheet(QString("QPushButton { background-color: %1; }").arg(buttonSender->objectName()));
    QColor col = QColor(buttonSender->objectName());
    brushColor = col;

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
    ui->label->setText("wspolrzedne: " + QString::number(event->pos().x()) + ", " + QString::number(event->pos().y()) + ")");
    if((event->buttons()  & Qt::LeftButton) && drawing && event->pos().x()>200){
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

void MainWindow::resizeEvent(QResizeEvent* event) {
    int width = event->size().width();
    int height = event->size().height();
    QSize newSize(width, height);

    resizeImage(&image, newSize);

    QMainWindow::resizeEvent(event);
}

void MainWindow::resizeImage(QImage *image, const QSize &newSize)
{
    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(Qt::white);

    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}



