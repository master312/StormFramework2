#include "core/StormCommon.h"
#include "SGlobalNotifier.h"

SGlobalNotifier::SGlobalNotifier() : StormModuleBase("StormGlobalNotifier") {
}

SGlobalNotifier::~SGlobalNotifier() {
    /* No dynamically allocated memory.
     * Clear containers just to be save */
    for (auto& iter : _Listeners) {
        iter.clear();
    }
}

void SGlobalNotifier::addListener(SNotificationType type, void* object, std::function<void(void*)> cbMethod) {
    if (!object || !cbMethod || type == SNotificationType::UNKNOWN) {
        LOG(ERROR) << "Could not add notification listener. Invalid parameters";
        return;
    }
    SNotificationListener listener(object, type, cbMethod);

#if !defined(PRODUCTION) || defined(STORM_EDITOR)
    for (auto& iter : _Listeners[(int)type]) {
        if (iter.object == object && iter.type == type) {
            LOG(WARNING) << "Trying to register same Global Notification callback multiple times.";
            return;
        }
    }
#endif

    _Listeners[(int)type].push_back(listener);
}

void SGlobalNotifier::removeListeners(void* object, SNotificationType type /* = UNKNOWN */) {
    if (type == SNotificationType::UNKNOWN) {
        /* Remove all @object callbacks, regardless of type */
        for (auto& listeners : _Listeners) {
            for (size_t i = 0; i < listeners.size(); i++) {
                if (listeners[i].object == object) {
                    listeners.erase(listeners.begin() + i);
                    i = i > 0 ? i - 1 : 0;
                }
            }
        }
        return;
    }
    /* Remove listener for selected type */
    std::vector<SNotificationListener>& listeners = _Listeners[(int)type];
    for (size_t i = 0; i < listeners.size(); i++) {
        if (listeners[i].object == object) {
            listeners.erase(listeners.begin() + i);
            i = i > 0 ? i - 1 : 0;
        }
    }
}

void SGlobalNotifier::fire(SNotificationType type, void* data /* = nullptr */) {
    for (auto& vecIter : _Listeners[(int)type]) {
        vecIter.callback(data);
    }
}