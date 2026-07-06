#include "Inventory.h"

Inventory::Inventory() {
    // Start with some blocks
    m_hotbar[0] = { BLOCK_DIRT, 64 };
    m_hotbar[1] = { BLOCK_STONE, 64 };
    m_hotbar[2] = { BLOCK_WOOD, 64 };
}

void Inventory::selectSlot(i32 slot) {
    if (slot >= 0 && slot < HOTBAR_SIZE) {
        m_selectedSlot = slot;
    }
}

BlockID Inventory::currentBlock() const {
    return m_hotbar[m_selectedSlot].id;
}

void Inventory::addBlock(BlockID id, i32 count) {
    for (auto& slot : m_hotbar) {
        if (slot.id == id) {
            slot.count += count;
            return;
        }
    }
    for (auto& slot : m_hotbar) {
        if (slot.id == BLOCK_AIR) {
            slot = { id, count };
            return;
        }
    }
}

bool Inventory::removeBlock(BlockID id, i32 count) {
    for (auto& slot : m_hotbar) {
        if (slot.id == id && slot.count >= count) {
            slot.count -= count;
            if (slot.count <= 0) {
                slot = { BLOCK_AIR, 0 };
            }
            return true;
        }
    }
    return false;
}
