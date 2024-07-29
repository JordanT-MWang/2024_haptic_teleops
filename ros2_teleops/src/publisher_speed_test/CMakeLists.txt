cmake_minimum_required(VERSION 3.8)
project(publisher_speed_test)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)

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
add_executable(simple_speed_publisher src/simple_test_publisher)
ament_target_dependencies(simple_speed_publisher rclcpp geometry_msgs)

add_executable(simple_com_publisher src/com_port_test)
ament_target_dependencies(simple_com_publisher rclcpp geometry_msgs)

install(TARGETS 
  simple_speed_publisher
  simple_com_publisher
  DESTINATION lib/${PROJECT_NAME}
  )

install(DIRECTORY 
    launch
    DESTINATION share/${PROJECT_NAME}/
    )
ament_package()
