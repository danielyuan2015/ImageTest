#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qdebug.h"
#include "qstring.h"
#include "qfiledialog.h"
#include "qpixmap.h"
#include "qtimer.h"
#include "convert.h"

//opencv headers
#include "opencv2/core/core_c.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/tracking.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/flann/flann.hpp"
#include "opencv2/calib3d/calib3d.hpp"
//#include "opencv2/objdetect/objdetect.hpp
//#include "opencv2/legacy/compat.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace ASM;
//using namespace cv;
//Mat image;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //using this 100ms timer to update slider value
    timer = new QTimer();

    ui->scrollArea->setWidget(ui->label);
    ui->horizontalSlider->setRange(0,255);
    ui->horizontalSlider_2->setRange(0,255);
    ui->horizontalSlider_3->setRange(0,255);

    timer->setInterval(200);
    connect(timer, SIGNAL(timeout()), this, SLOT(update_slider_value()));
    timer->start();
}

MainWindow::~MainWindow()
{
    timer->stop();
    delete timer;
    delete ui;
}

void MainWindow::update_slider_value()
{
    hVal = ui->horizontalSlider->value();
    sVal = ui->horizontalSlider_2->value();
    vVal = ui->horizontalSlider_3->value();
    ui->label_hVal->setNum(hVal);
    ui->label_sVal->setNum(sVal);
    ui->label_vVal->setNum(vVal);
   // qDebug()<<"hVal:"<<hVal<<endl;
}

void MainWindow::on_pushButton_clicked()
{
    qDebug()<<"Open button clicked!"<<endl;
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open Image"),
                            ".",tr("Image Files (*.png *.jpg *.bmp)"));
    qDebug()<<"filenames:"<<fileName<<endl;
    if(fileName == NULL)
        return;
#if 0
    //using opecv api
    //image = cv::imread(fileName.toAscii().data());
    image = cv::imread(fileName.toLatin1().constData());
    // cv::namedWindow(fileName.toAscii().data(),CV_WINDOW_AUTOSIZE));    //display use a new window
    cv::namedWindow(fileName.toLatin1().constData(),CV_WINDOW_AUTOSIZE);
    // cv::imshow((fileName.toAscii().data(), image);
    cv::imshow(fileName.toLatin1().constData(), image);

    //testing convert
    //QPixmap pixmap = cvMatToQPixmap(image);
    //ui->label->setPixmap(pixmap);
    //ui->label->show();
#endif

#if 1
    //using Qt api
    //ui->graphicsView->show();
    QPixmap pixmap(fileName);
    ui->label->setPixmap(pixmap);
    //ui->label->resize(QSize(pixmap.width(),pixmap.height()));
    //ui->scrollArea->setWidget(ui->label);//scrollImag->setWidget(label);
    ui->label->show();

    //testing convert
   image = QPixmapToCvMat(pixmap,true);
   cv::namedWindow(fileName.toLatin1().constData(),CV_WINDOW_AUTOSIZE);
   cv::imshow(fileName.toLatin1().constData(), image);
#endif
}
