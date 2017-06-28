#pragma once

template <class T>
class StormIntrusivePtr {
    T* _Ptr;
public:
    typedef T element_type;

    StormIntrusivePtr(): _Ptr(0) { }
    StormIntrusivePtr(const StormIntrusivePtr& s): _Ptr(s._Ptr) {
        if (s._Ptr) {
            StormIntrusivePtr_add_ref(s._Ptr);
        }
    }

    template<class U>
    StormIntrusivePtr(StormIntrusivePtr<U> const& rhs) : _Ptr(rhs.get()) {
        if (_Ptr != 0) {
            StormIntrusivePtr_add_ref(_Ptr);
        }
    }

    ~StormIntrusivePtr() {
        if (_Ptr) {
            StormIntrusivePtr_release(_Ptr);
        }
    }

    T* get() const {
        return _Ptr;
    }

    T& operator*() const {
        //OX_ASSERT(_Ptr && _Ptr->_ref_counter > 0);
        return *_Ptr;
    }

    T* operator->() const {
        //OX_ASSERT(_Ptr && _Ptr->_ref_counter > 0);
        return _Ptr;
    }

    StormIntrusivePtr& operator = (const StormIntrusivePtr& s) {
        StormIntrusivePtr(s).swap(*this);
        return *this;
    }

    StormIntrusivePtr& operator = (T* ptr) {
        StormIntrusivePtr(ptr).swap(*this);
        return *this;
    }

    StormIntrusivePtr(T* p): _Ptr(p) {
        if (p) {
            StormIntrusivePtr_add_ref(p);
        }
    }

    bool operator!() const {
        return _Ptr == 0;
    }

    void swap(StormIntrusivePtr& s) {
        T* p = s._Ptr;
        s._Ptr = _Ptr;
        _Ptr = p;
    }

    operator bool () const {
        return _Ptr != 0;
    }
};

template<class T, class U> inline bool operator==(StormIntrusivePtr<T> const& a, StormIntrusivePtr<U> const& b) {
    return a.get() == b.get();
}

template<class T, class U> inline bool operator!=(StormIntrusivePtr<T> const& a, StormIntrusivePtr<U> const& b) {
    return a.get() != b.get();
}

template<class T> inline bool operator==(StormIntrusivePtr<T> const& a, T* b) {
    return a.get() == b;
}

template<class T> inline bool operator!=(StormIntrusivePtr<T> const& a, T* b) {
    return a.get() != b;
}

template<class T> inline bool operator==(T* a, StormIntrusivePtr<T> const& b) {
    return a == b.get();
}

template<class T> inline bool operator!=(T* a, StormIntrusivePtr<T> const& b) {
    return a != b.get();
}

template<class T> T* get_pointer(StormIntrusivePtr<T> const& p) {
    return p.get();
}

template<class T, class U> StormIntrusivePtr<T> static_pointer_cast(StormIntrusivePtr<U> const& p) {
    return static_cast<T*>(p.get());
}

template<class T, class U> StormIntrusivePtr<T> const_pointer_cast(StormIntrusivePtr<U> const& p) {
    return const_cast<T*>(p.get());
}

template<class T, class U> StormIntrusivePtr<T> dynamic_pointer_cast(StormIntrusivePtr<U> const& p) {
    return dynamic_cast<T*>(p.get());
}

/*
template <class T>
class RefHolder : public T {
public:
    RefHolder() {
        this->_ref_counter = 1000000;
    }

    void addRef() { }
    void releaseRef() {}
};
*/