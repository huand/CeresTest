cmake_minimum_required(VERSION 3.3 FATAL_ERROR)
set (CMAKE_CXX_STANDARD 17)
project (myCeres)
#set(CMAKE_BUILD_TYPE Debug)
#list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
find_package (Eigen3 3.3 REQUIRED NO_MODULE)
find_package(glog)
find_package(Ceres REQUIRED)
find_package(Gflags REQUIRED)
find_package(OpenCV REQUIRED)

find_package(VTK COMPONENTS 
  vtkChartsCore
  vtkCommonCore
  vtkCommonDataModel
  vtkInteractionStyle
  vtkRenderingContext2D
  vtkRenderingContextOpenGL2
  vtkRenderingCore
  vtkRenderingFreeType
  vtkRenderingGL2PSOpenGL2
  vtkRenderingOpenGL2
  vtkViewsContext2D QUIET)
message (STATUS "VTK_VERSION: ${VTK_VERSION}")
include_directories(${CERES_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${GFLAGS_INCLUDE_DIRS} ${GFLAGS_NAMESPACE} ${GFLAGS_LIBRARIES})
add_executable(helloworld helloworld.cc)
target_link_libraries(helloworld glog ceres)

add_executable(powell powell.cc)
target_link_libraries(powell ${GLOG_LIBRARIES} ${CERES_LIBRARIES} ${GFLAGS_LIBRARIES})

add_executable(curvefitting curvefitting.cc)
target_link_libraries(curvefitting
${GLOG_LIBRARIES}
${CERES_LIBRARIES}
${GFLAGS_LIBRARIES}
${OpenCV_LIBRARIES}
)
vtk_module_autoinit(
  TARGETS curvefitting
  MODULES ${VTK_LIBRARIES}
)

add_executable(LinePlot MACOSX_BUNDLE LinePlot.cc )
target_link_libraries(LinePlot PRIVATE ${VTK_LIBRARIES})
# vtk_module_autoinit is needed
vtk_module_autoinit(
  TARGETS LinePlot
  MODULES ${VTK_LIBRARIES}
)

add_executable(test test.cc)

message(WARNING ${CXX_STANDARD})