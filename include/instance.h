#pragma once

#include "renderer.h"

/**
 * @brief Create a instance object for vulkan.
 *
 * @param renderer pointer to the renderer struct to create the instance for.
 * @return int CReturnSuccess if successful, CReturnFailure otherwise.
 */
int create_instance (VKRenderer *renderer);
