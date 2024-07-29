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

class SerialCOM : public rclcpp::Node
{
public:
  SerialCOM()
  : Node("serial_com_node")
  {
    //init publisher to post encoder values and timer
    publisher_ = this->create_publisher<geometry_msgs::msg::Point>("encoder_counter", 10);
    timer_ = this->create_wall_timer(0.300ms, std::bind(&SerialCOM::timer_callback, this));//change the ms seconds to match on the ardino code for best posting speeds
    //init subscripber to pwm values
    subscription_ = this->create_subscription<geometry_msgs::msg::Point>(
      "pwm_values", 10, std::bind(&SerialCOM::topic_callback, this, _1));
    //open serial compunication channles. Change here is device serial path changes
    comport_in.open("/dev/ttyUSB1");
    comport_out.open("/dev/ttyUSB1");
  }

private:
  //when post to pwm_Values will pull messages and write to the micro controller
  void topic_callback(const geometry_msgs::msg::Point::SharedPtr msg)
  {
    //create string of pwm values to send to the micro controller. (notation. x = base, y = shoulder, z = elbow)
    pwm_process_values += std::to_string(static_cast<int>(msg->x)) + ":" + std::to_string(static_cast<int>(msg->y)) + ":"  + std::to_string(static_cast<int>(msg->z)) +";";
    //pushes values over the serial port for the micro controller to read
    comport_out << pwm_process_values << std::endl;
    pwm_process_values = "";
  }
  //timer callback to periodically post encoder values. change the timer_ paramerter ms for different hz
  void timer_callback()
  {
    //create empty message
    auto message = geometry_msgs::msg::Point();
    //intake encoder values from micro controller
    comport_in >> port_data;
    //check that intake data wasnt empty
    if (port_data != ""){
      //data was not empty. Loop though string and convert to integers 
      for (size_t i = 0; i < port_data.size(); i++) {
        //data being sent over is seperated by :. when you see that char you know at end of one encoder values
        if (port_data[i] == ':'){
          //Rare at the end of one encoder value. turn it into a int and store in joint vlaues
          joint_values[point_in_joint_array] = std::atoi(joint_value.c_str());//joint_value[0] == '-' ? std::atoi(joint_value.erase(0).c_str()) * -1 : std::atoi(joint_value.c_str());
          //reset variable to store current joint values being parsed. more one up in the joint arrray index
          joint_value = "";
          point_in_joint_array++;
          continue;
        }
        //end of the encoder data is ; char. now send data to the topic 
        if (port_data[i] == ';'){
          //convert last encoder data to int
          joint_values[point_in_joint_array] = std::atoi(joint_value.c_str());
          //reset variable to store current joint values being parsed. reset the joint array index
          joint_value = "";
          point_in_joint_array = 0;
          //set the message varaible to match with encoder data and publish. (notation. x = base, y = shoulder, z = elbow)
          message.x = joint_values[0];
          message.y = joint_values[1];
          message.z = joint_values[2];
          publisher_->publish(message);
          continue;
        }
        //error handing and edge case handles. checks for a non digit or blank space and a negative sign
        if ((!isdigit(port_data[i]) || port_data[i] == ' ') && port_data[i] != '-'){
          continue;
        }
        //add current joint data to current joint value string for further processing
        joint_value += port_data[i];
      }
    }
  }
  //shared pointer for the timer, publisher, and subscriber
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<geometry_msgs::msg::Point>::SharedPtr publisher_;
  rclcpp::Subscription<geometry_msgs::msg::Point>::SharedPtr subscription_;
  //create objects for the serial comunication for out and input
  std::ofstream comport_out;
  std::ifstream comport_in;
  //vars for processing the data sent by the micro processor
  std::string port_data;
  int joint_values[3]={0,0,0};
  std::string joint_value;
  //vars for processing data being sent to the micro processor
  int point_in_joint_array = 0;
  std::string pwm_process_values;

};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SerialCOM>());
  rclcpp::shutdown();
  return 0;
}