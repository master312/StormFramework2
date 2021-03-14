#pragma once
#include <functional>
#include <map>
#include <vector>
#include "../StormCommon.h"

/* Event notification system. Uses observer pattern */

/* Macro used to generate event type IDs */
#define S_GENERATE_EVENT_ID(b,c,d) makefourcc(0xA,b,c,d)

namespace SEventDispatcher {

    struct Event {
        int32_t typeId;

        Event() : typeId(0) { }
        Event(int32_t _typeId) : typeId(_typeId) { }
    };

    typedef std::function<void(const Event*)> CallbackFunction;

    struct CallbackInfo {
        CallbackFunction function;
        void* object;

        CallbackInfo() : function(nullptr), object(nullptr) { }
        CallbackInfo(CallbackFunction fun, void* obj) : function(fun), object(obj) { }
    };

    class Dispatcher {
    public:
        Dispatcher() { }
        ~Dispatcher() { }

        void fireEvent(Event* event) {
            auto iter = _Callbacks.find(event->typeId);
            if (iter == _Callbacks.end()) {
                return;
            }
            for (CallbackInfo& info : iter->second) {
                info.function(event);
            }
        }

        template <class T>
        void removeListeners(T* object) {
            void* castedObject = static_cast<void*>(object);

            for (auto& iter : _Callbacks) {
                std::vector<CallbackInfo>& vector = iter.second;
                for (size_t i = 0; i < vector.size(); i++) {
                    if (vector[i].object == castedObject) {
                        vector.erase(vector.begin() + i);
                        i = i > 0 ? i - 1 : 0;
                    }
                }
            }
        }

        template <class T>
        void registerEventListener(int32_t eventTypeId, void (T::*fun)(const Event*), T* obj) {
            if (eventTypeId == 0) {
                LOG(ERROR) << "Tried to register event listener for event type '0'";
                return;
            }
            CallbackFunction function = std::bind(fun, obj, std::placeholders::_1);
            CallbackInfo info(function, obj);

            _Callbacks[eventTypeId].push_back(info);
        }

    private:
        /* TODO: Optimize storing and getting */
        std::map<int32_t, std::vector<CallbackInfo> > _Callbacks;
    };

} /* namespace SEventDispatcher */