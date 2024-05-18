#ifndef CANSERVICE_H
#define CANSERVICE_H

#include <string>
#include <vector>
#include <linux/can.h>
#include <net/if.h>

class ICANService {
public:
    virtual ~ICANService() = default;

    virtual void initialize(const std::string &interface) = 0;

    virtual struct can_frame readFrame() = 0;

    virtual void sendFrame(const struct can_frame &frame) = 0;

    virtual struct can_frame createFrame(uint32_t id, const std::string &data) = 0;
};

class CANService : public ICANService {
public:
    CANService();
    ~CANService() override;

    void initialize(const std::string &interface) override;

    struct can_frame readFrame() override;

    void sendFrame(const struct can_frame &frame) override;

    struct can_frame createFrame(uint32_t id, const std::string &data) override;

private:
    int sockfd;
    struct sockaddr_can sockaddr{};
    struct ifreq ifr{};

    void createSocket();

    void setupInterface(const std::string &interface);

    void bindSocket();
};

#endif // CANSERVICE_H
