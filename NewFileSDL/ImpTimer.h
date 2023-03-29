#ifndef IMP_TIMER_H
#define IMP_TIMER_H

class ImpTimer
{
    public:
        ImpTimer();
        ~ImpTimer();

        void start();
        void stop();
        void paused();
        void unpaused();
        int get_ticks();
        bool is_started();
        bool is_paused();

    private:
        //Thời điểm bắt đầu chạy
        int start_tick_;
        //Thời điểm dừng
        int paused_tick_;
        bool is_paused_;
        bool is_started_;

};



#endif