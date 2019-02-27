cmake_minimum_required(VERSION 2.6)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

if (${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
  set(CMAKE_CXX_COMPILER "em++")
  set(CMAKE_CXX_FLAGS "-s USE_SDL=2 -s USE_SDL_IMAGE=2")
  file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
  file(COPY assets/index.html DESTINATION ${CMAKE_BINARY_DIR})

  file(GLOB SRC
      "src/*.h"
      "src/*.cpp"
  )

  add_executable(main ${SRC}) 
  set_target_properties(main PROPERTIES LINK_FLAGS "${CMAKE_LINK_FLAGS} -std=c++11 -MMD -MP -Oz -s USE_SDL=2 --use-preload-plugins -s USE_SDL_IMAGE=2 -s WASM=1 --preload-file assets/sprite.png")
else ()
  include(FindPkgConfig)
  pkg_search_module(SDL2 REQUIRED sdl2)
  pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
  file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

  file(GLOB SRC
      "src/*.h"
      "src/*.cpp"
  )

  add_executable(main ${SRC}) 
  target_link_libraries(main ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
endif ()
