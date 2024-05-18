#ifndef CANSERVICE_H
#define CANSERVICE_H

#include <string>
#include <vector>
#include <linux/can.h>
#include <net/if.h>

class ICANService {
public:
    virtual ~ICANService() = default;
    virtual bool initialize(const std::string& interface) = 0;
    virtual bool readFrame(struct can_frame& frame) = 0;
};

class CANService : public ICANService {
public:
    CANService();
    ~CANService() override;

    bool initialize(const std::string& interface) override;
    bool readFrame(struct can_frame& frame) override;

private:
    int sockfd;
    struct sockaddr_can sockaddr{};
    struct ifreq ifr{};
};

#endif // CANSERVICE_H
