
// just trying to see what the compiler does here... using modern c++ generates same code as c!


using uint32_t = unsigned int;
using int32_t  = int;


namespace gpiof {
    static constexpr uint32_t clock_gate    = 0x400FE608U;
    static constexpr uint32_t base          = 0x40025000U;
    static constexpr uint32_t red_led       = 0x02U;
    static constexpr uint32_t blue_led      = 0x04U;
    static constexpr uint32_t green_led     = 0x08U;
};

template<typename address_type,
        address_type address,
        address_type val = 0>
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
    static volatile address_type& value() {
        return *reinterpret_cast<volatile address_type*>(address);
    }

    // returns value for read only
    static const address_type& const_value() {
        return *reinterpret_cast<volatile address_type*>(address);
    }

    static void toggle() {
        (*reinterpret_cast<volatile address_type*>(address + (val << 2))) ^= val;
    }

};

template<uint32_t base_addr>
class gpio_t {
public:
    using data_reg      = register_t<uint32_t, base_addr>;
    using direction_reg = register_t<uint32_t, base_addr + 0x400U>;
    using mode_reg      = register_t<uint32_t, base_addr + 0x51CU>;

};

class gpiof_regmap : public gpio_t<gpiof::base> {
public:
    using red      = register_t<uint32_t, gpiof::base, gpiof::red_led>;
    using blue     = register_t<uint32_t, gpiof::base, gpiof::blue_led>;
    using green    = register_t<uint32_t, gpiof::base, gpiof::green_led>;

public:

    static void set_to_led_output_mode() {
       register_t<uint32_t, gpiof::clock_gate, 0x20U>::write(); // enable gpiof
       direction_reg::value() = 0x0EU; // set to output
       mode_reg::value() = 0x0EU; // set to digital mode
    }

    static void reset_leds() {
        red::zero();
        green::zero();
        blue::zero();
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

    gpiof_regmap::set_to_led_output_mode();

    using busy     = busy_wait_t<int32_t, 1000000>;
    gpiof_regmap::reset_leds();
    gpiof_regmap::red::set();

    while(1){

      gpiof_regmap::red::toggle();
      gpiof_regmap::blue::toggle();

      busy::wait();


    }

    return 0;
}
