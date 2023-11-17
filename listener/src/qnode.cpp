/**
 * @file /src/qnode.cpp
 *
 * @brief Ros communication central!
 *
 * @date February 2011
 **/

/*****************************************************************************
** Includes
*****************************************************************************/

#include <ros/ros.h>
#include <ros/network.h>
#include <string>
#include <std_msgs/String.h>
#include <sstream>
#include "../include/listener/qnode.hpp"
#include <geometry_msgs/Twist.h>
#include <opencv2/opencv.hpp>

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace listener
{
  /*****************************************************************************
  ** Implementation
  *****************************************************************************/

  QNode::QNode(int argc, char** argv) : init_argc(argc), init_argv(argv)
  {
  }

  QNode::~QNode()
  {
    if (ros::isStarted())
    {
      ros::shutdown();  // explicitly needed since we use ros::start();
      ros::waitForShutdown();
    }
    wait();
  }
  bool QNode::init()
  {
    ros::init(init_argc, init_argv, "listener");
    if (!ros::master::check())
    {
      return false;
    }
    ros::start();  // explicitly needed since our nodehandle is going out of scope.
    ros::NodeHandle n;
    // Add your ros communications here.

    Cam_sub = n.subscribe("/camera/image", 1, &QNode::SubCallback, this);
    gazebo_pub = n.advertise<geometry_msgs::Twist>("/cmd_vel",30);
    start();
    return true;
  }

  void QNode::run()
  {
    ros::Rate loop_rate(33);
    while (ros::ok())
    {
      ros::spinOnce();
      loop_rate.sleep();
    }
    std::cout << "Ros shutdown, proceeding to close the gui." << std::endl;
    Q_EMIT rosShutdown();  // used to signal the gui for a shutdown (useful to roslaunch)
  }
  void QNode::pubcmdvel()
  {

          geometry_msgs::Twist msg;
          msg.angular = turtledevel.angular;
          msg.linear = turtledevel.linear;
          gazebo_pub.publish(msg);
        //  CheckS = false;

  }
  void QNode::SubCallback(const sensor_msgs::Image::ConstPtr &msg_img)
  {
     // if(Cam_img == NULL && !Isrecieved)
     // {
          Cam_img = new cv::Mat(cv_bridge::toCvCopy(msg_img, sensor_msgs::image_encodings::BGR8)->image);
          if(Cam_img !=NULL)
         {
              Isrecieved = true;
              Q_EMIT SubSignalsupport();

         }
      //}
  }
}  // namespace s
