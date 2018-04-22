#pragma once
#include <functional>
#include <map>
#include <vector>
#include "../StormCommon.h"

/* Observable class. Used to implement observer pattern.
 * T_TYPES - is an enum of notification types */

template <typename...> class SNotificationCenter;

template<typename T_TYPES>
class SNotificationCenter<T_TYPES> {
private:
    struct SObserver;

public:
    SNotificationCenter() { }
    virtual ~SNotificationCenter() {
        for (auto& observers : _Observers) {
            observers.second.clear();
        }
        _Observers.clear();
    }

    /* Registers observer object */
    template<class T>
    void registerObserver(T_TYPES type, void (T::*fun)(void*), T* object) {
        if (!object || !fun) {
            LOG(ERROR) << "Tried to register observer with invalid arguments";
            return;
        }

        _Observers[type].push_back(SObserver(object, type, std::bind(fun, object, std::placeholders::_1)));
    }

    /* Removes observers of all types by @object */
    template<class T>
    void removeObservers(T* object) {
        for (auto& iter : _Observers) {
            removeObjectFromVector(iter.second, static_cast<void*>(object));
        }
    }

    /* Notify all observers that are listening for notifications of type @type */
    void notifyObservers(T_TYPES type, void* data = nullptr) {
        auto iter = _Observers.find(type);
        if (iter == _Observers.end()) {
            return;
        }
        for (SObserver& observer : iter->second) {
            if (observer.object && observer.callback) {
                observer.callback(data);
            } else {
                LOG(ERROR) << "Invalid observer object found in SNotificationCenter::notifyObservers";
            }
        }
    }

private:
    /* This struct holds info abound single observer */
    struct SObserver {
        /* Observer object that is listening for event */
        void* object;
        /* Type of notification for witch this observers listens */
        T_TYPES type;
        /* Notification callback method */
        std::function<void(void*)> callback;

        SObserver(void* _object, T_TYPES _type, std::function<void(void*)> _callback)
                    : object(_object), type(_type), callback(_callback) { }
    };

    /* Map of all registered observers, indexed by type */
    std::map<T_TYPES, std::vector<SObserver> > _Observers;

    void removeObjectFromVector(std::vector<SObserver>& vec, void* object) {
        for (size_t i = 0; i < vec.size(); i++) {
            if (vec[i].object == object) {
                vec.erase(vec.begin() + i);
                return;
            }
        }
    }
};
