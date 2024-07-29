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
/* This example creates a subclass of Node and uses std::bind() to register a
 * member function as a callback from the timer. */

class SerialReadCOM : public rclcpp::Node
{
public:
  SerialReadCOM()
  : Node("serial_read_node")
  {
    
    publisher_ = this->create_publisher<geometry_msgs::msg::Point>("encoder_counter", 10);
    timer_ = this->create_wall_timer(0.250ms, std::bind(&SerialReadCOM::timer_callback, this));
    comport_in.open("/dev/ttyUSB0");

  }

private:
  void timer_callback()
  {
    
    auto message = geometry_msgs::msg::Point();
    
    //RCLCPP_INFO_STREAM(this->get_logger(), "test2" << port_data << "test");
    
    
    //comport.flush();
    
    
    comport_in >> port_data;
    //for(size_t i = 0; i < port_data.size(); i++){
    //  RCLCPP_INFO_STREAM(this->get_logger(), port_data[i]);
    //}
    //comport.flush();
    
    //comport << pwm_process_values << std::endl;
    //pwm_process_values = "";
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
          //RCLCPP_INFO_STREAM(this->get_logger(), "test" << joint_values[0] << ' ' << joint_values[1] << ' ' <<  joint_values[2] << "test" << port_data);
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

    //comport << pwm_process_values << std::endl;
    
  }
 
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<geometry_msgs::msg::Point>::SharedPtr publisher_;
  
  std::ifstream comport_in;
  std::string port_data;
  int joint_values[3]={0,0,0};
  std::string joint_value;
  int point_in_array = 0;
};

int main(int argc, char * argv[])
{
  
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SerialReadCOM>());
  rclcpp::shutdown();
  return 0;
}