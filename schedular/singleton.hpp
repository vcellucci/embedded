/*
 * singleton.hpp
 *
 *  Created on: Jul 30, 2019
 *      Author: vel
 */

#ifndef SINGLETON_HPP_
#define SINGLETON_HPP_

template<class T>
class singleton_t {
protected:
    static T instance_t;
public:
    static T& instance() {
        return instance_t;
    }
};

template<class T>
T singleton_t<T>::instance_t;



#endif /* SINGLETON_HPP_ */
