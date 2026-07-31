#pragma once
#include "Packet.hpp"
#include "Event.hpp"
#include <vector>
#include "../tools/antenna.hpp"
#include <random>

struct location{
    int x;
    int y;
};

class Device{
    protected:
    public:
        int device_id;
        int bw_floor;
        int bw_ceil;
        std::string config;
        Antenna antenna;
        location coordinate;

        Device(int id,int low, int high):bw_ceil(high),bw_floor(low),device_id(id){
            set_place();
        };
        Device(int id,int low, int high,std::string a):bw_ceil(high),bw_floor(low),device_id(id),config(a){
            set_place();};
        Device(int id,int low, int high,std::string a,Antenna ant):bw_ceil(high),bw_floor(low),device_id(id),config(a),antenna(ant){
            set_place();
        };
        virtual ~Device() = default;
        virtual std::vector<Event> handleEvent(const Event&){
            return {};
        };

        void set_place();

        virtual bool isScanning()const{
            return false;
        }
        location get_place()const{
            return coordinate;
        }
        int get_id(){
            return device_id;
        }
};

enum dev_state{
    NODE_OFF,
    SCANNING,
    CONNECTED
}; 

class Node : public Device{
    public:
        dev_state state = SCANNING;
        int master_id = -1;
        std::vector<Packet> to_send;
        std::vector<Packet> to_process;
        Node(int id,int low,int high):
            Device(id,low,high){};
        bool isScanning()const  override{
            return state == SCANNING;
        }

        std::vector<Event> handleEvent(const Event& a)override;
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
        std::vector<int>network;
        Master(int id, int low, int high,std::string a):Device(id,low,high,a){};

        void set_config(std::string abc){
            config = abc;
        }

        std::vector<Event> handleEvent(const Event& a)override;
        
};