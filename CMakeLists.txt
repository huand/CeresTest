cmake_minimum_required(VERSION 3.3 FATAL_ERROR)
set (CMAKE_CXX_STANDARD 11)
project (myCeres)
set(CMAKE_BUILD_TYPE Debug)
find_package (Eigen3 3.3 REQUIRED NO_MODULE)
find_package(glog REQUIRED)
find_package(Gflags REQUIRED)
find_package(Ceres REQUIRED)
find_package(OpenCV 4 REQUIRED)

find_package(VTK COMPONENTS 
  ChartsCore
  CommonCore
  CommonDataModel
  InteractionStyle
  RenderingContext2D
  #RenderingContextOpenGL2
  RenderingCore
  RenderingFreeType
  RenderingGL2PSOpenGL2
  RenderingOpenGL2
  #ViewsContext2D
   REQUIRED)
message (STATUS "VTK_VERSION: ${VTK_VERSION}")
include_directories(${CERES_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${GFLAGS_INCLUDE_DIRS} ${GFLAGS_NAMESPACE} ${GFLAGS_LIBRARIES})
add_executable(helloworld helloworld.cc)
target_link_libraries(helloworld glog ceres)

#add_executable(powell powell.cc)
#target_link_libraries(powell ${GLOG_LIBRARIES} ${CERES_LIBRARIES} ${GFLAGS_LIBRARIES})
#
#add_executable(powell2 powell2.cc)
#target_link_libraries(powell2 ${GLOG_LIBRARIES} ${CERES_LIBRARIES} ${GFLAGS_LIBRARIES})
#
#add_executable(curvefitting curvefitting.cc)
#target_link_libraries(curvefitting
#${GLOG_LIBRARIES}
#${CERES_LIBRARIES}
#${GFLAGS_LIBRARIES}
#${OpenCV_LIBRARIES}
#${VTK_LIBRARIES}
#)
#vtk_module_autoinit(
#  TARGETS curvefitting
#  MODULES ${VTK_LIBRARIES}
#)
#
##add_executable(LinePlot MACOSX_BUNDLE LinePlot.cc )
##target_link_libraries(LinePlot PRIVATE ${VTK_LIBRARIES})
### vtk_module_autoinit is needed
##vtk_module_autoinit(
##  TARGETS LinePlot
##  MODULES ${VTK_LIBRARIES}
##)
#
##add_executable(test test.cc)
##
##message(WARNING ${CXX_STANDARD})