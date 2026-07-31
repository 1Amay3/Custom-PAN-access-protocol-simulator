#include "Device.hpp"
#include <iostream>

void Device::set_place(){
    static std::mt19937_64 engine(std::random_device{}());
    std::uniform_int_distribution<int> place(-200, 200);
    coordinate.x = place(engine);
    coordinate.y = place(engine);
}

std::vector<Event> Node::handleEvent(const Event& a){
            switch(a.type){
                case BEACON_RX:{
                    if (state !=SCANNING){
                        return {};
                    }
                    else{
                    Packet p_out = a.get_packet();
                    config = p_out.get_frame();
                    Packet p(device_id,p_out.get_source_id(),3,CONN_REQ,config,a.time+0.002);
                    Event e(a.time+0.002,JOIN_REQUEST,device_id,p_out.get_source_id(),p);
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
                        std::cout << "Node " << device_id << " connected to master " << master_id << "\n";
                        
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

std::vector<Event> Master::handleEvent(const Event& a){
            switch(a.type){
                case BEACON_TX:{
                    if(state != BEACONING){
                        return {};
                    }

                    double beacon_delay = 0.002;
                    Packet p(device_id,-1,3,BEACON,config,a.time+beacon_delay);
                    Event e((a.time+beacon_delay),BEACON_RX,device_id,100,p);
                    Event b_next(a.time+1,BEACON_TX,device_id,device_id);
                    std::cout << "Beaconing at time " << a.time << "\n";
                    return {e,b_next};
                    
                }
                case JOIN_REQUEST:{
                    double resp_delay = 0.002;
                    Packet check = a.get_packet();
                    if(check.get_dest_id()==device_id && check.get_frame() == config && network.size()<7){
                        network.push_back(check.get_source_id());
                        Packet resp(device_id,check.get_source_id(),3,CONN_RESP,"Approved",a.time+0.002);
                        Event e((a.time+0.002),JOIN_RESPONSE,device_id,check.get_source_id(),resp);
                        return {e};
                    }
                    if(network.size()==7){
                        state = FULL;
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
        


