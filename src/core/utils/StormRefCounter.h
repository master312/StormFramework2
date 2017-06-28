#pragma once
#include "StormIntrusivePtr.h"

class StormRefCounter {
public:
    int _RefCounter;

    StormRefCounter(): _RefCounter(0) { }
    virtual ~StormRefCounter() { }

    void addRef() {
        ++_RefCounter;
    }
    
    void releaseRef() {
        if (0 == --_RefCounter)
            delete this;
    }
private:
    StormRefCounter(const StormRefCounter&);
    const StormRefCounter& operator=(const StormRefCounter&);
};


inline void StormIntrusivePtr_add_ref(StormRefCounter* p) {
    p->addRef();
}

inline void StormIntrusivePtr_release(StormRefCounter* p) {
    p->releaseRef();
}