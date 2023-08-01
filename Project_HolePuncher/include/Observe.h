#ifndef _OBSERVE_H_
#define _OBSERVE_H_

#include <algorithm>
#include <functional>
#include <vector>

// class Observer
// {
// public:
//     virtual void request(void *param) = 0;
// };

using ObserverHandle = std::function<void(void *)>;
class Observeable
{
private:
    std::vector<ObserverHandle> observers;

public:
    void addObserver(const ObserverHandle &observer)
    {
        observers.push_back(observer);
    }

    // void removeObserver(const ObserverHandle &observer)
    // {
        // auto it = find(observers.begin(), observers.end(), observer);
        // if (it != observers.end())
        // {
        //     observers.erase(it);
        // }
    // }

    void notifyObservers(void *param)
    {
        for (const auto &observe : observers)
        {
            observe(param);
        }
    }
};

#endif // _OBSERVE_H_