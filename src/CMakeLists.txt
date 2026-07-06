set(LIB_SOURCES
    engine/Engine.cpp
    engine/Window.cpp
    engine/Input.cpp

    rendering/Renderer.cpp
    rendering/Shader.cpp
    rendering/Texture.cpp
    rendering/Camera.cpp
    rendering/Mesh.cpp

    world/World.cpp
    world/Chunk.cpp
    world/Block.cpp
    world/BlockRegistry.cpp
    world/TerrainGenerator.cpp

    player/Player.cpp
    player/Inventory.cpp
)

add_library(VoxelCraftLib STATIC ${LIB_SOURCES})

target_include_directories(VoxelCraftLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(VoxelCraftLib PUBLIC
    glad
    glm::glm
    glfw
)

if(WIN32)
    target_link_libraries(VoxelCraftLib PRIVATE opengl32)
endif()

add_executable(VoxelCraft main.cpp)
target_link_libraries(VoxelCraft PRIVATE VoxelCraftLib)
