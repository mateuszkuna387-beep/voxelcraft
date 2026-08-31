#pragma once

#include <array>

#include "core/Types.h"
#include "core/Constants.h"

class Inventory {
public:
    Inventory();

    void selectSlot(i32 slot);
    BlockID currentBlock() const;
    void addBlock(BlockID id, i32 count);
    bool removeBlock(BlockID id, i32 count);

    i32 slotCount(i32 index) const;
    BlockID slotID(i32 index) const;
    i32 selectedSlot() const { return m_selectedSlot; }

    static constexpr i32 HOTBAR_SIZE = 5;
    static constexpr i32 MAX_STACK = 100;

private:
    struct Slot {
        BlockID id = BLOCK_AIR;
        i32 count = 0;
    };

    std::array<Slot, HOTBAR_SIZE> m_hotbar{};
    i32 m_selectedSlot = 0;
};
