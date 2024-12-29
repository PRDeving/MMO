#pragma once

namespace Networking::Messages {
    typedef char Token[10];

    enum MESSAGE_TYPE {
        NONE,
        HANDSHAKE,
    };

    #pragma pack(push, 1)
    typedef struct {
        MESSAGE_TYPE type = MESSAGE_TYPE::HANDSHAKE;
        Token token;
    } Handshake;
    #pragma pack(pop)

}
