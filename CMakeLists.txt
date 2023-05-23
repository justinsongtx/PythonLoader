cmake_minimum_required(VERSION 3.0)

project(MyProject)

set(CMAKE_OSX_ARCHITECTURES x86_64)
set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_CXX_FLAGS "-g")

include_directories("./")

# Find Python library
find_package(PythonLibs REQUIRED)


FIND_PACKAGE(Boost)
IF (Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
    ADD_DEFINITIONS( "-DHAS_BOOST" )
    message(DEBUG, "Boost include directory:" ${Boost_INCLUDE_DIR})
else()
    message(FATAL_ERROR, "can not find Boost")
ENDIF()

# find_package(Boost REQUIRED COMPONENTS interprocess)
# include_directories(${Boost_INCLUDE_DIRS})

# Add the CPP file to be compiled
add_executable(myprogram main.cpp PythonLoader.cpp AudioToVideoConverter.cpp)

# Link the Python library to the executable
target_link_libraries(myprogram ${PYTHON_LIBRARIES} librt.so)
Message(python include dir "${PYTHON_INCLUDE_DIR}")
# Message(python libraries dir "${PYTHON_LIBRARIES}")

target_include_directories(myprogram PRIVATE ${PYTHON_INCLUDE_DIR})
