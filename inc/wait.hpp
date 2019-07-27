#ifndef __wait__hpp__
#define __wait__hpp__


template<typename counter_type, counter_type value>
class busy_wait_t{
public:
    static void wait() {
        volatile counter_type counter = 0;
        while(counter < value){
            counter++;
        }
    }
};

#endif