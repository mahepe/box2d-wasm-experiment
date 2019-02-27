cmake_minimum_required(VERSION 2.6)
ADD_DEFINITIONS(
    -std=c++11 # Or -std=c++0x
    # Other flags
)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB SRC
    "src/*.h"
    "src/*.cpp"
)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

if (${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
  link_directories(${CMAKE_SOURCE_DIR}/lib/web)
  set(CMAKE_CXX_COMPILER "em++")
  file(COPY assets/index.html DESTINATION ${CMAKE_BINARY_DIR})
  set(CMAKE_CXX_FLAGS "-s USE_SDL=2 -s USE_SDL_IMAGE=2")
  add_executable(main ${SRC})
  target_link_libraries(main Box2D)
  set_target_properties(main PROPERTIES LINK_FLAGS "${CMAKE_LINK_FLAGS} -std=c++11 -MMD -MP -Oz -s USE_SDL=2 --use-preload-plugins -s USE_SDL_IMAGE=2 -s WASM=1 --preload-file assets/sprite.png")
else ()
  link_directories(${CMAKE_SOURCE_DIR}/lib/native)
  include(FindPkgConfig)
  PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
  PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
  find_package(OpenGL REQUIRED)
  include_directories(${OPENGL_INCLUDE_DIR})
  add_executable(main ${SRC}) 
  target_link_libraries(main Box2D)
  target_link_libraries(main ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
endif ()
