#pragma once

#include <cstdlib>

namespace octo
{
    template <class T>
    class Singleton
    {
        static T *_instance;

        Singleton(Singleton const &);
        Singleton &operator=(Singleton const &);

    protected:
        Singleton();
        ~Singleton();

    public:
        static T *getPtr()
        {
            return _instance ? _instance : (_instance = new T());
        }

        static T getRef()
        {
            if (!_instance)
                _instance = new T();
            return *_instance;
        }
    };

    template <class T>
    T *Singleton<T>::_instance = NULL;
}