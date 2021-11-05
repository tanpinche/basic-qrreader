#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/PoseStamped.h>
#include "espdrone_msgs/FullState.h"
#include "espdrone_msgs/BatteryStatus.h"

// #include <opencv2/objdetect.hpp>
// #include <opencv2/imgcodecs.hpp>
//  #include <opencv2/imgproc/imgproc.hpp>
//  #include <opencv2/highgui/highgui.hpp>
 #include <image_transport/image_transport.h>
 #include <cv_bridge/cv_bridge.h>
 #include <sensor_msgs/image_encodings.h>
 #include<opencv4/opencv2/objdetect.hpp>
 #include<opencv4/opencv2/imgcodecs.hpp>
 #include<opencv4/opencv2/imgproc/imgproc.hpp>
 #include<opencv4/opencv2/highgui/highgui.hpp>
 

//#include <espdrone_msgs/msg/FullState.msg>

class BasicReader
{
    public: BasicReader();

    private:
    ros::NodeHandle nh_;
    ros::Subscriber drone_sub_;
    ros::Publisher drone_control;
    bool found_qr;
    std::string last_data;

    void Callback(const sensor_msgs::Image::ConstPtr& msg);
    void Display (cv::Mat &im, cv::Mat &bbox);

};

BasicReader::BasicReader(){
    drone_sub_ = nh_.subscribe<sensor_msgs::Image>("/espdrone/image_rect_color", 10, &BasicReader::Callback, this);

}




void BasicReader::Callback(const sensor_msgs::Image::ConstPtr& msg){
     cv_bridge::CvImagePtr cv_ptr;
     cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
     cv::QRCodeDetector qrDecoder;
     cv::Mat bbox, rectifiedImage, image;
     image = cv_ptr->image;
     std::string data = qrDecoder.detectAndDecode(image, bbox, rectifiedImage);

     if(data.length()>0){
    
    if(last_data.compare(data)){
      found_qr = true;
      last_data = data;
     std::cout << "Decoded Data : " << data << std::endl;

     Display(image, bbox);
     rectifiedImage.convertTo(rectifiedImage, CV_8UC3);
     cv::imshow("Rectified QRCode", rectifiedImage);
     cv::waitKey(0);
     }


     }
     else {
       if(found_qr){
         found_qr = false;
         std::cout<<"No QR code detected"<<std::endl;
       }
     }
     
}

void BasicReader::Display(cv::Mat &im, cv::Mat &bbox)
{
  int n = bbox.rows;
  for(int i = 0 ; i < n ; i++)
  {
    line(im, cv::Point2i(bbox.at<float>(i,0),bbox.at<float>(i,1)), cv::Point2i(bbox.at<float>((i+1) % n,0), bbox.at<float>((i+1) % n,1)), cv::Scalar(255,0,0), 3);
  }
  imshow("Result", im);
}



    // float x, y, z;
    // x = msg->acc.x;
    // y = msg->acc.y;
    // z = msg->acc.z;

    // float pointx, pointy, pointz;
    // pointx = msg->pose.position.x;
    // pointy = msg->pose.position.y;
    // pointz = msg->pose.position.z;

    // float twistx, twisty, twistz;
    // twistx = msg->twist.linear.x;
    // twisty = msg->twist.linear.y;
    // twistz = msg->twist.linear.z;

    // printf("\nposition:x: %f y:%f z:%f ", pointx, pointy,pointz );

    // printf("batt: %f", msg->voltage);
    
    // printf("position:/nx: %f y:%f z%f \nvelocity \nx:%f y:%f z:%f\nacceleration\nx:%f y:%f z:%f", pointx, pointy,pointz, twistx,twisty,twistz, x,y,z );

