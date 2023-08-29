#ifndef _OBSERVESTRUCTS_H_
#define _OBSERVESTRUCTS_H_

enum observe_notify_t
{
    OBSERVE_NOTIFY_NO_TYPE,
    OBSERVE_NOTIFY_ON_VALUE_CHANGE,
    OBSERVE_NOTIFY_ON_OPERATION,
    OBSERVE_NOFITY_ON_CONFIRM,
    OBSERVE_NOTIFY_ON_INFO,
    OBSERVE_NOTIFY_ON_ALERT,
    OBSERVE_NOTIFY_ON_CONFIRM,
    OBSERVE_NOTIFY_ON_WARNING,
    OBSERVE_NOTIFY_ON_ERROR,
    OBSERVE_NOTIFY_ON_CRITICAL,
};

struct observe_data_t
{
    observe_notify_t interface;
    void *request_data;
    void *response_data;
};

#endif  // _OBSERVESTRUCTS_H_