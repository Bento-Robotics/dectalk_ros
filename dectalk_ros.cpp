#include <stdio.h>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

#define DECTALK_BINARY DECTALK_BINARY_DIR "/say -pre '[:phoneme on]'  -"

class Dectalk_ROS : public rclcpp::Node {
public:
  Dectalk_ROS() : Node("dectalk_ros") {
    // start DECtalk
    dectalk_write_pipe = popen(DECTALK_BINARY, "w");
    if (!dectalk_write_pipe)
      RCLCPP_ERROR(this->get_logger(), "popen failed!");
    // turn off buffering - clashes with ROS2
    setvbuf(dectalk_write_pipe, NULL, _IONBF, 0);
    // create subscriber with class member function
    subscriber_string_ = this->create_subscription<std_msgs::msg::String>(
        "speak", 10,
        std::bind(&Dectalk_ROS::string_callback, this, std::placeholders::_1));
  }

  ~Dectalk_ROS() { pclose(dectalk_write_pipe); }

  void string_callback(std_msgs::msg::String::UniquePtr msg) {
    if (dectalk_write_pipe) {
      // append a newline (\n) so DECtalk reads individual messages as sentences
      if (fputs(msg->data.append("\n").c_str(), dectalk_write_pipe) == EOF)
        RCLCPP_ERROR(this->get_logger(), "dectalk pipe write failed");
    } else
      RCLCPP_ERROR(this->get_logger(), "dectalk pipe closed unexpectedly");
  }

private:
  FILE *dectalk_write_pipe;
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscriber_string_;
};

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Dectalk_ROS>());
  rclcpp::shutdown();
  return 0;
}
