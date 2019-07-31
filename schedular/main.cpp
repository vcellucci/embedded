/*
This uses systick to schedule tasks.  It cycles through red, gree, blue led on Tiva C

It's using CCS startup code with the vector table
modified to use SystickHandler

*/

#include <stdint.h>
#include <functional>
#include "bsp.hpp"
#include "singleton.hpp"
#include "schedular.hpp"

using led_schedular = round_robin_schedular_t<3>;
using the_led_schedular = singleton_t<led_schedular>;

extern "C"
void SystickHandler() {
    the_led_schedular::instance().switch_to_next_task();
}

int main(){
    gpiof_regmap::set_to_led_output_mode();

    gpiof_regmap::reset_leds();

    the_led_schedular::instance().queue_task([]{
        gpiof_regmap::reset_leds();
        gpiof_regmap::red::set();
    });

    the_led_schedular::instance().queue_task([]{
        gpiof_regmap::reset_leds();
        gpiof_regmap::green::set();
    });

    the_led_schedular::instance().queue_task([]{
        gpiof_regmap::reset_leds();
        gpiof_regmap::blue::set();

    });

    the_led_schedular::instance().start();

    systick_regmap::load_reg::value() = SYSCLOCK_HZ / 2U - 1U;
    systick_regmap::val_reg::value() = 0U;
    systick_regmap::ctrl_reg::value() = (1U << 2) | (1U << 1) | 1U;

    while(1){
        // do nothing
    }

    return 0;
}