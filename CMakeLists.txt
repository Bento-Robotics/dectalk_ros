cmake_minimum_required(VERSION 3.8)
project(dectalk_ros)

# download dectalk precompiled
include(FetchContent)

FetchContent_Declare(dectalk_binary
  URL "https://github.com/dectalk/dectalk/releases/download/2023-10-30/ubuntu-latest.tar.gz"
  URL_HASH SHA256=185d94dbb5f51f6f866a4d4521b39eba486095753d21d398578cc573604e3b0c
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
  BINARY_DIR .
)

FetchContent_MakeAvailable(dectalk_binary)

add_definitions( -DDECTALK_BINARY_DIR="${dectalk_binary_SOURCE_DIR}" )


# --- ROS2 from here on ---
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

add_executable(${PROJECT_NAME}_node dectalk_ros.cpp)

ament_target_dependencies(${PROJECT_NAME}_node rclcpp std_msgs)

install(TARGETS
  ${PROJECT_NAME}_node
  DESTINATION lib/${PROJECT_NAME})

ament_package()