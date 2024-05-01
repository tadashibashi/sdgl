#include "GamepadConstants.h"

namespace sdgl {
    static const char *s_btnNames[] = {
        "A",
        "B",
        "X",
        "Y",
        "Bumper Left",
        "Bumper Right",
        "Stick Left",
        "Stick Right",
        "Back",
        "Start",
        "Guide",
        "Left",
        "Right",
        "Up",
        "Down"
    };
    static_assert(GamepadBtn::Count == sizeof(s_btnNames) / sizeof(void *));

    static const char *s_axisNames[] = {
        "Left X",
        "Left Y",
        "Right X",
        "Right Y",
        "Trigger Left",
        "Trigger Right",
    };
    static_assert(GamepadAxis::Count == sizeof(s_axisNames) / sizeof(void *));

    string_view GamepadBtn::getName(GamepadBtn::Enum btn)
    {
        return s_btnNames[btn];
    }

    string_view GamepadAxis::getName(GamepadAxis::Enum axis)
    {
        return s_axisNames[axis];
    }
}
