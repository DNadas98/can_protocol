#include "service/CANService.h"
#include <iostream>

int main() {
  CANService canService;
  if (!canService.initialize("vcan0")) {
    return -1;
  }

  struct can_frame frame{};
  while (true) {
    if (canService.readFrame(frame)) {
      std::cout << "ID = " << frame.can_id << ", DLC = " << static_cast<int>(frame.can_dlc) << ", Data = ";
      for (int i = 0; i < frame.can_dlc; i++) {
        std::cout << std::hex << static_cast<int>(frame.data[i]) << " ";
      }
      std::cout << std::endl;

      // Translate data to readable text
      std::cout << "Text = ";
      for (int i = 0; i < frame.can_dlc; i++) {
        std::cout << static_cast<char>(frame.data[i]);
      }
      std::cout << std::endl;
    }
  }

  return 0;
}
