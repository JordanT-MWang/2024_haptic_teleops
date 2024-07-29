#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/point.hpp"
#include <chrono>
#include <string>
#include <fstream>
#include <iostream>
#include <stdio.h> 
#include <ctype.h>


using namespace std::chrono_literals;
using std::placeholders::_1;

class SerialWriteCOM : public rclcpp::Node
{
public:
  SerialWriteCOM()
  : Node("serial_com__node")
  {
    //publisher to test speeds of subscriber
    publisher_ = this->create_publisher<geometry_msgs::msg::Point>("sub_hz_test", 10);
    //subscriber to pwmvalues topic
    subscription_ = this->create_subscription<geometry_msgs::msg::Point>(
      "pwm_values", 10, std::bind(&SerialWriteCOM::topic_callback, this, _1));
    //open serial coms to send data to micro controller
    comport_out.open("/dev/ttyUSB0");
  }

private:
  //topic callback for subscriber. very simple will read topic 
  void topic_callback(const geometry_msgs::msg::Point::SharedPtr msg)
  {
    auto message = geometry_msgs::msg::Point();
    count+=1;
    message.x = count;
    publisher_->publish(message);
    pwm_process_values += std::to_string(static_cast<int>(msg->x)) + ":" + std::to_string(static_cast<int>(msg->y)) + ":"  + std::to_string(static_cast<int>(msg->z)) +";";
    //comport.flush();
    comport_out << pwm_process_values << std::endl;
    //pwm_process_values = "";
    //comport.flush();
  }
 
  int count =0;
  rclcpp::Publisher<geometry_msgs::msg::Point>::SharedPtr publisher_;
  
  std::ofstream comport_out;
 
  std::string pwm_process_values;
  rclcpp::Subscription<geometry_msgs::msg::Point>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
  
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SerialWriteCOM
>());
  rclcpp::shutdown();
  return 0;
}