#include "constants.h"
#include "renderer.h"
#include "window.h"
#include "instance.h"

int init_renderer (VKRenderer *renderer)
{
    if (create_window(renderer) != CReturnSuccess)
        return CReturnFailure;

    if (create_instance(renderer) != CReturnSuccess)
        return CReturnFailure;

    return CReturnSuccess;
}