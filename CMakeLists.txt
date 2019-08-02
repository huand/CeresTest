cmake_minimum_required(VERSION 3.3 FATAL_ERROR)
set (CMAKE_CXX_STANDARD 17)
project (myCeres)
set(CMAKE_BUILD_TYPE Debug)
find_package (Eigen3 3.3 REQUIRED NO_MODULE)
find_package(glog REQUIRED)
find_package(Gflags REQUIRED)
find_package(Ceres REQUIRED)
find_package(OpenCV 4 REQUIRED)

add_executable(helloworld helloworld.cc)
target_link_libraries(helloworld glog ceres)

add_executable(powell powell.cc)
target_link_libraries(powell ${GLOG_LIBRARIES} ${CERES_LIBRARIES} ${GFLAGS_LIBRARIES})


add_executable(curvefitting curvefitting.cc)
target_link_libraries(curvefitting
${CERES_LIBRARIES}
${OpenCV_LIBRARIES}
)

add_executable(BundleAdjustment BundleAdjustment.cc)
target_link_libraries(BundleAdjustment glog ceres)

add_executable(BundleAdjSimpleRef BundleAdjSimpleRef.cc)
target_link_libraries(BundleAdjSimpleRef glog ceres)

