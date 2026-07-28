#pragma once
#include "Packet.hpp"
#include "Event.hpp"
#include <vector>
#include <iostream>


class Device{
    protected:
    public:
        int device_id;
        int bw_floor;
        int bw_ceil;
        std::string config;        
        Device(int id,int low, int high):bw_ceil(high),bw_floor(low),device_id(id){};
        virtual ~Device() = default;
        virtual std::vector<Event> handleEvent(const Event&){
            return {};
        };
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

        std::vector<Event> handleEvent(const Event& a)override{
            switch(a.type){
                case BEACON_RX:{
                    if (state !=SCANNING){
                        return {};
                    }
                    else{
                    Packet p_out = a.get_packet();
                    config = p_out.get_frame();
                    Packet p(device_id,p_out.get_source_id(),3,CONN_REQ,config,a.time);
                    Event e(a.time,JOIN_REQUEST,device_id,master_id,p);
                    return {e};
                    }
                }
                case JOIN_RESPONSE:{
                    if (state!=SCANNING){
                        return {};
                    }
                    Packet p_out =a.get_packet();
                    if(p_out.get_frame()=="Approved"){
                        master_id = p_out.get_source_id();
                        state = CONNECTED;
                        
                    }
                    else{
                        state = SCANNING;
                    }
                    return {};
                }
                default:{
                    std::cout<<a.type<<" Event unidentifiable.\n";
                    return {};
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
        std::vector<int>network;
        Master(int id, int low, int high):
            Device(id,low,high){};
        std::vector<Event> handleEvent(const Event& a)override{
            switch(a.type){
                case BEACON_TX:{
                    double beacon_delay = 0.002;
                    Packet p(device_id,-1,3,BEACON,config,a.time+beacon_delay);
                    Event e((a.time+beacon_delay),BEACON_RX,device_id,100,p);
                    return {e};
                }
                case JOIN_REQUEST:{
                    double resp_delay = 0.002;
                    Packet check = a.get_packet();
                    if(check.get_dest_id()==device_id && check.get_frame() == config){
                        network.push_back(check.get_source_id());
                        Packet resp(device_id,check.get_source_id(),3,CONN_RESP,"Approved",a.time+0.002);
                        Event e((a.time+0.002),JOIN_RESPONSE,device_id,check.get_source_id(),resp);
                        return {e};
                    }
                    Packet reject(device_id, check.get_source_id(), 3, CONN_RESP, "Rejected", a.time + 0.002);
                    Event e(a.time + 0.002, JOIN_RESPONSE, device_id, check.get_source_id(), reject);
                    return {e};
                }
                default:{
                    std::cout<<a.type<<" Event unidentifiable.\n";
                    return {};
                }

        }
        }
        
};