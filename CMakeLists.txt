cmake_minimum_required(VERSION 3.1.0)

set(PROJECT_NAME DWG-Radio)
project(${PROJECT_NAME})

set (${PROJECT_NAME}_MAJOR_VERSION 0)
set (${PROJECT_NAME}_MINOR_VERSION 1)
set (${PROJECT_NAME}_PATCH_VERSION 0)

set(${PROJECT_NAME}_VERSION
    ${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}.${${PROJECT_NAME}_PATCH_VERSION})
# This is used when configuring files
# (this way we have a variable which is not dependent on the project name)
set(PROJECT_VERSION ${${PROJECT_NAME}_VERSION})

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)
#create resourcees automatically
set(CMAKE_AUTORCC ON)


#specify qt version
set(QT_QMAKE_EXECUTABLE /opt/Qt/5.12.2/gcc_64/bin/qmake)

option(${PROJECT_NAME}_ENABLE_TESTS "Enable tests" ON)
if(${PROJECT_NAME}_ENABLE_TESTS)
    enable_testing()
endif()

if(MSVC)
    # Enable openmp
    #set(CMAKE_CXX_FLAGS_RELEASE "/openmp /GL")
    #set(CMAKE_INSTALL_OPENMP_LIBRARIES ON) # used by InstallRequiredSystemLibraries to install VCOMP*.dll
endif()

file(GLOB_RECURSE SOURCES         src/*.cpp)
file(GLOB_RECURSE HEADERS         src/*.cpp)
file(GLOB_RECURSE USER-INTERFACES src/*.ui)
file(GLOB_RECURSE RESSOURCES         src/*.qrc)

# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Network  CONFIG REQUIRED)
find_package(Qt5Multimedia  CONFIG REQUIRED)

#================================
# Subdirectories
#================================
# Directories in which to look for header files
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src )

add_executable(${PROJECT_NAME} ${SOURCES} ${RESSOURCES} ${HEADERS} )
# Use the Widgets module from Qt 5
target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Network Qt5::Multimedia)
