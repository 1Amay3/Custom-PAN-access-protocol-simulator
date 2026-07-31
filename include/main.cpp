#include <iostream>
#include "simulator.hpp"

int main(){
    Simulator sim;
    sim.add_device(std::make_unique<Node>(4,2000,10000));
    sim.add_device(std::make_unique<Node>(8,2000,10000));
    sim.add_device(std::make_unique<Node>(12,2000,10000));
    sim.add_device(std::make_unique<Node>(16,2000,10000));

    sim.add_device(std::make_unique<Master>(1,2000,10000,"Hello"));
    Packet p(1,100,3,BEACON,"Hello",sim.current_time);
    Event origin(0.0,BEACON_TX,1,1,p);
    sim.pull_queue({origin});
    sim.run();
    return 0;
}