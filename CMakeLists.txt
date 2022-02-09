cmake_minimum_required(VERSION 3.10)

project(my_project)

#set(HFILES summa.h)
set(CPPFILES src/main.cpp)

#add_executable(${HFILES} ${CPPFILES})
#add_executable(CModule ${CPPFILES})

add_subdirectory(extern/pybind11)
pybind11_add_module(CModule ${CPPFILES})



