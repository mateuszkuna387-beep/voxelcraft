set(TEST_SOURCES
    main.cpp
    world/ChunkTest.cpp
    world/BlockTest.cpp
    world/TerrainGeneratorTest.cpp
    rendering/CameraTest.cpp
    player/PlayerTest.cpp
    gui/MenuTest.cpp
)

add_executable(run_tests ${TEST_SOURCES})

target_include_directories(run_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(run_tests PRIVATE gtest_main VoxelCraftLib)

# Copy runtime DLLs next to test executable on Windows
if(WIN32 AND MINGW)
    get_filename_component(MINGW_BIN_DIR ${CMAKE_CXX_COMPILER} DIRECTORY)
    add_custom_command(TARGET run_tests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            "${MINGW_BIN_DIR}/libstdc++-6.dll"
            "${MINGW_BIN_DIR}/libgcc_s_seh-1.dll"
            "${MINGW_BIN_DIR}/libwinpthread-1.dll"
            "${MINGW_BIN_DIR}/glfw3.dll"
            $<TARGET_FILE_DIR:run_tests>
    )
endif()

add_test(NAME VoxelCraftTests COMMAND run_tests)
