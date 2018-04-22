#pragma once
#include <functional>
#include <map>
#include <vector>
#include "../StormCommon.h"

/* Event notification system. Uses observer pattern */

namespace SEventListener {

    class Event {
    public:
        int32_t typeId;

        Event() : typeId(0) { }
    };

    typedef std::function<void(const Event&)> CallbackFunction;

    struct CallbackInfo {
        CallbackFunction function;
        void* object;

        CallbackInfo() : function(nullptr), object(nullptr) { }
        CallbackInfo(CallbackFunction fun, void* obj) : function(fun), object(obj) { }
    };

    class Listener {
    public:
        Listener() { }
        ~Listener() { }

        void fireEvent(Event event) {
            auto iter = _Callbacks.find(event.typeId);
            if (iter == _Callbacks.end()) {
                return;
            }
            for (CallbackInfo& info : iter->second) {
                info.function(event);
            }
        }

        template <class T>
        void removeListener(T* object) {
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
        void registerEventListener(int32_t eventTypeId, void (T::*fun)(const Event&), T* obj) {
            if (eventTypeId == 0) {
                LOG(ERROR) << "Tried to register event listener for event type '0'";
                return;
            }
            std::function<void(const Event&)> function = std::bind(fun, obj, std::placeholders::_1);
            CallbackInfo info(function, obj);

            _Callbacks[eventTypeId].push_back(info);
        }

    private:
        /* TODO: Optimize storing and getting */
        std::map<int32_t, std::vector<CallbackInfo> > _Callbacks;
    };



} /* namespace SEventListener */