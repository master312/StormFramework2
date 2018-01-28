#pragma once
#include <vector>
#include <functional>
#include "core/StormModuleBase.h"

/* Macro used for firing global notifications.
 * @type is element from enum 'SNotificationType'
 * @data is data pointer passed to callback methods. Will be cast to void* */
#define S_FIRE_GLOBAL_NOTIFICATION(type, data) StormEngine::getGlobalNotifier()->fire(type, static_cast<void*>(data));
/* Macro used for registering new listener to global notification system.
 * @type is element from enum 'SNotificationType'
 * @object is pointer to class object which is listening for notifications
 * @cbFunction is callback function that will be called in case of notification */
#define S_ADD_GLOBAL_NOTIFICATION_LISTENER(type, object, cbFunction) StormEngine::getGlobalNotifier()->addListener(type, object, std::bind(&cbFunction, object, std::placeholders::_1));
/* Macro used for removing notification listener from system.
 * First parameter is object, and second parameter is notification type.
 * Second parameter is optional */
#define S_REMOVE_GLOBAL_NOTIFICATION_LISTENER(...) StormEngine::getGlobalNotifier()->removeListeners(__VA_ARGS__);

/* Class used for sending notifications between different parts of engine */

enum SNotificationType {
    UNKNOWN = 0,
    /* Fired every time new scene object has been added to scene */
    SCENE_OBJECT_ADDED,
    /* Fired when new scene object prefab has been instantiated */
    SCENE_PREFAB_INSTANTIATED,

    /* Should always be last element in this enum! */
    NOTIFICATION_TYPE_LAST
};

class SGlobalNotifier : public StormModuleBase {
public:
    SGlobalNotifier();
    ~SGlobalNotifier();

    void addListener(SNotificationType type, void* object, std::function<void(void*)> cbMethod);

    /* Remove all listeners of @type connected to @object */
    void removeListeners(void* object, SNotificationType type = UNKNOWN);

    /* Fire notification of @type to all registered listeners. */
    void fire(SNotificationType type, void* data = nullptr);

private:
    struct SNotificationListener {
        /* Pointer to object that is listening for notification */
        void* object;
        /* Type of notification for which is this object listening */
        SNotificationType type;
        /* Notification callback method */
        std::function<void(void*)> callback;

        SNotificationListener(void* _object, SNotificationType _type, std::function<void(void*)> method)
                : object(_object), type(_type), callback(method) { }
    };

    std::vector<SNotificationListener> _Listeners[NOTIFICATION_TYPE_LAST];
};
