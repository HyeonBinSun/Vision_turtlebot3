/**
 * @file /include/listener/main_window.hpp
 *
 * @brief Qt based gui for %(package)s.
 *
 * @date November 2010
 **/
#ifndef listener_MAIN_WINDOW_H
#define listener_MAIN_WINDOW_H

/*****************************************************************************
** Includes
*****************************************************************************/

#include <QMainWindow>
#include "ui_main_window.h"
#include "qnode.hpp"
#include "udp.h"
#include <QPixmap>
#include <QSlider>
#include <vector>
#include <QLabel>

/*****************************************************************************
** Namespace
*****************************************************************************/

namespace listener
{
  /*****************************************************************************
  ** Interface [MainWindow]
  *****************************************************************************/
  /**
   * @brief Qt central, all operations relating to the view part here.
   */
  class MainWindow : public QMainWindow
  {
      Q_OBJECT

  public:
    MainWindow(int argc, char** argv, QWidget* parent = 0);
    ~MainWindow();

   // bool IsRecieved = false;

    QHostAddress myIP;
    QHostAddress camIP;
    uint16_t PORT;
    QPixmap pixmap;
    \
    cv::Mat udp_img;    // 원본 이미지를 처음 담는 변수
    cv::Mat src_img;    // udp_img를 그대로 담는 변수
    cv::Mat dst_img;    // 원본의 resize, gbr 사이 과정을 처리하는 변수

    cv::Mat edge_img;   // Canny로 엣지 검출을 한 이미지를 담는 변수
    cv::Mat HSV_img;        // HSV 처리한 이미지
    cv::Mat Mask_img;       // 이진화된 이미지

    cv::Mat Roi_img;
    cv::Mat dst_Roi_img;

    cv::Mat stats, centroids, labels;

    cv::Mat dst_lime_img;
    cv::Mat dst_orange_img;
    cv::Mat dst_find_img;


    int Width, height;
    int low_H[4] = {0,}, low_S[4] = {0,}, low_V[4] = {0,};
    int high_H[4] = {0,}, high_S[4] = {0,}, high_V[4] = {0,};

    Point points[4];
    int save_points[4];
    int lime_rect[4];
    int orange_rect[4];

    int slider_flag = 0;

    QUdpSocket* receive_Socket;
    cv::Mat Change2Binary(cv::Mat conc, int Svalue);
    cv::Mat RegionOfInterest(cv::Mat img, Point *points);
    void CannyNHough();
    void BlopBounding(cv::Mat bod_img, int B, int G, int R, QLabel* label, int *save_rect);
    void OutputUI(const cv::Mat& img, QImage::Format Type);

  public Q_SLOTS:
    void UDP_Callback();
    void slider(int Svalue);
    void on_pushButton_clicked();

  private:
    Ui::MainWindowDesign ui;
    QNode qnode;
  };

}  // namespace s

#endif  // listener_MAIN_WINDOW_H
