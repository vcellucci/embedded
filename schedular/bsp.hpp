/*
 * bsp.hpp
 *
 *  Created on: Jul 29, 2019
 *      Author: Vittorio Cellucci
 */

#ifndef BSP_HPP_
#define BSP_HPP_

#include "register_t.hpp"

static constexpr uint32_t SYSCLOCK_HZ = 16000000U;

namespace gpiof {
    static constexpr uint32_t clock_gate    = 0x400FE608U;
    static constexpr uint32_t base          = 0x40025000U;
    static constexpr uint32_t red_led       = 0x02U;
    static constexpr uint32_t blue_led      = 0x04U;
    static constexpr uint32_t green_led     = 0x08U;
};

namespace systick {
    static constexpr uint32_t base  = 0xE000E000;
    static constexpr uint32_t ctrl  = 0x010;
    static constexpr uint32_t load  = 0x014;
    static constexpr uint32_t val   = 0x018;
}


template<uint32_t base_addr>
class gpiof_regmap_t {
public:
    using data_reg      = register_t<uint32_t, base_addr>;
    using direction_reg = register_t<uint32_t, base_addr + 0x400U>;
    using mode_reg      = register_t<uint32_t, base_addr + 0x51CU>;
    using red      = register_t<uint32_t, gpiof::base, gpiof::red_led>;
    using blue     = register_t<uint32_t, gpiof::base, gpiof::blue_led>;
    using green    = register_t<uint32_t, gpiof::base, gpiof::green_led>;

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

using gpiof_regmap = gpiof_regmap_t<gpiof::base>;

template<uint32_t base_addr>
class systick_t {
public:
    using ctrl_reg = register_t<uint32_t, base_addr + systick::ctrl>;
    using load_reg = register_t<uint32_t, base_addr + systick::load>;
    using val_reg  = register_t<uint32_t, base_addr + systick::val>;

};

using systick_regmap = systick_t<systick::base>;


#endif /* BSP_HPP_ */
