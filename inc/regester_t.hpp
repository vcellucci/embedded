#ifndef __regester_t_hpp__
#define __regester_t_hpp__

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


#endif