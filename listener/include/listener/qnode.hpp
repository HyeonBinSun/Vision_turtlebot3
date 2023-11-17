/**
 * @file /include/listener/qnode.hpp
 *
 * @brief Communications central!
 *
 * @date February 2011
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef listener_QNODE_HPP_
#define listener_QNODE_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

// To workaround boost/qt4 problems that won't be bugfixed. Refer to
//    https://bugreports.qt.io/browse/QTBUG-22829
#ifndef Q_MOC_RUN
#include <ros/ros.h>
#endif
#include <string>
#include <QThread>
#include <QStringListModel>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Image.h>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <opencv2/opencv.hpp>
/*****************************************************************************
** Namespaces
*****************************************************************************/
using namespace cv;

namespace listener
{
  /*****************************************************************************
  ** Class
  *****************************************************************************/

  class QNode : public QThread
  {
    Q_OBJECT
  public:
    QNode(int argc, char** argv);
    virtual ~QNode();
    bool init();
    void run();
    bool Isrecieved = false;
    bool CheckS = true;
    cv::Mat *Cam_img;
    cv::Mat resize_img;

    geometry_msgs::Twist turtledevel;
 void SubCallback(const sensor_msgs::ImageConstPtr &img);
 void pubcmdvel();


  Q_SIGNALS:
    void rosShutdown();
    void SubSignalsupport();


  private:
    int init_argc;
    char** init_argv;
    ros::Subscriber Cam_sub;
    ros::Publisher gazebo_pub;

  };

}  // namespace s

#endif /* listener_QNODE_HPP_ */
