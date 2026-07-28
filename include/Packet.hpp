#pragma once
#include <string>

enum packet_type{
        BEACON,
        CONN_REQ,
        CONN_RESP,
        DATA_TX
    };

class Packet
{
private:
    packet_type p_type;
    int source_id;
    int dest_id;
    int retry_ct;
    std::string payload;
    double timestamp;
public:
    Packet(int src = -1, int dst = -1, int rt = 0, packet_type ptype = DATA_TX,std::string s = "",double time_s = -1):source_id(src),dest_id(dst),retry_ct(rt),payload(s),p_type(ptype),timestamp(time_s){};
    ~Packet(){};
    //getters:
    int get_source_id()const{
        return source_id;
    }
    int get_dest_id() const{
        return dest_id;
    }
    int get_retry_ct() const{
        return retry_ct;
    }
    double get_time() const{
        return timestamp;
    }
    std::string get_frame() const{
        return payload;
    }
    packet_type get_p_type() const{
        return p_type;
    }


};



