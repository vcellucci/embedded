/*
 * register_t.hpp
 *
 *  Created on: Jul 29, 2019
 *      Author: Vittorio Cellucci
 */

#ifndef REGISTER_T_HPP_
#define REGISTER_T_HPP_


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


#endif /* REGISTER_T_HPP_ */
