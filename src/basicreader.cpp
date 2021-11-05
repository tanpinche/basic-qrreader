#include "basicreader.hpp"






int main(int argc, char **argv){
    
printf("starting node");
ros::init(argc, argv, "basicreader");

BasicReader basicreader1;
ros::spin();
return 0 ;

}