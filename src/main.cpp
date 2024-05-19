#include "service/CANService.h"
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>
#include <iomanip>
#include <limits>

std::string stringToHex(const std::string &input);

void printFrame(const struct can_frame &frame);

void readUserInput(CANService &canService);

void handleHexInput(CANService &canService);

void handleTextInput(CANService &canService);

uint32_t readCanId();

void initializeCanInterface(CANService &canService);

void startCanReader(CANService &canService);

int main() {
  try {
    CANService canService;
    initializeCanInterface(canService);

    std::thread userInputThread(readUserInput, std::ref(canService));
    startCanReader(canService);

    userInputThread.join();
    return 0;
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return -1;
  }
}

void initializeCanInterface(CANService &canService) {
  std::string interfaceName;

  while (true) {
    std::cout << "Enter CAN interface name, or 'q' to quit: ";
    if (!std::getline(std::cin, interfaceName) || interfaceName == "q" || interfaceName == "Q") {
      exit(0);
    }

    try {
      canService.initialize(interfaceName);
      std::cout << "CAN reader started on interface " << interfaceName << std::endl;
      break;
    } catch (const std::exception &e) {
      std::cerr << "Error initializing CAN interface: " << e.what() << std::endl;
    }
  }
}

void startCanReader(CANService &canService) {
  while (true) {
    try {
      struct can_frame currentFrame = canService.readFrame();
      printFrame(currentFrame);
    } catch (const std::exception &e) {
      std::cerr << "Error reading frame: " << e.what() << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

std::string stringToHex(const std::string &input) {
  std::ostringstream hexStream;
  for (unsigned char c : input) {
    hexStream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c) << " ";
  }
  return hexStream.str();
}

void printFrame(const struct can_frame &frame) {
  std::cout << "ID = " << std::hex << frame.can_id << ", DLC = " << std::dec << static_cast<int>(frame.can_dlc)
            << ", Data = ";
  for (int i = 0; i < frame.can_dlc; i++) {
    std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(frame.data[i]) << " ";
  }
  std::cout << std::endl;

  std::cout << "Text = ";
  for (int i = 0; frame.data[i] && i < frame.can_dlc; i++) {
    std::cout << static_cast<char>(frame.data[i]);
  }
  std::cout << std::endl;
}

void readUserInput(CANService &canService) {
  while (true) {
    std::cout << "Press 'h' to send a CAN message in hex, 't' to send one in plain text or 'q' to exit." << std::endl;
    try {
      std::string input;
      if (!std::getline(std::cin, input)) {
        if (std::cin.eof()) {
          exit(0);
        } else {
          throw std::runtime_error("Invalid input.");
        }
      }

      if (input == "h" || input == "H") {
        handleHexInput(canService);
      } else if (input == "t" || input == "T") {
        handleTextInput(canService);
      } else if (input == "q" || input == "Q") {
        exit(0);
      } else {
        throw std::runtime_error("Invalid input.");
      }
    } catch (const std::exception &e) {
      std::cerr << "Error: " << e.what() << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

void handleHexInput(CANService &canService) {
  uint32_t id = readCanId();

  std::cout << "Enter message (HEX, space-separated): ";
  std::string hexMessage;
  if (!std::getline(std::cin, hexMessage)) {
    if (std::cin.eof()) {
      exit(0);
    } else {
      std::cerr << "Error reading message." << std::endl;
      return;
    }
  }

  // 2 hex chars / byte, 1 space between bytes
  if (hexMessage.size() > CAN_MAX_DLEN * 2 + CAN_MAX_DLEN - 1) {
    throw std::runtime_error("Message too long.");
  } else {
    struct can_frame frame = canService.createFrame(id, hexMessage);
    canService.sendFrame(frame);
    std::cout << "Message sent!" << std::endl;
  }
}

void handleTextInput(CANService &canService) {
  uint32_t id = readCanId();

  std::cout << "Enter message (plain text, max 8 characters): ";
  std::string message;
  if (!std::getline(std::cin, message)) {
    if (std::cin.eof()) {
      exit(0);
    } else {
      std::cerr << "Error reading message." << std::endl;
      return;
    }
  }

  if (message.size() > CAN_MAX_DLEN) {
    throw std::runtime_error("Message too long.");
  } else {
    std::string hexMessage = stringToHex(message);
    struct can_frame frame = canService.createFrame(id, hexMessage);
    canService.sendFrame(frame);
    std::cout << "Message sent!" << std::endl;
  }
}

uint32_t readCanId() {
  while (true) {
    std::cout << "Enter CAN ID (HEX): ";
    uint32_t id;
    if (!(std::cin >> std::hex >> id)) {
      if (std::cin.eof()) {
        exit(0);
      } else {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cerr << "Invalid CAN ID. Please enter a valid hexadecimal value." << std::endl;
      }
    } else {
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      return id;
    }
  }
}
