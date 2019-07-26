
namespace gpiof {
    static constexpr unsigned int direction     = 0x40025400U;
    static constexpr unsigned int clock_gate    = 0x400FE608U;
    static constexpr unsigned int digital_mode  = 0x4002551CU;
    static constexpr unsigned int data          = 0x400253FCU;
};

using uint32_t = unsigned int;
using int32_t  = int;

template<typename address_type, address_type address, address_type val>
class register_t {
public:
    static void set() {
        *reinterpret_cast<volatile address_type*>(address) = val;
    }
};

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


int main(){

    register_t<uint32_t, gpiof::clock_gate, 0x20U>::set(); // enable gpiof
    register_t<uint32_t, gpiof::direction, 0x0EU>::set();  // set all to output
    register_t<uint32_t, gpiof::digital_mode, 0x0EU>::set(); // set all to digital
    using blue_led = register_t<uint32_t, gpiof::data, 0x4U>;
    using red_led  = register_t<uint32_t, gpiof::data, 0x2U>;
    using busy     = busy_wait_t<int32_t, 1000000>;


    while(1){
        blue_led::set();
        busy::wait();

        red_led::set();
        busy::wait();
    }

    return 0;
}
