#pragma once

#include "renderer.h"

/**
 * @brief Create a window object for the renderer.
 *
 * @param renderer pointer to the renderer struct to create the window for.
 * @return int CReturnSuccess if successful, CReturnFailure otherwise.
 */
int create_window (VKRenderer *renderer);
