/**
 * @file /src/main_window.cpp
 *
 * @brief Implementation for the qt gui.
 *
 * @date February 2011
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <QtGui>
#include <QMessageBox>
#include <iostream>
#include "../include/listener/main_window.hpp"
#include <sensor_msgs/Image.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <QPixmap>
#include <QLabel>
#include <QImage>
#include <QByteArray>
#include <string>
#include <QSlider>
#include <vector>
#include <opencv2/opencv.hpp>
#include <geometry_msgs/Twist.h>

/*****************************************************************************
** Namespaces
*****************************************************************************/


namespace listener
{
  using namespace Qt;

  /*****************************************************************************
  ** Implementation [MainWindow]
  *****************************************************************************/

  MainWindow::MainWindow(int argc, char** argv, QWidget* parent) : QMainWindow(parent), qnode(argc, argv)
  {
    ui.setupUi(this);  // Calling this incidentally connects all ui's triggers to on_...() callbacks in this class.

    setWindowIcon(QIcon(":/images/icon.png"));

    myIP = QHostAddress("192.168.0.63");
    camIP = QHostAddress("192.168.0.51");

    PORT =  8888;

    receive_Socket = new QUdpSocket(this);

    qnode.init();

    QObject::connect(&qnode, SIGNAL(rosShutdown()), this, SLOT(close()));
    QObject::connect(&qnode, SIGNAL(SubSignalsupport()), this, SLOT(UDP_Callback()));

  }

  MainWindow::~MainWindow()
  {
  }
  void MainWindow::on_pushButton_clicked()
  {
      qnode.turtledevel.angular.z = 0;
      qnode.turtledevel.linear.x = 0.1;
      //qnode.CheckS = true;
      qnode.pubcmdvel();

  }
  void MainWindow::slider(int Svalue){

      if(Svalue == 0)
      {
      low_H[Svalue] = ui.low_Hue->value();
      ui.HueL->setText(QString::number(low_H[Svalue]));

      low_S[Svalue] = ui.low_Saturation->value();
      ui.SatL->setText(QString::number(low_S[Svalue]));

      low_V[Svalue] = ui.low_Value->value();
      ui.ValL->setText(QString::number(low_V[Svalue]));

      high_H[Svalue] = ui.high_Hue->value();
      ui.HueH->setText(QString::number(high_H[Svalue]));

      high_S[Svalue] = ui.high_Saturation->value();
      ui.SatH->setText(QString::number(high_S[Svalue]));

      high_V[Svalue] = ui.high_Value->value();
      ui.ValH->setText(QString::number(high_V[Svalue]));
      }
      if(Svalue == 1)
      {
          low_H[Svalue] = ui.low_Hue_3->value();
          ui.HueL_3->setText(QString::number(low_H[Svalue]));

          low_S[Svalue] = ui.low_Saturation_3->value();
          ui.SatL_3->setText(QString::number(low_S[Svalue]));

          low_V[Svalue] = ui.low_Value_3->value();
          ui.ValL_3->setText(QString::number(low_V[Svalue]));

          high_H[Svalue] = ui.high_Hue_3->value();
          ui.HueH_3->setText(QString::number(high_H[Svalue]));

          high_S[Svalue] = ui.high_Saturation_3->value();
          ui.SatH_3->setText(QString::number(high_S[Svalue]));

          high_V[Svalue] = ui.high_Value_3->value();
         ui.ValH_3->setText(QString::number(high_V[Svalue]));
      }
      if(Svalue == 2)
      {
          low_H[Svalue] = ui.low_Hue_4->value();
          ui.HueL_4->setText(QString::number(low_H[Svalue]));

          low_S[Svalue] = ui.low_Saturation_4->value();
          ui.SatL_4->setText(QString::number(low_S[Svalue]));

          low_V[Svalue] = ui.low_Value_4->value();
          ui.ValL_4->setText(QString::number(low_V[Svalue]));

          high_H[Svalue] = ui.high_Hue_4->value();
          ui.HueH_4->setText(QString::number(high_H[Svalue]));

          high_S[Svalue] = ui.high_Saturation_4->value();
          ui.SatH_4->setText(QString::number(high_S[Svalue]));

          high_V[Svalue] = ui.high_Value_4->value();
         ui.ValH_4->setText(QString::number(high_V[Svalue]));
      }
      if(Svalue == 3)
      {
          low_H[Svalue] = ui.low_Hue_5->value();
          ui.HueL_5->setText(QString::number(low_H[Svalue]));

          low_S[Svalue] = ui.low_Saturation_5->value();
          ui.SatL_5->setText(QString::number(low_S[Svalue]));

          low_V[Svalue] = ui.low_Value_5->value();
          ui.ValL_5->setText(QString::number(low_V[Svalue]));

          high_H[Svalue] = ui.high_Hue_5->value();
          ui.HueH_5->setText(QString::number(high_H[Svalue]));

          high_S[Svalue] = ui.high_Saturation_5->value();
          ui.SatH_5->setText(QString::number(high_S[Svalue]));

          high_V[Svalue] = ui.high_Value_5->value();
         ui.ValH_5->setText(QString::number(high_V[Svalue]));
      }
  }
  void MainWindow::OutputUI(const cv::Mat& img, QImage::Format Type){
      QImage qtImg(img.data, img.cols, img.rows, img.step,Type);      // img를 띄우기위해 qtImg형태로, 회색조로 변경.
      qtImg = qtImg.rgbSwapped();                                                           // rgb Swapped();
      pixmap = QPixmap::fromImage(qtImg);                                                   // pixmap으로 qtimg를 받는다.

  }
  cv::Mat MainWindow::Change2Binary(cv::Mat conc, int Svalue){
     slider(Svalue);
     cv::inRange(dst_img, Scalar(low_H[Svalue], low_S[Svalue], low_V[Svalue]),
                 Scalar(high_H[Svalue], high_S[Svalue], high_V[Svalue]), conc);    // HSV로 변환한 이미지를 inRange
     cv::erode(conc, conc, Mat::ones(Size(1,1),CV_8UC1), Point(-1,-1),3);                     // erode
     cv::dilate(conc, conc, Mat::ones(Size(1,1),CV_8UC1), Point(-1,-1),3);                    // dilate
  return conc;
  }
  cv::Mat MainWindow::RegionOfInterest(cv::Mat img, Point *points){                         // 관심 영역 설정

      dst_Roi_img = cv::Mat::zeros(height, Width, CV_8UC1);
      const Point* PosofPoints[1] = {points};
      int NumofPoints[] = {4};
      cv::fillPoly(dst_Roi_img, PosofPoints, NumofPoints, 1, Scalar(255, 255, 255), LINE_8);

      cv::Mat bitand_img;
      bitwise_and(img, dst_Roi_img, bitand_img);

      return bitand_img;
  }
  void MainWindow::BlopBounding(cv::Mat bod_img, int B, int G, int R, QLabel *label, int *save_rect){
      int Boundingelements = cv::connectedComponentsWithStats(bod_img, labels, stats, centroids, 8);

      if(Boundingelements > 0)
      {
          for(int j = 1; j < Boundingelements; j++){
              int *p = stats.ptr<int>(j);
              rectangle(dst_find_img, Rect(p[0], p[1], p[2], p[3]), Scalar(B, G, R), 2, 16);

              for(int i = 0; i < 4; i++){
               save_rect[i] = p[i];
            }
          }
  }
  }
  void MainWindow::CannyNHough(){
      cv::Canny(Roi_img, edge_img, 50, 200, 3);

       Width = edge_img.cols;
       height = edge_img.rows;

       points[0] = Point((Width-220)/2, height-60);
       points[1] = Point((Width-220)/2, height-85);
       points[2] = Point((Width+110)/2, height-85);
       points[3] = Point((Width+110)/2, height-60);

       dst_Roi_img = RegionOfInterest(Roi_img, points);

  }

