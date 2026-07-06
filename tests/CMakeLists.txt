set(TEST_SOURCES
    main.cpp
    world/ChunkTest.cpp
    world/BlockTest.cpp
    world/TerrainGeneratorTest.cpp
    rendering/CameraTest.cpp
    player/PlayerTest.cpp
)

add_executable(run_tests ${TEST_SOURCES})

target_include_directories(run_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(run_tests PRIVATE gtest_main VoxelCraftLib)

add_test(NAME VoxelCraftTests COMMAND run_tests)
