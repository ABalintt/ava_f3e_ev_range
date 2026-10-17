cmake_minimum_required(VERSION 3.8)
project(ava_f3e_ev_range)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

add_executable(ev_sensor_node src/ev_sensor_node.cpp)
ament_target_dependencies(ev_sensor_node rclcpp std_msgs)

add_executable(range_calc_node src/range_calc_node.cpp)
# add_executable(simple_sub_node src/print_pose.cpp)
ament_target_dependencies(range_calc_node rclcpp std_msgs)

install(TARGETS
  ev_sensor_node
  range_calc_node
  DESTINATION lib/${PROJECT_NAME})

install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME})

ament_package()