#pragma once

#include "Types.h"

constexpr i32 CHUNK_SIZE_X = 16;
constexpr i32 CHUNK_SIZE_Y = 100;
constexpr i32 CHUNK_SIZE_Z = 16;

constexpr i32 WORLD_SIZE_X = 100;
constexpr i32 WORLD_SIZE_Y = 100;
constexpr i32 WORLD_SIZE_Z = 100;

constexpr i32 CHUNK_VOLUME = CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z;

constexpr f32 BLOCK_SIZE = 1.0f;

constexpr f32 PLAYER_HEIGHT = 1.8f;
constexpr f32 PLAYER_EYE_HEIGHT = 1.6f;
constexpr f32 PLAYER_RADIUS = 0.3f;
constexpr f32 PLAYER_SPEED = 4.5f;
constexpr f32 PLAYER_JUMP_SPEED = 8.0f;
constexpr f32 GRAVITY = -20.0f;
constexpr f32 MAX_FALL_SPEED = 30.0f;

constexpr f32 MOUSE_SENSITIVITY = 0.002f;
constexpr f32 FOV = 75.0f;
constexpr f32 NEAR_PLANE = 0.1f;
constexpr f32 FAR_PLANE = 1000.0f;

constexpr i32 RENDER_DISTANCE = 8;

constexpr i32 INVENTORY_SLOTS = 5;
constexpr i32 INVENTORY_MAX_STACK = 100;
constexpr f32 RAYCAST_DISTANCE = 10.0f;
constexpr f32 BASE_BREAK_TIME = 10.0f;
constexpr f32 BLOCK_HIGHLIGHT_DISTANCE = 3.0f;

constexpr BlockID BLOCK_AIR = 0;
constexpr BlockID BLOCK_STONE = 1;
constexpr BlockID BLOCK_DIRT = 2;
constexpr BlockID BLOCK_GRASS = 3;
constexpr BlockID BLOCK_WOOD = 4;
constexpr BlockID BLOCK_LEAVES = 5;
constexpr BlockID BLOCK_SAND = 6;
constexpr BlockID BLOCK_WATER = 7;
constexpr BlockID BLOCK_BEDROCK = 8;
