#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include "Device.hpp"
#include <memory>
#include <cmath>

class Simulator{
    private:
        std::priority_queue<Event,std::vector<Event>,std::greater<Event>> event_queue;
        std::vector<std::unique_ptr<Device>> device_list;

    public:
        double current_time;

        
        Simulator(){
            current_time = 0.0;
        }
    
        void add_device(std::unique_ptr<Device> x) {
            device_list.push_back(std::move(x));
        }
        

        void scheduleEvent(const Event& a){
            event_queue.push(a);
        }
    
        void pull_queue(std::vector<Event> a){
            for(auto& c:a){
                scheduleEvent(c);
            }
        }
        void Event_run(Event e){
            if(e.target_id == 100){
                for(auto& c:device_list){
                    if(c->isScanning()){
                        auto results = c->handleEvent(e);
                        pull_queue(results);
                    }
                }
            }
            else{
                for(auto& c:device_list){
                    if(c->device_id == e.target_id){
                        auto results = c->handleEvent(e);
                        pull_queue(results);
                        break;
                    }
                }
            }
        }

        Device& get_device(int a){
            for(auto &c:device_list){
                if (c->get_id() == a){
                    return *c;
                }
            }
            throw std::runtime_error("Device not found: " + std::to_string(a));

        }

        double get_dist(Packet& a){
            location a1 = get_device(a.get_source_id()).get_place();
            location a2 = get_device(a.get_dest_id()).get_place();
            double sum = ((a1.x-a2.x)*(a1.x-a2.x)) + ((a1.y-a2.y)*(a1.y-a2.y));
            return sqrt(sum);
        }

        void run(){
            while(event_queue.size()>0){
                Event process=event_queue.top();
                event_queue.pop();
                current_time=process.time;
                Event_run(process);
                if(current_time>2.0){
                    break;
                }
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