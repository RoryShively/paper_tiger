cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
set( CMAKE_VERBOSE_MAKEFILE ON )

project(paper_tiger)

set(CMAKE_CXX_STANDARD 11)

#add_executable(paper_tiger main.cpp Particle.cpp Particle.hpp)

get_filename_component( CINDER_PATH "~/CLionProjects/Cinder" ABSOLUTE )
get_filename_component( APP_PATH "${CMAKE_CURRENT_SOURCE_DIR}" ABSOLUTE )

include( ${CINDER_PATH}/proj/cmake/modules/cinderMakeApp.cmake )

find_package(  OpenCV REQUIRED )

set( CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} /Users/Rory/Development/libfreenect2 )
find_package( freenect2 REQUIRED )


ci_make_app(
        APP_NAME    "PaperTigerApp"
        CINDER_PATH ${CINDER_PATH}
        SOURCES     ${APP_PATH}/src/PaperTigerApp.cpp ${APP_PATH}/src/Particle.cpp ${APP_PATH}/src/ParticleController.cpp ${APP_PATH}/src/VectorFieldInput.cpp ${APP_PATH}/src/Kinect.cpp
        INCLUDES    ${APP_PATH}/include /Users/Rory/Development/libfreenect2/include
        LIBRARIES   ${OpenCV_LIBS} ${freenect2_LIBRARIES}
)
