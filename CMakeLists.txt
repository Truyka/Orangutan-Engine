cmake_minimum_required(VERSION 3.0.0)
project(orangutan VERSION 0.4.1)

set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(CPP_SRCS 
    src/Graphics.cpp 
    src/Scene.cpp
    src/OgeGame.cpp
    src/InputHandler.cpp
    src/Timer.cpp
    src/Renderer.cpp
    src/OgeRandom.cpp
    src/OgeTest.cpp

    src/components/Animation.cpp
    src/components/Camera.cpp

    src/collision/AABBTree.cpp

    src/sound/Audio.cpp
    src/sound/SoundEffect.cpp

    src/math/OgeMath.cpp
    src/math/Vector2f.cpp
    src/math/Rect.cpp
    src/math/Ray.cpp

    src/systems/ScriptSystem.cpp
    src/systems/PhysicsSystem.cpp
    src/systems/AnimationSystem.cpp
    src/systems/FollowSystem.cpp
)

set(INCLUDE_DIRS 
    "C:/devel/SDL2/include/SDL2"
    "C:/devel/SDL2_image/include/SDL2"
    "C:/devel/SDL2_ttf/include/SDL2"
    "C:/devel/SDL2_mixer/include/SDL2"
    "src"
    "external"
)

add_subdirectory("external/aecs")

# TODO: replace with find_module
link_directories("C:/devel/SDL2/lib")
link_directories("C:/devel/SDL2_image/lib")
link_directories("C:/devel/SDL2_ttf/lib")
link_directories("C:/devel/SDL2_mixer/lib")

add_library(orangutan ${CPP_SRCS})

target_include_directories(orangutan PUBLIC ${INCLUDE_DIRS})
target_link_libraries(orangutan PUBLIC 
    aecslib 
    mingw32 
    sdl2main 
    sdl2 
    sdl2_image
    sdl2_ttf
    sdl2_mixer
)

add_executable(test main.cpp)

target_include_directories(test PUBLIC ${INCLUDE_DIRS})
target_link_libraries(test PUBLIC orangutan)

