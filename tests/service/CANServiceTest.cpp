#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../../src/service/CANService.h"

class MockCANService : public ICANService {
public:
    MOCK_METHOD(bool, initialize, (const std::string& interface), (override));
    MOCK_METHOD(bool, readFrame, (struct can_frame& frame), (override));
};

TEST(CANServiceTest, InitializeSuccess) {
  MockCANService mockCANService;
  EXPECT_CALL(mockCANService, initialize("vcan0"))
    .Times(1)
    .WillOnce(testing::Return(true));

  EXPECT_TRUE(mockCANService.initialize("vcan0"));
}

TEST(CANServiceTest, ReadFrameSuccess) {
  MockCANService mockCANService;
  struct can_frame frame{};
  EXPECT_CALL(mockCANService, readFrame(testing::_))
    .Times(1)
    .WillOnce(testing::DoAll(testing::SetArgReferee<0>(frame), testing::Return(true)));

  EXPECT_TRUE(mockCANService.readFrame(frame));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
