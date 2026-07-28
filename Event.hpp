#pragma once
#include <optional>
#include "Packet.hpp"

enum event_type{
    BEACON_TX,
    JOIN_REQUEST,
    JOIN_RESPONSE,
    DATA_UPLOAD,
    DATA_OFFLOAD
};


class Event{
    public:
        double time;
        event_type type;
        int source_id;
        int target_id;
        std::optional<Packet> packet_processed;  
        bool operator<(const Event&b)const {
            return time < b.time;
        }
        bool operator>(const Event&b)const {
            return time > b.time;
        } 

};


