#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPainter>
#include <QFileDialog>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void paintEvent(QPaintEvent *event);
    virtual void resizeEvent(QResizeEvent* event);
    virtual void resizeImage(QImage *image, const QSize &newSize);
    virtual void activateCurrentTool();
    virtual void closeEvent(QCloseEvent *event) override;
    virtual void drawGrid();

private slots:
    void pushButtonClicked();
    void on_resetButton_clicked();
    void on_brushWidthSlider_valueChanged(int value);
    void on_pencilButton_clicked();
    void on_rubberButton_clicked();
    void on_actionGridLines_changed();

private:
    Ui::MainWindow *ui;
    QImage image;
    QPoint lastPoint;
    QColor brushColor;
    QString selectedTool;
    double gridSize;
    int brushsize;
    bool drawing;
    bool modified;
    bool showGrid;

};

#endif // MAINWINDOW_H
