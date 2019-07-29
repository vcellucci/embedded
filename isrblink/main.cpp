/*
This uses systick isr to toggle blue, red led every second.   It's using CCS startup code with the vector table
modified to use SystickHandler
*/

#include <stdint.h>
#include "bsp.hpp"

extern "C"
void SystickHandler() {
    gpiof_regmap::blue::toggle();
    gpiof_regmap::red::toggle();
}

int main(){
    gpiof_regmap::set_to_led_output_mode();

    gpiof_regmap::reset_leds();
    gpiof_regmap::red::set();

    systick_regmap::load_reg::value() = SYSCLOCK_HZ / 2U - 1U;
    systick_regmap::val_reg::value() = 0U;
    systick_regmap::ctrl_reg::value() = (1U << 2) | (1U << 1) | 1U;


    while(1){
        // do nothing
    }

    return 0;
}
