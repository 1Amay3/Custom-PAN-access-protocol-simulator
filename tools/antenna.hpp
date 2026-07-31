#pragma once


class Antenna{
    private:
        double Power;
        double Gain;
    public:
        Antenna(double power = 20,double gain = 30):Power(power),Gain(gain){};
        void show_params(){};
        ~Antenna(){};
    };