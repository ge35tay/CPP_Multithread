#include "singleton_class.h"

singleton& get_singleton()
{
    static singleton single;
    return single;
}