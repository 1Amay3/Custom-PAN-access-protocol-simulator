#pragma once
#include "Packet.hpp"
#include "Event.hpp"
#include <vector>


class Simulator;

class Device{
    protected:
    Simulator* sim;
    public:
        int device_id;
        int bw_floor;
        int bw_ceil;
        std::string config;        
        Device(int id,int low, int high):bw_ceil(high),bw_floor(low),device_id(id){};
        virtual ~Device() = default;
        virtual void handleEvent(const Event&){};
        virtual bool isScanning()const{
            return false;
        }
};

enum dev_state{
    NODE_OFF,
    SCANNING,
    CONNECTED
}; 

class Node : public Device{
    public:
        dev_state state = NODE_OFF;
        int master_id = -1;
        std::vector<Packet> to_send;
        std::vector<Packet> to_process;
        Node(int id,int low,int high):
            Device(id,low,high){};
        bool isScanning()const  override{
            return state == SCANNING;
        }

        void handleEvent(const Event& a)override{
            switch(a.type){
                case JOIN_REQUEST:{
                    Packet p(device_id,master_id,3,CONN_REQ,config,a.time);
            }
        }
        }


};


enum master_state{
    MASTER_OFF,
    BEACONING,
    FULL
};
class Master : public Device{
    public:
        master_state state = BEACONING;
        int max_limit = 7;
        std::vector<Packet> buffer;   
        std::vector<Node*>network;
        Master(int id, int low, int high):
            Device(id,low,high){};
        void handleEvent(const Event& a)override{
            switch(a.type){
                case BEACON_TX:{
                    double beacon_delay = 0.002;
                    Packet p(device_id,-1,3,BEACON,config,a.time+beacon_delay);
                    sim->broadcastBeacon();
            }

        }
        }
        
};