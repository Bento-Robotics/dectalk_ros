#include <iostream>
#include <stdio.h>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

#define DECTALK_BINARY DECTALK_BINARY_DIR "/say -"

class Dectalk_ROS {
public:
  Dectalk_ROS() {
    dectalk_write_pipe = popen(DECTALK_BINARY, "w");
    if (!dectalk_write_pipe)
      std::cout << "popen failed!";
  }

  ~Dectalk_ROS() { pclose(dectalk_write_pipe); }

  void say(std::string text) {
    fputs(text.append("\n").c_str(), dectalk_write_pipe);
  }

private:
  FILE *dectalk_write_pipe;
};

int main() {
  Dectalk_ROS dt = Dectalk_ROS();
  dt.say("jazz tastic");
  dt.say("Henlo Wordl");
  return 0;
}
