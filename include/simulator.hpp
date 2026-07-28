#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include "Device.hpp"
#include <memory>

class Simulator{
    private:
        std::priority_queue<Event,std::vector<Event>,std::greater<Event>> event_queue;
        std::vector<std::unique_ptr<Device>> device_list;

    public:
        double current_time;

        
        Simulator(){
            current_time = 0.0;
        }
        void add_device(std::unique_ptr<Device> x){
            device_list.push_back(std::move(x));
        }

        void add_delay(double x){
            current_time+=x;
        }
        void scheduleEvent(const Event& a){
            event_queue.push(a);
        }
        void pull_queue(std::vector<Event> a){
            for(auto& c:a){
                scheduleEvent(c);
            }
        }






        /*void broadcastBeacon(Packet p){
            Event e(current_time,BEACON_TX);
            e.type = BEACON_TX;
            e.time = current_time;
            e.source_id = p.get_source_id();
            e.packet_processed = p;
            for(auto& c:device_list){
                if(c->isScanning()){
                    c->handleEvent(e); 
                }
            }
        }
*/

};