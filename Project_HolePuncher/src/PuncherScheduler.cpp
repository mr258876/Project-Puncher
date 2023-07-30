#include "PuncherScheduler.h"

PuncherScheduler::PuncherScheduler()
{
}

PuncherScheduler::~PuncherScheduler()
{
}

time_t PuncherScheduler::getETA()
{
    double _eta = 0;

    int _lastX = 0;
    double _lastZ = 0;

    for (size_t i = 0; i < holeList.size(); i++)
    {
        _eta += abs(holeList.at(i).x - _lastX) * 2.0 / X->getSpeed(); // X轴
        _eta += abs(holeList.at(i).z - _lastZ) / Z->getSpeed();       // Y轴
        _eta += 11 / Y->getSpeed();                                   // Z轴

        _lastX = holeList.at(i).x;
        _lastZ = holeList.at(i).z;
    }

    return (long)_eta;
}

void PuncherScheduler::ui_cb_draw_configurable_menu(PuncherUI *ui, Configurable *item, void *ui_param)
{
    /* TODO: item settings access control */

    std::vector<ConfigurableItem> itemList = item->getConfigurations(true);
    std::vector<ui_menu_item_t> menu;
    for (ConfigurableItem &i : itemList)
    {
        ui_menu_item_t menu_item;

        menu_item.disp_name = i._name;
        menu_item.cb = ([this, ui, i](void *param)
                        {
                            int res = i._callback(param);
                            /* TODO -> */ 
                            // ui->alert(res); 
                            /* <- TODO */ 
                            return res; 
                        });

        switch (i._itemType)
        {
        case CONFIGURABLE_DUMBTEXT:
        {
            menu_item.item_type = MENU_ITEM_TYPE_TEXT;
            ConfigurableDumbText *ip = static_cast<ConfigurableDumbText *>(&i);
            menu_item.data = (void *)(ip->_value.c_str());
            break;
        }
        case CONFIGURABLE_CALLBACK:
        {
            menu_item.item_type = MENU_ITEM_TYPE_CALLBACK;
            ConfigurableCallback *ip = static_cast<ConfigurableCallback *>(&i);
            menu_item.data = (void *)(ip->_value);
            break;
        }
        case CONFIGURABLE_ENUM:
        {
            menu_item.item_type = MENU_ITEM_TYPE_DROP_DOWN;
            ConfigurableEnum *ip = static_cast<ConfigurableEnum *>(&i);
            menu_item.data = (void *)(ip->_value);
            break;
        }
        case CONFIGURABLE_SUBMENU:
        {
            menu_item.item_type = MENU_ITEM_TYPE_SUB_MENU;
            ConfigurableSubmenu *ip = static_cast<ConfigurableSubmenu *>(&i);
            menu_item.data = (void *)(ip->_value);
            break;
        }
        case CONFIGURABLE_BOOLEAN:
        {
            ConfigurableBoolean *ip = static_cast<ConfigurableBoolean *>(&i);
            switch (ip->_style)
            {
            case CONFIGURABLE_BOOLEAN_CHECKBOX:
                menu_item.item_type = MENU_ITEM_TYPE_CHECKBOX;
                break;

            default:
                menu_item.item_type = MENU_ITEM_TYPE_SWITCH;
                break;
            }
            menu_item.data = &ip->_value;
            break;
        }
        case CONFIGURABLE_INTEGER:
        {
            menu_item.item_type = MENU_ITEM_TYPE_SPINBOX;
            ConfigurableInteger *ip = static_cast<ConfigurableInteger *>(&i);
            menu_item.data = (void *)(&ip->_value);
            break;
        }
        case CONFIGURABLE_FLOAT:
        {
            menu_item.item_type = MENU_ITEM_TYPE_SPINBOX;
            ConfigurableFloat *ip = static_cast<ConfigurableFloat *>(&i);
            menu_item.data = (void *)(&ip->_value);
            break;
        }
        default:
        {
            break;
        }
        }

        menu.push_back(menu_item);
    }

    ui->drawMenu(menu, ui_param);
}


void PuncherScheduler::onUINotify(void *param)
{
    scheduler_request_t *p = (scheduler_request_t *)(param);
    switch (p->endpoint)
    {
    case SCHEDULER_REQUEST_SELF_CALIBRATION:
    {
        /* code */
        break;
    }
    case SCHEDULER_REQUEST_SET_STATUS:
    {
        /* code */
        break;
    }
    case SCHEDULER_REQUEST_FEED_PAPER:
    {
        /* code */
        break;
    }
    case SCHEDULER_REQUEST_ADD_HOLE:
    {
        addHole(*((scheduler_hole_t *)p->request_data));
        break;
    }
    case SCHEDULER_REQUEST_CLEAR_HOLE:
    {
        holeList.clear();
        break;
    }
    default:
        break;
    }
}


void PuncherScheduler::onMotorNotify(void *param)
{
    observe_notify_t *p = (observe_notify_t *)(param);
    switch (*p)
    {
    case OBSERVE_NOTIFY_NO_TYPE:
    {
        /* code */
        break;
    }
    case OBSERVE_NOTIFY_ON_VALUE_CHANGE:
    {
        /* code */
        break;
    }
    case OBSERVE_NOTIFY_ON_OPERATION:
    {
        /* code */
        break;
    }
    case OBSERVE_NOFITY_ON_CONFIRM:
    {
        /* code */
        break;
    }
    case OBSERVE_NOTIFY_ON_INFO:
    {
        /* code */
        break;
    }
    case OBSERVE_NOTIFY_ON_WARNING:
    {
        /* code */
        break;
    }
    case OBSERVE_NOTIFY_ON_ERROR:
    {
        /* code */
        break;
    }
    case OBSERVE_NOTIFY_ON_CRITICAL:
    {
        /* code */
        break;
    }
    default:
        break;
    }
}