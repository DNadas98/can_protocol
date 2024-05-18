#include "CANService.h"
#include <sys/socket.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <cstdio>
#include <cstring>

CANService::CANService() : sockfd(-1) {
  memset(&sockaddr, 0, sizeof(sockaddr));
  memset(&ifr, 0, sizeof(ifr));
}

CANService::~CANService() {
  if (sockfd >= 0) {
    close(sockfd);
  }
}

/**
 * @brief This initializes the CAN service by creating a socket and binding it to the specified interface.
 * @param interface The name of the network interface to use (e.g., "vcan0").
 * @return True if the initialization was successful, false otherwise.
 */
bool CANService::initialize(const std::string& interface) {
  // Create socket
  sockfd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
  if (sockfd < 0) {
    perror("Error while opening socket");
    return false;
  }

  // Set up interface
  strncpy(ifr.ifr_name, interface.c_str(), IFNAMSIZ - 1);
  ifr.ifr_name[IFNAMSIZ - 1] = '\0';

  // Get interface index
  if (ioctl(sockfd, SIOCGIFINDEX, &ifr) < 0) {
    perror("Error getting interface index");
    return false;
  }

  // Bind socket to CAN interface
  sockaddr.can_family = AF_CAN;
  sockaddr.can_ifindex = ifr.ifr_ifindex;
  if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
    perror("Error in socket bind");
    return false;
  }

  return true;
}

bool CANService::readFrame(struct can_frame& frame) {
  long nbytes = read(sockfd, &frame, sizeof(struct can_frame));
  if (nbytes < 0) {
    perror("Error while reading");
    return false;
  }
  return true;
}