  void MainWindow::UDP_Callback()
  {
          dst_img = *(qnode.Cam_img);
         cv::resize(dst_img, dst_img, Size(240,160), 0 , 0, CV_INTER_LINEAR);
          cv::GaussianBlur(dst_img, dst_img, Size(3,3), 0);

            Roi_img = dst_img.clone();
           dst_find_img = dst_img.clone();
            dst_lime_img = dst_img.clone();
           dst_orange_img = dst_img.clone();

           Mask_img = Change2Binary(Mask_img, 0);
            Roi_img = Change2Binary(Roi_img, 1);
           dst_orange_img = Change2Binary(dst_orange_img, 2);
          dst_lime_img = Change2Binary(dst_lime_img, 3);

           CannyNHough();

             std::vector<Vec4i> white_line;    // 직선 속성 변수로 선의 양극단 좌표를 배열처럼 반환

             cv::HoughLinesP(edge_img, white_line, 1, CV_PI/180, 50, 45, 30);                       // 허프 라인 따고, 선 좌표 저장
            for(const Vec4i& line : white_line){
                 int Xa, Xb, Ya, Yb;
                 std::tie(Xa, Ya, Xb, Yb) = std::make_tuple(line[0], line[1], line[2], line[3]);
            cv::line(dst_find_img, Point(Xa,Ya), Point(Xb,Yb), cv::Scalar(153,51,0), 2, LINE_AA);
               for(int i = 0; i < 4; i++){
                save_points[i] = line[i];
           }
             }

            OutputUI(dst_img, QImage::Format_RGB888);           // 원본 이미지 출력
            ui.Cv_cam->setPixmap(pixmap);

            OutputUI(Mask_img, QImage::Format_Grayscale8);      // 선 이미지 출력
            ui.Binary_White->setPixmap(pixmap);

            OutputUI(dst_Roi_img, QImage::Format_Grayscale8);          // Roi 출력
            ui.RoI->setPixmap(pixmap);

            OutputUI(dst_orange_img, QImage::Format_Grayscale8);    // 오렌지 콘 출력
            ui.Binary_Orange->setPixmap(pixmap);

            OutputUI(dst_lime_img, QImage::Format_Grayscale8);      // 라임 콘 출력
            ui.Binary_Lime->setPixmap(pixmap);

            BlopBounding(dst_orange_img, 0, 0, 255, ui.Orangestat, orange_rect);
            BlopBounding(dst_lime_img, 51, 255, 51, ui.Limestat, lime_rect);

            OutputUI(dst_find_img, QImage::Format_RGB888);      // 라임 콘 출력
            ui.Find_obj->setPixmap(pixmap);

            qnode.Cam_img = NULL;
           qnode.Isrecieved = false;
          delete qnode.Cam_img;


  }


  /*****************************************************************************
  ** Functions
  *****************************************************************************/

}  // namespace s
