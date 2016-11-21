#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qdebug.h"
#include "qstring.h"
#include "qfiledialog.h"
#include "qpixmap.h"

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
//using namespace cv;
//Mat image;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->scrollArea->setWidget(ui->label);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    qDebug()<<"Open button clicked!"<<endl;
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open Image"),
                            ".",tr("Image Files (*.png *.jpg *.bmp)"));
    qDebug()<<"filenames:"<<fileName;
    if(fileName == NULL)
        return;
    //using opecv api
    //image = cv::imread(fileName.toAscii().data());
    image = cv::imread(fileName.toLatin1().constData());
    // cv::namedWindow(fileName.toAscii().data(),CV_WINDOW_AUTOSIZE));    //display use a new window
    cv::namedWindow(fileName.toLatin1().constData(),CV_WINDOW_AUTOSIZE);
    // cv::imshow((fileName.toAscii().data(), image);
    cv::imshow(fileName.toLatin1().constData(), image);

    //using Qt api
    //ui->graphicsView->show();
    QPixmap pixmap(fileName);
    ui->label->setPixmap(pixmap);
    //ui->label->resize(QSize(pixmap.width(),pixmap.height()));
    //ui->scrollArea->setWidget(ui->label);//scrollImag->setWidget(label);
    ui->label->show();
}
