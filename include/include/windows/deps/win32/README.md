# win32_SetProcessDpiAware

Sets the current process as (per-monitor) dpi aware.

## Usage

Put them into your project.

And include `win32_SetProcessDpiAware.h` and call `bool win32_SetProcessDpiAware(void)` before creating the window.

## Example

```c
#include <stdio.h>
#include "SDL.h"

#ifdef _WIN32
#include "win32_SetProcessDpiAware.h"
#endif

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2

#ifdef _WIN32
    win32_SetProcessDpiAware();            // Disable dpi scaling
#endif
    
    // Create an application window with the following settings:
    SDL_Window *window = SDL_CreateWindow(
        "An SDL2 window",                  // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        640,                               // width, in pixels
        480,                               // height, in pixels
        SDL_WINDOW_OPENGL                  // flags - see below
    );

    // Check that the window was successfully created
    if (window == NULL) {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    // The window is open: could enter program loop here (see SDL_PollEvent())

    SDL_Delay(3000);  // Pause execution for 3000 milliseconds, for example

    // Close and destroy the window
    SDL_DestroyWindow(window);

    // Clean up
    SDL_Quit();
    return 0;
}
```

## Internal

Do the `SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE)` or `SetProcessDPIAware()`.
They are loaded dynamically from the system DLLs to support older environment and MinGW. 

## License

BSD 2-clause
