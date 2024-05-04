#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include<QDebug>
#include "colors.h"
#include "icons.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    selectedTool = "pencilButton";
    activateCurrentTool();

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
    ui->graphicsView->setCursor(Qt::ArrowCursor);

    QList<QPushButton*> buttons = MainWindow::findChildren<QPushButton *>();
    for (int i = 0; i < 30; ++i) {
        buttons.at(i)->setStyleSheet(QString("QPushButton { background-color: %1; }").arg(mainColors.at(i)));
        buttons.at(i)->setObjectName(mainColors.at(i));
        buttons.at(i)->setCursor(Qt::ArrowCursor);
        connect(buttons.at(i),SIGNAL(clicked()),this,SLOT(pushButtonClicked()));
    }
    buttons.at(30)->setStyleSheet(QString("QPushButton { background-color: black; }"));

    QPixmap clearIcon(Icons::clearIcon96);
    QPixmap pencilIcon(Icons::pencilIcon);
    QPixmap rubberIcon(Icons::rubberIcon);

    ui->resetButton->setIcon(clearIcon);

    ui->brushWidthSlider->setValue(50);

    ui->lineWidth->setFrameShape(QFrame::HLine);
    ui->lineWidth->setFrameShadow(QFrame::Plain);
    ui->pencilButton->setIcon(pencilIcon);
    ui->rubberButton->setIcon(rubberIcon);

}

MainWindow::~MainWindow()
{
    delete ui;
}


//colors buttons
void MainWindow::pushButtonClicked() {

    QPushButton* buttonSender = qobject_cast<QPushButton*>(sender());
    ui->currentColorButton->setStyleSheet(QString("QPushButton { background-color: %1; }").arg(buttonSender->objectName()));
    ui->lineWidth->setStyleSheet(QString("QFrame { color: %1 } ").arg(buttonSender->objectName()));
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
        if(selectedTool == "rubberButton") {
            painter.setPen(QPen(Qt::white,brushsize,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
        }
        else {
            painter.setPen(QPen(brushColor,brushsize,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
        }
        painter.drawLine(lastPoint, event->pos());
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

// tools defined below

void MainWindow::on_resetButton_clicked()
{
    image.fill(Qt::white);
    update();

}

void MainWindow::on_brushWidthSlider_valueChanged(int value)
{
    ui->lineWidth->setLineWidth(value/7.5);
    qDebug() << value;
    brushsize = value/8;
}

void MainWindow::activateCurrentTool() {
    QList<QPushButton*> foundButtons = ui->frame_4->findChildren<QPushButton*>();
    for (QPushButton* button : foundButtons) {
        button->setStyleSheet("");
    }
    QPushButton *foundButton = QWidget::findChild<QPushButton*>(selectedTool);
    foundButton->setStyleSheet("QPushButton { background-color: #2ecc71; border: 2px solid #27ae60; }"
                               "QPushButton:hover { background-color: #27ae60; }"
                               "QPushButton:pressed { background-color: #1f8b4c; }");
}

void MainWindow::on_pencilButton_clicked()
{
    selectedTool = "pencilButton";
    activateCurrentTool();

}


void MainWindow::on_rubberButton_clicked()
{
    selectedTool = "rubberButton";
    activateCurrentTool();
}

