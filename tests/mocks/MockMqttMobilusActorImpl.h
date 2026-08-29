#pragma once

#include "crypto/Encryptor.h"
#include "jungi/mobgtw/Envelope.h"
#include "jungi/mobgtw/io/SocketEvents.h"

#include <google/protobuf/message_lite.h>
#include <mosquitto.h>

#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

static constexpr char kClientId[] = "123456789ABC";
static constexpr std::array<uint8_t, 6> kClientIdBinary = { 0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc };
static constexpr char kSerialNumber[] = "MB123456";
static constexpr char kUsername[] = "admin";
static constexpr char kPassword[] = "admin";
static constexpr char kCommandTopic[] = "module";
static constexpr char kEventsTopic[] = "clients";

namespace jungi::mobgtw::proto {

class CallEvents;

}

namespace jungi::mobgtw::tests::mocks {

class MockMqttMobilusActorImpl final {
public:
    enum class Commands {
        ReplyClient,
        ShareMessage,
        MockResponse,
    };

    struct Command {
        virtual ~Command() = default;
        [[nodiscard]] virtual Commands commandId() const = 0;
    };

    struct ReplyClientCommand final : Command {
        std::unique_ptr<const google::protobuf::MessageLite> message;

        explicit ReplyClientCommand(std::unique_ptr<const google::protobuf::MessageLite> aMessage)
            : message(std::move(aMessage))
        {
        }

        Commands commandId() const override { return Commands::ReplyClient; }
    };

    struct ShareMessageCommand final : Command {
        std::unique_ptr<const google::protobuf::MessageLite> message;

        explicit ShareMessageCommand(std::unique_ptr<const google::protobuf::MessageLite> aMessage)
            : message(std::move(aMessage))
        {
        }

        Commands commandId() const override { return Commands::ShareMessage; }
    };

    struct MockResponseCommand final : Command {
        uint8_t requestType;
        std::unique_ptr<const google::protobuf::MessageLite> response;

        MockResponseCommand(uint8_t aRequestType, std::unique_ptr<const google::protobuf::MessageLite> aResponse)
            : requestType(aRequestType)
            , response(std::move(aResponse))
        {
        }

        Commands commandId() const override { return Commands::MockResponse; }
    };

    MockMqttMobilusActorImpl(std::string host, uint16_t port);
    ~MockMqttMobilusActorImpl();

    bool connect();
    void handle(ReplyClientCommand& cmd);
    void handle(ShareMessageCommand& cmd);
    void handle(MockResponseCommand& cmd);

    [[nodiscard]] int socketFd();
    [[nodiscard]] io::SocketEvents socketEvents();
    void handleSocketEvents(io::SocketEvents revents);

private:
    using MockResponseMap = std::unordered_map<uint8_t, std::unique_ptr<const google::protobuf::MessageLite>>;

    mosquitto* mMosq = nullptr;
    std::string mHost;
    uint16_t mPort;
    crypto::bytes mPublicKey;
    crypto::bytes mPrivateKey;
    MockResponseMap mMockResponses;
    std::string mLoggedClientId;

    static void onMessageCallback(mosquitto*, void* self, const mosquitto_message* message) { static_cast<MockMqttMobilusActorImpl*>(self)->onMessage(message); }
    [[nodiscard]] static crypto::bytes randomKey();
    [[nodiscard]] static Envelope envelopeFor(const google::protobuf::MessageLite& message);

    void send(const std::string& topic, const google::protobuf::MessageLite& message, const crypto::bytes& key);
    void onMessage(const mosquitto_message* message);
    void handleLoginRequest(const Envelope& envelope);
    void handleMockResponse(const Envelope& envelope);
};

}
