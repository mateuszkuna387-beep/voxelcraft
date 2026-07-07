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

# Copy runtime DLLs next to executable on Windows
if(WIN32 AND MINGW)
    get_filename_component(MINGW_BIN_DIR ${CMAKE_CXX_COMPILER} DIRECTORY)
    add_custom_command(TARGET VoxelCraft POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            "${MINGW_BIN_DIR}/libstdc++-6.dll"
            "${MINGW_BIN_DIR}/libgcc_s_seh-1.dll"
            "${MINGW_BIN_DIR}/libwinpthread-1.dll"
            "${MINGW_BIN_DIR}/glfw3.dll"
            $<TARGET_FILE_DIR:VoxelCraft>
    )
endif()

# Copy assets (shaders) next to executable
add_custom_command(TARGET VoxelCraft POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:VoxelCraft>/assets
)
