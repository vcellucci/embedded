
using uint32_t = unsigned int;
using int32_t  = int;


namespace gpiof {
    static constexpr uint32_t direction     = 0x40025400U;
    static constexpr uint32_t clock_gate    = 0x400FE608U;
    static constexpr uint32_t digital_mode  = 0x4002551CU;
    static constexpr uint32_t data          = 0x40025000U;
    static constexpr uint32_t red_led       = 0x02U;
    static constexpr uint32_t blue_led      = 0x04U;
    static constexpr uint32_t green_led     = 0x08U;

};

template<typename address_type,
        address_type address,
        address_type val>
class register_t {
public:
    static void write() {
        *reinterpret_cast<volatile address_type*>(address) = val;
    }

    static void set() {
        // The two here, is because the MCU does not use the first 2 bits, must be a factor of 4
        (*reinterpret_cast<volatile address_type*>(address + (val << 2))) = val;
    }

    static void zero() {
        // The two here, is because the MCU does not use the first 2 bits, must be a factor of 4
        (*reinterpret_cast<volatile address_type*>(address + (val << 2))) = 0;
    }

    // returns value for direct access
    static address_type& value() {
        return *reinterpret_cast<volatile address_type*>(address);
    }

    // returns value for read only
    static const address_type& const_value() {
        return *reinterpret_cast<volatile address_type*>(address);
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

    register_t<uint32_t, gpiof::clock_gate, 0x20U>::write(); // enable gpiof
    register_t<uint32_t, gpiof::direction, 0x0EU>::write();  // set all to output
    register_t<uint32_t, gpiof::digital_mode, 0x0EU>::write(); // set all to digital

    using busy     = busy_wait_t<int32_t, 1000000>;
    using red      = register_t<uint32_t, gpiof::data, gpiof::red_led>;
    using blue     = register_t<uint32_t, gpiof::data, gpiof::blue_led>;

    while(1){

      red::zero();
      blue::set();
      busy::wait();

      blue::zero();
      red::set();
      busy::wait();

    }

    return 0;
}
