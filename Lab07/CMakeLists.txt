cmake_minimum_required(VERSION 3.24)
project(Lab07)

set(CMAKE_CXX_STANDARD 17)

add_executable(Lab07 main.cpp)

set(OpenCV_DIR "C:\\Users\\mikol\\Desktop\\NAI\\Lab07\\opencv-4.x\\cmake-build-debug")
find_package( OpenCV REQUIRED )

if( OpenCV_FOUND )
    include_directories( ${OpenCV_INCLUDE_DIRS} )
    link_directories( ${OpenCV_LIB_DIR} )
    #target_link_libraries( ocvdemo ${OpenCV_LIBS} ) # - to jest wersja dzialajaca na linux, ale nie na windows

    target_link_libraries( Lab07 opencv_core;opencv_highgui;opencv_imgcodecs;opencv_imgproc;opencv_videoio )
endif()

# Jesli chcesz wylistowac wszystkie zmienne, oto fragment (ukradziony ze stackoverflow)
#get_cmake_property(_variableNames VARIABLES)
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()
