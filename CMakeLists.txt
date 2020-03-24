# Lines that begin with a # are comments
# set the minimum required version of cmake, usually the first line
cmake_minimum_required(VERSION 3.0)

# project_name sets the name of the project and causes cmake to
# find the c and c++ compilers
project(K-Bandits)

# Find your dependencies.
# Many libraries ship with files that allow CMake to find them
# Then general behavior is to call "find_package" but the options
# provided are package specific.  Usually there is then a CMAKE variable
# That is defined to reference the library
# here: we find the eigen library as per the instruction
# https://eigen.tuxfamily.org/dox/TopicCMakeGuide.html
#find_package(Eigen3 3.3 REQUIRED NO_MODULE)

# Create an executable from the following source code files
# The Name of the executable creates a cmake "target"
add_executable(myAgent myAgent.cpp)

# Create a library.  Can specify if it is shared or static but usually
# you don't need or want to.
# name is the name of the library without the extension or lib prefix
# name creates a cmake "target"
#add_library(libname fileA.cpp fileB.cpp)
add_library(stdlib myAgent.cpp)
add_library(iostream myAgent.cpp)
add_library(random myAgent.cpp)

# Use target_link_libraries to add dependencies to a "target"
# (e.g., a library or executable)
# This will automatically add all required library files
# that need to be linked
# and paths to th locations of header files
#target_link_libraries(Name Eigen3::Eigen)

# specify additional compilation flags for a target
#target_compile_options(Agent -Wall)

# This is a line you will see in catkin cmake files to make sure that the projet depends on
# special catkin targets (such as any messages that are generated)
#add_dependencies(${PROJECT_NAME}_${PARSED_ARGS_TARGET} 
#   ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
