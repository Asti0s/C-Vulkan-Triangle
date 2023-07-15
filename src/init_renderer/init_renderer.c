#include "constants.h"
#include "window.h"
#include "instance.h"
#include "device.h"

int init_renderer (VKRenderer *renderer)
{
    if (create_window(renderer) != CReturnSuccess)
        return CReturnFailure;

    if (create_instance(renderer) != CReturnSuccess)
        return CReturnFailure;

    if (pick_physical_device(renderer) != CReturnSuccess)
        return CReturnFailure;

    return CReturnSuccess;
}
