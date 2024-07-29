#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/point.hpp"
#include <chrono>
#include <string>
#include <fstream>
#include <iostream>
#include <stdio.h> 
#include <ctype.h>

using namespace std::chrono_literals;

/* This example creates a subclass of Node and uses std::bind() to register a
 * member function as a callback from the timer. */

class SimplePublisher : public rclcpp::Node
{
public:
  SimplePublisher()
  : Node("simple_speed_publisher_node")
  {
    
    publisher_ = this->create_publisher<geometry_msgs::msg::Point>("counter", 10);
    //match with ardiuon while loop
    timer_ = this->create_wall_timer(
      0.250ms, std::bind(&SimplePublisher::timer_callback, this));
    comport.open("/dev/ttyUSB0");

  }

private:
  void timer_callback()
  {
    //RCLCPP_INFO_STREAM(this->get_logger(), "test1" << port_data << "test");
    auto message = geometry_msgs::msg::Point();
    //RCLCPP_INFO_STREAM(this->get_logger(), "test2" << port_data << "test");
    comport >> port_data;
    //RCLCPP_INFO_STREAM(this->get_logger(), "test3" << port_data << "test");

    if (port_data != ""){
      //RCLCPP_INFO_STREAM(this->get_logger(), point_in_array);
      for (size_t i = 0; i < port_data.size(); i++) {
        //RCLCPP_INFO_STREAM(this->get_logger(),  point_in_array << " " << port_data[i] << " " << i);
        if (port_data[i] == ':'){
          //RCLCPP_INFO_STREAM(this->get_logger(), point_in_array << " " << port_data.size() << " " << joint_value << " " << port_data);
          joint_values[point_in_array] = std::atoi(joint_value.c_str());//joint_value[0] == '-' ? std::atoi(joint_value.erase(0).c_str()) * -1 : std::atoi(joint_value.c_str());
          joint_value = "";
          point_in_array++;
          continue;
        }

        if (port_data[i] == ';'){
          joint_values[point_in_array] = std::atoi(joint_value.c_str());
          joint_value = "";
          point_in_array = 0;
          message.x = joint_values[0];
          //RCLCPP_INFO_STREAM(this->get_logger(), "test" << joint_values[0] << ' ' << joint_values[1] << ' ' <<  joint_values[2] << "test");
          message.y = joint_values[1];
          message.z = joint_values[2];
          publisher_->publish(message);
          continue;
        }
        
        if ((!isdigit(port_data[i]) || port_data[i] == ' ') && port_data[i] != '-'){
          continue;
        }
        
        
        joint_value += port_data[i];
      }
    }

    //port_data = "";
    
  }
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<geometry_msgs::msg::Point>::SharedPtr publisher_;
  
  std::fstream comport;
  std::string port_data;
  int joint_values[3]={0,0,0};
  std::string joint_value;
  int point_in_array = 0;
  
};

int main(int argc, char * argv[])
{
  
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SimplePublisher>());
  rclcpp::shutdown();
  return 0;
}