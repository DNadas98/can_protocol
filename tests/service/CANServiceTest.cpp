#include "gtest/gtest.h"
#include "../../src/service/CANService.h"

TEST(CANServiceTest, InitializeSuccess) {
  CANService canService;
  EXPECT_NO_THROW(canService.initialize("vcan0"));
}

TEST(CANServiceTest, SendFrameSuccess) {
  CANService canService;
  ASSERT_NO_THROW(canService.initialize("vcan0"));

  struct can_frame frame = canService.createFrame(0x123, "11 22 33 44 55 66 77 88");
  EXPECT_NO_THROW(canService.sendFrame(frame));
}

TEST(CANServiceTest, CreateFrameSuccess) {
  CANService canService;
  uint32_t id = 0x123;
  std::string data = "11 22 33 44 55 66 77 88";

  struct can_frame frame = canService.createFrame(id, data);
  EXPECT_EQ(frame.can_id, id);
  EXPECT_EQ(frame.can_dlc, 8);
  EXPECT_EQ(frame.data[0], 0x11);
  EXPECT_EQ(frame.data[1], 0x22);
  EXPECT_EQ(frame.data[2], 0x33);
  EXPECT_EQ(frame.data[3], 0x44);
  EXPECT_EQ(frame.data[4], 0x55);
  EXPECT_EQ(frame.data[5], 0x66);
  EXPECT_EQ(frame.data[6], 0x77);
  EXPECT_EQ(frame.data[7], 0x88);
}

TEST(CANServiceTest, ReadFrameSuccess) {
  CANService canService;
  ASSERT_NO_THROW(canService.initialize("vcan0"));
  // TODO: Implement a way to test readFrame
  // struct can_frame frame = canService.readFrame();
  // EXPECT_EQ(frame.can_id, 0x123);
  // EXPECT_EQ(frame.data[0], 0x11);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
