#include "Inventory.h"

Inventory::Inventory() {
    m_hotbar[0] = { BLOCK_DIRT, 100 };
    m_hotbar[1] = { BLOCK_STONE, 100 };
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
    i32 remaining = count;
    for (auto& slot : m_hotbar) {
        if (slot.id == id && slot.count < MAX_STACK) {
            i32 space = MAX_STACK - slot.count;
            i32 add = (remaining < space) ? remaining : space;
            slot.count += add;
            remaining -= add;
            if (remaining <= 0) return;
        }
    }
    for (auto& slot : m_hotbar) {
        if (slot.id == BLOCK_AIR) {
            i32 add = (remaining < MAX_STACK) ? remaining : MAX_STACK;
            slot = { id, add };
            remaining -= add;
            if (remaining <= 0) return;
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

i32 Inventory::slotCount(i32 index) const {
    if (index < 0 || index >= HOTBAR_SIZE) return 0;
    return m_hotbar[index].count;
}

BlockID Inventory::slotID(i32 index) const {
    if (index < 0 || index >= HOTBAR_SIZE) return BLOCK_AIR;
    return m_hotbar[index].id;
}
