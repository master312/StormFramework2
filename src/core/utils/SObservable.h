#pragma once
#include <functional>
#include <map>
#include <vector>
#include "../StormCommon.h"

/* Observable class. Used to implement observer patern. 
 * T_TYPES - is an enum of notification types
 * T_OBJECT - is class type of observer objects */

/* Macros for easy observer access */
#define S_OBSERVER_ADD(observable, object, type, callback) observable->registerObserver(type, object, std::bind(&callback, object, std::placeholders::_1));
#define S_OBSERVER_REMOVE(observable, object, type) observable->removeObserver(type, object);
#define S_OBSERVER_REMOVE_ALL(observable, object) observable->removeObserver(object);

template <typename...> class SObservable;

template<typename T_TYPES, typename T_OBJECT>
class SObservable<T_TYPES, T_OBJECT> {
private:
    struct SObserver;

public:
    SObservable() { }
    virtual ~SObservable() {
        for (auto& observers : _Observers) {
            observers.second.clear();
        }
        _Observers.clear();
    }

    /* Registers observer object
     * Use macro #S_OBSERVER_ADD */
    void registerObserver(T_TYPES type, T_OBJECT* object, std::function<void(void*)> callback) {
        if (!object || !callback) {
            LOG(ERROR) << "Tryed to register observer with invalid argumets";
            return;
        }
        _Observers[type].push_back(SObserver(object, type, callback));
    }

    /* Removes observer of @type in @object
     * Use macro #S_OBSERVER_REMOVE */
    void removeObserver(T_TYPES type, T_OBJECT* object) {
        auto iter = _Observers.find(type);
        if (iter == _Observers.end()) {
            return;
        }
        removeObjectFromVector(iter->second, object);
    }

    /* Removes observers of all types by @object
     * Use macro #S_OBSERVER_REMOVE_ALL */
    void removeObserver(T_OBJECT* object) {
        for (auto& iter : _Observers) {
            removeObjectFromVector(iter.second, object);
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
                LOG(ERROR) << "Invalid observer object found in SObservable::notifyObservers";
            }
        }
    }

private:
    /* This struct holds info abound single observer */
    struct SObserver {
        /* Observer object that is listening for event */
        T_OBJECT* object;
        /* Type of notification for witch this observers listens */
        T_TYPES type;
        /* Notification callback method */
        std::function<void(void*)> callback;

        SObserver(T_OBJECT* _object, T_TYPES _type, std::function<void(void*)> _callback) 
                    : object(_object), type(_type), callback(_callback) { }
    };

    /* Map of all registered observers, indexed by type */
    std::map<T_TYPES, std::vector<SObserver> > _Observers;

    void removeObjectFromVector(std::vector<SObserver>& vec, T_OBJECT* object) {
        for (size_t i = 0; i < vec.size(); i++) {
            if (vec[i].object == object) {
                vec.erase(vec.begin() + i);
                return;
            }
        }
    }
};
