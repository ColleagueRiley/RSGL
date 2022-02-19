//
// Created by kumar on 2016/03/29.
//

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

/// Sets the current process as (per-monitor) dpi aware.
/// \return true if succeed.
bool win32_SetProcessDpiAware(void);

#ifdef __cplusplus
}
#endif
