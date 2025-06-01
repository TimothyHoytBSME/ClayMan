cmake_minimum_required(VERSION 3.15)
project(ClayManager)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(EXAMPLES_DIR ${CMAKE_SOURCE_DIR}/examples)
set(EXAMPLES_OUTPUT_DIR ${CMAKE_BINARY_DIR}/examples)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g")

find_package(raylib REQUIRED)

#readme example
add_executable(readme-example ${EXAMPLES_DIR}/readme-example/readme-example.cpp clayman.cpp)
set_target_properties(readme-example PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXAMPLES_OUTPUT_DIR}/readme-example)
target_link_libraries(readme-example raylib)
file(COPY ${EXAMPLES_DIR}/readme-example/resources DESTINATION ${EXAMPLES_OUTPUT_DIR}/readme-example)
#end readme example

#full example
add_executable(full-example ${EXAMPLES_DIR}/full-example/full-example.cpp clayman.cpp)
set_target_properties(full-example PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXAMPLES_OUTPUT_DIR}/full-example)
target_link_libraries(full-example raylib)
file(COPY ${EXAMPLES_DIR}/full-example/resources DESTINATION ${EXAMPLES_OUTPUT_DIR}/full-example)
#end full example

#quick example
add_executable(quick-example ${EXAMPLES_DIR}/quick-example/quick-example.cpp clayman.cpp)
set_target_properties(quick-example PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXAMPLES_OUTPUT_DIR}/quick-example)
target_link_libraries(quick-example raylib)
file(COPY ${EXAMPLES_DIR}/quick-example/resources DESTINATION ${EXAMPLES_OUTPUT_DIR}/quick-example)
#end quick example

#advanced use example
add_executable(advanced-use-example ${EXAMPLES_DIR}/advanced-use-example/advanced-use-example.cpp clayman.cpp)
set_target_properties(advanced-use-example PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXAMPLES_OUTPUT_DIR}/advanced-use-example)
target_link_libraries(advanced-use-example raylib)
file(COPY ${EXAMPLES_DIR}/advanced-use-example/resources DESTINATION ${EXAMPLES_OUTPUT_DIR}/advanced-use-example)
#end advanced use example

#separate sources example
add_executable(separate-sources-example ${EXAMPLES_DIR}/separate-sources-example/separate-sources-example.cpp ${EXAMPLES_DIR}/separate-sources-example/layout.cpp clayman.cpp)
set_target_properties(separate-sources-example PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXAMPLES_OUTPUT_DIR}/separate-sources-example)
target_link_libraries(separate-sources-example raylib)
file(COPY ${EXAMPLES_DIR}/separate-sources-example/resources DESTINATION ${EXAMPLES_OUTPUT_DIR}/separate-sources-example)
#end separate sources example

#sdl2 example
# set(SDL2_STATIC_LIB ON)
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)
add_executable(sdl2-example ${EXAMPLES_DIR}/sdl2-example/sdl2-example.cpp clayman.cpp)
set_target_properties(sdl2-example PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXAMPLES_OUTPUT_DIR}/sdl2-example)
target_link_libraries(sdl2-example PRIVATE 
    SDL2::SDL2main
    SDL2::SDL2-static
    SDL2_ttf::SDL2_ttf-static
    SDL2_image::SDL2_image-static
)
file(COPY ${EXAMPLES_DIR}/sdl2-example/resources DESTINATION ${EXAMPLES_OUTPUT_DIR}/sdl2-example)
#end sdl2 example

#sdl3 example
# set(SDL3_STATIC_LIB ON)
find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(SDL3_image REQUIRED)
add_executable(sdl3-example ${EXAMPLES_DIR}/sdl3-example/sdl3-example.cpp clayman.cpp) #clayman.cpp
set_target_properties(sdl3-example PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXAMPLES_OUTPUT_DIR}/sdl3-example)
target_link_libraries(sdl3-example PRIVATE 
    SDL3::SDL3
    SDL3_ttf::SDL3_ttf
    SDL3_image::SDL3_image
)

file(COPY ${EXAMPLES_DIR}/sdl3-example/resources DESTINATION ${EXAMPLES_OUTPUT_DIR}/sdl3-example)
#end sdl3 example