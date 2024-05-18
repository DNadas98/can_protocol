#include <cstdio>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <linux/can.h>


// ID = 291, DLC = 5, Data = 68 65 6C 6C 6F
// Text = hello
// ID = 292, DLC = 5, Data = 77 6F 72 6C 64
// Text = world

int main() {
  int sockfd;
  long nbytes;

  struct sockaddr_can sockaddr{};
  struct ifreq ifr{};
  struct can_frame frame{};

  // Create CAN socket
  sockfd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
  if (sockfd < 0) {
    perror("Error while opening socket");
    return -1;
  }

  // Set up CAN interface
  strcpy(ifr.ifr_name, "vcan0");

  // Get interface index
  ioctl(sockfd, SIOCGIFINDEX, &ifr);

  // Bind socket to CAN interface
  sockaddr.can_family = AF_CAN;
  sockaddr.can_ifindex = ifr.ifr_ifindex;
  if (bind(sockfd, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) < 0) {
    perror("Error in socket bind");
    return -2;
  }

  while (true){
    // Read CAN frame
    nbytes = read(sockfd, &frame, sizeof(struct can_frame));
    if (nbytes < 0) {
      perror("Error while reading");
      return -3;
    }

    // Print CAN frame
    printf("ID = %d, DLC = %d, Data = ", frame.can_id, frame.can_dlc);
    for (int i = 0; i < frame.can_dlc; i++) {
      printf("%02X ", frame.data[i]);
    }
    printf("\n");

    // Translate data to readable text
    printf("Text = ");
    for (int i = 0; i < frame.can_dlc; i++) {
      printf("%c", frame.data[i]);
    }
    printf("\n");
  }
}
