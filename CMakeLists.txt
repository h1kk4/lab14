cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 11)
include(cmake/HunterGate.cmake)

HunterGate(
        URL "https://github.com/h1kk4/hunter/archive/v0.19.137.1.tar.gz"
        SHA1 "8667deec901df1cade767c847ffacdbf26f2efd1"
)

project(lab14)

hunter_add_package(Boost COMPONENTS program_options system)

find_package(Boost CONFIG REQUIRED program_options system filesystem)

set(SOURCE_FILES builder.cpp)

add_executable(lab14 ${SOURCE_FILES})

target_link_libraries(lab14 Boost::program_options Boost::system )

install(TARGETS lab14 RUNTIME DESTINATION bin)