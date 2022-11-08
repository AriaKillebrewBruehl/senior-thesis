# CMakeLists.txt

# Older versions of CMake are likely to work just fine but, since
# I don't know where to cut off I just use the version I'm using
cmake_minimum_required(VERSION "3.17")

# name of this example project
project(edge-detection)

# set OpenCV_DIR variable equal to the path to the cmake
# files within the previously installed opencv program
set(OpenCV_DIR /Users/ariakillebrewbruehl/Documents/thesis/senior-thesis/opencv/install/lib/cmake/opencv4)

# Tell compiler to use C++ 14 features which is needed because
# Clang version is often behind in the XCode installation
set(CMAKE_CXX_STANDARD 14)

# configure the necessary common CMake environment variables
# needed to include and link the OpenCV program into this
# demo project, namely OpenCV_INCLUDE_DIRS and OpenCV_LIBS
find_package( OpenCV REQUIRED )

# tell the build to include the headers from OpenCV
include_directories( ${OpenCV_INCLUDE_DIRS} )

# specify the executable target to be built
add_executable(gs gray-scale.cpp read-save.cpp type2str.cpp)
add_executable(canny canny.cpp read-save.cpp type2str.cpp)
add_executable(DoG DoG.cpp read-save.cpp type2str.cpp)
add_executable(edge-extraction extract-edges.cpp canny.cpp DoG.cpp threshold-edges.cpp skeleton.cpp read-save.cpp type2str.cpp)
add_executable(skeleton skeleton.cpp read-save.cpp type2str.cpp)
add_executable(close close-gaps.cpp read-save.cpp type2str.cpp)
add_executable(tests tests.cpp extract-edges.cpp canny.cpp DoG.cpp threshold-edges.cpp skeleton.cpp gray-scale.cpp read-save.cpp type2str.cpp)
add_executable(thresh threshold-edges.cpp skeleton.cpp read-save.cpp type2str.cpp)

# tell it to link the executable target against OpenCV
target_link_libraries(gs ${OpenCV_LIBS})
target_link_libraries(canny ${OpenCV_LIBS})
target_link_libraries(DoG ${OpenCV_LIBS})
target_link_libraries(edge-extraction ${OpenCV_LIBS})
target_link_libraries(skeleton ${OpenCV_LIBS})
target_link_libraries(close ${OpenCV_LIBS})
target_link_libraries(thresh ${OpenCV_LIBS})
target_link_libraries(tests ${OpenCV_LIBS})
