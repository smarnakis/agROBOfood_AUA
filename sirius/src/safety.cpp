#include "sirius.hpp"

int main(int argc, char** argv){
  ros::init(argc, argv, "safety_node");

  ros::NodeHandle nh;

  safety safety_obj(nh);

  ros::spin();

  return 0;
}