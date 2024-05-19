#include "CANService.h"
#include <sys/socket.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <cstring>
#include <sstream>

CANService::CANService() : sockfd(-1) {
  memset(&sockaddr, 0, sizeof(sockaddr));
  memset(&ifr, 0, sizeof(ifr));
}

CANService::~CANService() {
  closeSocket();
}

void CANService::initialize(const std::string &interface) {
  closeSocket();
  createSocket();
  setupInterface(interface);
  bindSocket();
}

void CANService::createSocket() {
  sockfd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
  if (sockfd < 0) {
    throw std::runtime_error("Error while opening socket");
  }
}

void CANService::setupInterface(const std::string &interface) {
  strncpy(ifr.ifr_name, interface.c_str(), IFNAMSIZ - 1);
  ifr.ifr_name[IFNAMSIZ - 1] = '\0';
  if (ioctl(sockfd, SIOCGIFINDEX, &ifr) < 0) {
    throw std::runtime_error("Error getting interface index");
  }
}

void CANService::bindSocket() {
  sockaddr.can_family = AF_CAN;
  sockaddr.can_ifindex = ifr.ifr_ifindex;
  if (bind(sockfd, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) < 0) {
    throw std::runtime_error("Error in socket bind");
  }
}

struct can_frame CANService::readFrame() {
  struct can_frame frame{};
  long nbytes = read(sockfd, &frame, sizeof(struct can_frame));
  if (nbytes < 0) {
    throw std::runtime_error("Error while reading frame");
  }
  return frame;
}

void CANService::sendFrame(const struct can_frame &frame) {
  long nbytes = write(sockfd, &frame, sizeof(struct can_frame));
  if (nbytes < 0) {
    throw std::runtime_error("Error while writing frame");
  }
}

struct can_frame CANService::createFrame(uint32_t id, const std::string &data) {
  if (id > 0x7FF) {
    throw std::runtime_error("Invalid CAN ID. ID must be a standard 11-bit identifier.");
  }
  struct can_frame frame{};
  frame.can_id = id;
  std::istringstream iss(data);
  std::string byteStr;
  int byteIndex = 0;
  while (iss >> byteStr) {
    if (byteStr.size() != 2) {
      throw std::runtime_error("Invalid byte format.");
    }
    if (byteIndex >= CAN_MAX_DLEN) {
      throw std::runtime_error("Data too long.");
    }
    frame.data[byteIndex++] = static_cast<uint8_t>(std::stoul(byteStr, nullptr, 16));
  }
  frame.can_dlc = byteIndex;
  return frame;
}

void CANService::closeSocket() {
  if (sockfd >= 0) {
    close(sockfd);
    sockfd = -1;
  }
}
