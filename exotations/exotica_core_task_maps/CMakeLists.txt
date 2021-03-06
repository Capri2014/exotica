cmake_minimum_required(VERSION 2.8.12)
project(exotica_core_task_maps)

find_package(catkin REQUIRED COMPONENTS
  exotica_core
  exotica_python
  eigen_conversions
  geometry_msgs
)

AddInitializer(
  center_of_mass
  interaction_mesh
  eff_position
  eff_orientation
  eff_frame
  frame_with_axis_and_direction
  eff_axis_alignment
  eff_velocity
  joint_limit
  joint_velocity_limit
  distance
  point_to_line
  joint_pose
  sphere_collision
  sphere
  collision_check
  point_to_plane
  quasi_static
  joint_velocity_backward_difference
  joint_acceleration_backward_difference
  joint_jerk_backward_difference
  look_at
  manipulability
)
GenInitializers()

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES ${PROJECT_NAME}
  CATKIN_DEPENDS exotica_core geometry_msgs
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

set(SOURCES
    src/center_of_mass.cpp
    src/interaction_mesh.cpp
    src/eff_position.cpp
    src/eff_orientation.cpp
    src/eff_frame.cpp
    src/eff_axis_alignment.cpp
    src/eff_velocity.cpp
    src/joint_limit.cpp
    src/joint_velocity_limit.cpp
    src/distance.cpp
    src/point_to_line.cpp
    src/point_to_plane.cpp
    src/joint_pose.cpp
    src/sphere_collision.cpp
    src/collision_check.cpp
    src/quasi_static.cpp
    src/joint_velocity_backward_difference.cpp
    src/joint_acceleration_backward_difference.cpp
    src/joint_jerk_backward_difference.cpp
    src/look_at.cpp
    src/manipulability.cpp
)

add_library(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${catkin_LIBRARIES})
add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}_initializers ${catkin_EXPORTED_TARGETS})

pybind_add_module(${PROJECT_NAME}_py MODULE src/task_map_py.cpp)
target_link_libraries(${PROJECT_NAME}_py PRIVATE ${PROJECT_NAME})
add_dependencies(${PROJECT_NAME}_py ${PROJECT_NAME} ${PROJECT_NAME}_initializers ${catkin_EXPORTED_TARGETS})

## Install
install(TARGETS ${PROJECT_NAME}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
install(DIRECTORY include/ DESTINATION include)
install(FILES exotica_plugins.xml DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})
install(TARGETS ${PROJECT_NAME}_py LIBRARY DESTINATION ${CATKIN_GLOBAL_PYTHON_DESTINATION})

if(CATKIN_ENABLE_TESTING)
  catkin_add_gtest(test_maps test/test_maps.cpp)
  target_link_libraries(test_maps ${catkin_LIBRARIES})
  add_dependencies(test_maps ${catkin_EXPORTED_TARGETS})
endif()