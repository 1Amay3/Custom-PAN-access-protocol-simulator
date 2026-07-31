#pragma once
#include <optional>
#include "Packet.hpp"

enum event_type{
    BEACON_TX,
    BEACON_RX,
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
        bool check_packet(){
            if(packet_processed){
                return true;
            }
            else{
                return false;
            }
        }
        const Packet& get_packet() const{
            return packet_processed.value();
        }


        Event(double t, event_type ty,int src, int tar,Packet p):time(t),type(ty),source_id(src),target_id(tar),packet_processed(p){}
        Event(double t, event_type ty,int src, int tar):time(t),type(ty),source_id(src),target_id(tar){}
        //Have to update: target == 100 -> broadcast to Nodes.
        ~Event(){};

};


