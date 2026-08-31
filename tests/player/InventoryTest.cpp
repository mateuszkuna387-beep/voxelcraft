#include <gtest/gtest.h>

#include "player/Inventory.h"
#include "core/Constants.h"

TEST(InventoryTest, DefaultSlotsCount) {
    Inventory inv;
    EXPECT_EQ(inv.slotCount(0), 100);
    EXPECT_EQ(inv.slotCount(1), 100);
    EXPECT_EQ(inv.slotCount(2), 0);
    EXPECT_EQ(inv.slotCount(3), 0);
    EXPECT_EQ(inv.slotCount(4), 0);
}

TEST(InventoryTest, DefaultSlotsID) {
    Inventory inv;
    EXPECT_EQ(inv.slotID(0), BLOCK_DIRT);
    EXPECT_EQ(inv.slotID(1), BLOCK_STONE);
    EXPECT_EQ(inv.slotID(2), BLOCK_AIR);
}

TEST(InventoryTest, SelectedSlotDefault) {
    Inventory inv;
    EXPECT_EQ(inv.selectedSlot(), 0);
}

TEST(InventoryTest, SelectSlot) {
    Inventory inv;
    inv.selectSlot(2);
    EXPECT_EQ(inv.selectedSlot(), 2);
}

TEST(InventoryTest, SelectSlotOutOfRange) {
    Inventory inv;
    inv.selectSlot(10);
    EXPECT_EQ(inv.selectedSlot(), 0);
    inv.selectSlot(-1);
    EXPECT_EQ(inv.selectedSlot(), 0);
}

TEST(InventoryTest, CurrentBlockMatchesSelected) {
    Inventory inv;
    inv.selectSlot(0);
    EXPECT_EQ(inv.currentBlock(), BLOCK_DIRT);
    inv.selectSlot(1);
    EXPECT_EQ(inv.currentBlock(), BLOCK_STONE);
    inv.selectSlot(2);
    EXPECT_EQ(inv.currentBlock(), BLOCK_AIR);
}

TEST(InventoryTest, AddBlockToExistingStack) {
    Inventory inv;
    inv.addBlock(BLOCK_DIRT, 1);
    EXPECT_EQ(inv.slotCount(0), 100);
}

TEST(InventoryTest, AddBlockFillsSlotToMax) {
    Inventory inv;
    inv.addBlock(BLOCK_DIRT, 50);
    EXPECT_EQ(inv.slotCount(0), 100);
}

TEST(InventoryTest, AddBlockToEmptySlot) {
    Inventory inv;
    inv.addBlock(BLOCK_WOOD, 30);
    EXPECT_EQ(inv.slotID(2), BLOCK_WOOD);
    EXPECT_EQ(inv.slotCount(2), 30);
}

TEST(InventoryTest, AddBlockFillsPartialSlot) {
    Inventory inv;
    inv.removeBlock(BLOCK_DIRT, 30);
    EXPECT_EQ(inv.slotCount(0), 70);
    inv.addBlock(BLOCK_DIRT, 20);
    EXPECT_EQ(inv.slotCount(0), 90);
}

TEST(InventoryTest, AddBlockSpillsToNewSlot) {
    Inventory inv;
    inv.removeBlock(BLOCK_DIRT, 1);
    inv.addBlock(BLOCK_DIRT, 100);
    EXPECT_EQ(inv.slotCount(0), 100);
    EXPECT_EQ(inv.slotID(2), BLOCK_DIRT);
    EXPECT_EQ(inv.slotCount(2), 99);
}

TEST(InventoryTest, AddBlockMultipleSlots) {
    Inventory inv;
    inv.removeBlock(BLOCK_DIRT, 50);
    inv.removeBlock(BLOCK_STONE, 100);
    inv.removeBlock(BLOCK_DIRT, 50);
    inv.addBlock(BLOCK_DIRT, 120);
    EXPECT_EQ(inv.slotCount(0), 100);
    EXPECT_EQ(inv.slotCount(1), 20);
}

TEST(InventoryTest, AddBlockDoesNotOverflowMaxStack) {
    Inventory inv;
    inv.removeBlock(BLOCK_DIRT, 50);
    inv.addBlock(BLOCK_DIRT, 200);
    EXPECT_EQ(inv.slotCount(0), 100);
    EXPECT_EQ(inv.slotCount(2), 100);
    EXPECT_EQ(inv.slotCount(3), 50);
}

TEST(InventoryTest, RemoveBlockReducesCount) {
    Inventory inv;
    EXPECT_TRUE(inv.removeBlock(BLOCK_DIRT, 10));
    EXPECT_EQ(inv.slotCount(0), 90);
}

TEST(InventoryTest, RemoveBlockClearsSlot) {
    Inventory inv;
    EXPECT_TRUE(inv.removeBlock(BLOCK_DIRT, 100));
    EXPECT_EQ(inv.slotID(0), BLOCK_AIR);
    EXPECT_EQ(inv.slotCount(0), 0);
}

TEST(InventoryTest, RemoveBlockReturnsFalseIfNotEnough) {
    Inventory inv;
    EXPECT_FALSE(inv.removeBlock(BLOCK_DIRT, 200));
    EXPECT_EQ(inv.slotCount(0), 100);
}

TEST(InventoryTest, RemoveBlockReturnsFalseIfNotPresent) {
    Inventory inv;
    EXPECT_FALSE(inv.removeBlock(BLOCK_WOOD, 1));
}

TEST(InventoryTest, AddBlockFillsAllSlots) {
    Inventory inv;
    inv.addBlock(BLOCK_SAND, 500);
    EXPECT_EQ(inv.slotID(2), BLOCK_SAND);
    EXPECT_EQ(inv.slotID(3), BLOCK_SAND);
    EXPECT_EQ(inv.slotID(4), BLOCK_SAND);
    EXPECT_EQ(inv.slotCount(2), 100);
    EXPECT_EQ(inv.slotCount(3), 100);
    EXPECT_EQ(inv.slotCount(4), 100);
}
