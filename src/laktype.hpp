#include <memory>
using std::shared_ptr;
using std::make_shared;
#include <type_traits>
using std::remove_reference;
#include <exception>
using std::exception;

#ifndef LAKTYPE_H
#define LAKTYPE_H

class lak_type_exception : public exception { using exception::exception; };

template<typename T>
void typeof(){}

struct lakType
{
    void (*type)();
    shared_ptr<void> value;
    lakType& operator=(const lakType& other)
    {
        type = other.type;
        value = other.value;
        return *this;
    }
    lakType& operator=(lakType&& other)
    {
        type = other.type;
        value = other.value;
        return *this;
    }
    template<typename T>
    lakType& operator=(const T& t)
    {
        type = typeof<remove_reference<T>::type>;
        value = make_shared<remove_reference<T>::type>();
        remove_reference<T>::type& ot = *(remove_reference<T>::type*)value.get();
        ot = t;
        return *this;
    }
    template<typename T>
    lakType& operator=(T&& t)
    {
        type = typeof<remove_reference<T>::type>;
        value = make_shared<remove_reference<T>::type>();
        remove_reference<T>::type& ot = *(remove_reference<T>::type*)value.get();
        ot = t;
        return *this;
    }
    template<typename T>
    T get()
    {
        return *(remove_reference<T>::type*)value.get();
    }
    template<typename T>
    T getStrict()
    {
        if (type == typeof<remove_reference<T>::type>) return get<T>();
        else throw lak_type_exception();
    }
};

#endif