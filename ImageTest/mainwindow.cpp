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

void ColorIdentify(cv::Mat &image, cv::Mat &result);

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
#if 1
    //using opecv api
    image = cv::imread(fileName.toLatin1().constData());  
    cv::namedWindow(fileName.toLatin1().constData(),CV_WINDOW_AUTOSIZE);//display use a new window
    cv::imshow(fileName.toLatin1().constData(), image);

    //Testing convert from Mat to QPixmap
    //QPixmap pixmap = cvMatToQPixmap(image);
    //ui->label->setPixmap(pixmap);
    //ui->label->show();

    //vector<cv::Mat> huv;
    //cv::split(image,huv);
    //cv::Mat imageBlueChannel = huv.at(1);
    //cv::namedWindow("test",CV_WINDOW_AUTOSIZE);
    //cv::imshow("test", imageBlueChannel);

    cv::Mat result;
    ColorIdentify(image,result);
    QPixmap pixmap = cvMatToQPixmap(result);
    ui->label->setPixmap(pixmap);
    ui->label->show();
    cv::namedWindow("result",CV_WINDOW_AUTOSIZE);//display use a new window
    cv::imshow("result", result);


#endif

#if 0
    //using Qt api
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

//identify Pink color
void ColorIdentify(cv::Mat &image, cv::Mat &result)  //产出是一个mask
{
    const uchar* curr_r_hue;
    const uchar* curr_r_satur;
    const uchar* curr_r_value;
    uchar* curr_r_result;
    uchar* tp;

    cv::Mat hsv_image;        //转HSV
    hsv_image.create(image.size(), image.type());
    //result = image.clone();
    result.create(image.size(), image.type());

    cv::cvtColor(image, hsv_image, CV_BGR2HSV);

    vector<cv::Mat> channels;
    cv::split(hsv_image, channels);

    cv::namedWindow("channels0",CV_WINDOW_AUTOSIZE);//display h channels
    cv::imshow("channels0", channels[0]);
    cv::namedWindow("channels1",CV_WINDOW_AUTOSIZE);//display s channels
    cv::imshow("channels1", channels[1]);
    cv::namedWindow("channels2",CV_WINDOW_AUTOSIZE);//display v channels
    cv::imshow("channels2", channels[2]);

    int num_row = image.rows;
    int num_col = image.cols;
    qDebug()<<"num_row"<<num_row<<endl;
    qDebug()<<"num_col"<<num_col<<endl;
    int count = 0;
    for (int r = 0; r < num_row; r++)
    {
        //const cv::Vec3b* curr_r_image = image.ptr<const cv::Vec3b>(r);
        tp = image.ptr<uchar>(r);
        curr_r_hue = channels[0].ptr<const uchar>(r);
        curr_r_satur = channels[1].ptr<const uchar>(r);
        curr_r_value = channels[2].ptr<const uchar>(r);
        curr_r_result = result.ptr<uchar>(r);

        for (int c = 0; c < num_col; c++)
        {
            //curr_r_result[c] = tp[c];
#if 1
            if ( ((curr_r_hue[c] <= 2 && curr_r_hue[c] >= 0)||(curr_r_hue[c] <= 180 && curr_r_hue[c] >= 150)) \
                    && (curr_r_value[c]>130) && (curr_r_satur[c]>35 && curr_r_satur[c]<150)) //找颜色
            {
                curr_r_result[3*c] = 255;//tp[c];//255;
                 curr_r_result[3*c+1] = 255;
                  curr_r_result[3*c+2] = 255;
                count++;
            }
            else
            {
                curr_r_result[3*c] = 0;//tp[c];//0;
                curr_r_result[3*c+1] = 0;
                 curr_r_result[3*c+2] = 0;
                count++;
            }
#endif

        }
    }
    //cv::cvtColor(result, result, CV_HSV2BGR);
    qDebug()<<"count"<<count<<endl;
}
