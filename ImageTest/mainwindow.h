#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "opencv2/imgproc/imgproc.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void update_slider_value();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    cv::Mat image;
    int hVal;
    int sVal;
    int vVal;
};

#endif // MAINWINDOW_H
