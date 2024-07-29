#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/point.hpp"
#include <chrono>
#include <string>
#include <fstream>
#include <iostream>
#include <stdio.h> 
#include <ctype.h>



using std::placeholders::_1;

class SerialWriter : public rclcpp::Node
{
public:
  SerialWriter()
  : Node("serial_writer")
  {
    subscription_ = this->create_subscription<geometry_msgs::msg::Point>(
      "pwm_values", 10, std::bind(&SerialWriter::topic_callback, this, _1));
    comport.open("/dev/ttyUSB0");
    
  }
  
  ~SerialWriter(){
    //comport.close();
  }

private:
  void topic_callback(const geometry_msgs::msg::Point::SharedPtr msg)
  {
    
    //comport.open("/dev/ttyUSB0");
    start = std::chrono::high_resolution_clock::now();
    
    //RCLCPP_INFO(this->get_logger(), "test1");
    pwm_process_values += std::to_string(static_cast<int>(msg->x)) + ":" + std::to_string(static_cast<int>(msg->y)) + ":"  + std::to_string(static_cast<int>(msg->z)) +";";
    
    //RCLCPP_INFO(this->get_logger(), "test2");
    //strcpy(pwm_write_values,pwm_process_values.c_str());
    //RCLCPP_INFO_STREAM(this->get_logger(), "test3 " << pwm_write_values << "test");
    
    comport << pwm_process_values << std::endl;
    //fprintf(ControllerPort, "%s\n", pwm_process_values);
    //RCLCPP_INFO(this->get_logger(), "test4");
    pwm_process_values = "";
    
    //comport.close();
    end = std::chrono::high_resolution_clock::now();
    overall_time = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
    //strcpy(output_time, std::to_string(static_cast<int>(overall_time)).c_str());
    RCLCPP_INFO_STREAM(this->get_logger(), "time " << overall_time);
  }
  rclcpp::Subscription<geometry_msgs::msg::Point>::SharedPtr subscription_;
  /*
  FILE *ControllerPort;
  change to ofstream if fails
  */
  std::fstream comport;
  std::string pwm_process_values;
  char pwm_write_values[12];
  //timer values
  //auto start;
  //auto end;
  //auto overall_time;
  //const char output_time[25];
  std::chrono::_V2::system_clock::time_point start, end;
  long long overall_time; 
  
  
  
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SerialWriter>());
  rclcpp::shutdown();
  return 0;
}