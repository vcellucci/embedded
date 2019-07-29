
/*
This uses systick isr to toggle blue, red led every second.   It's using CCS startup code with the vector table
modified to use SystickHandler

*/

#include <stdint.h>

static constexpr uint32_t SYSCLOCK_HZ = 16000000U;

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

namespace systick {
    static constexpr uint32_t base  = 0xE000E000;
    static constexpr uint32_t ctrl  = 0x010;
    static constexpr uint32_t load  = 0x014;
    static constexpr uint32_t val   = 0x018;
}

template<uint32_t base_addr>
class systick_t {
public:
    using ctrl_reg = register_t<uint32_t, base_addr + systick::ctrl>;
    using load_reg = register_t<uint32_t, base_addr + systick::load>;
    using val_reg  = register_t<uint32_t, base_addr + systick::val>;

};

using systick_regmap = systick_t<systick::base>;

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


extern "C"
void SystickHandler() {
    gpiof_regmap::blue::toggle();
    gpiof_regmap::red::toggle();
}


int main(){
    gpiof_regmap::set_to_led_output_mode();
    gpiof_regmap::reset_leds();
    gpiof_regmap::red::set();

    // use systick
    systick_regmap::load_reg::value() = SYSCLOCK_HZ / 2U - 1U; 
    systick_regmap::val_reg::value() = 0U;
    systick_regmap::ctrl_reg::value() = (1U << 2) | (1U << 1) | 1U; // enable 


    while(1){}


    return 0;
}
