#pragma once
#ifndef WIN_NOEXCEPT
#define WIN_NOEXCEPT
#endif
#define _WIN32_WINNT 999999

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <windows.h>
#include <psapi.h>
#include <shlobj.h>
#include <vector>
#include <chrono>
#include <gdiplus.h>
#include "deps/win32/Bitmap_OP.h"
#include "deps/win32/win32_SetProcessDpiAware.h"
#include "deps/win32/no_sal2.h"
#include "deps/win32/Xinput.h" //Init the WIN32 files that requires the Win SDK
#include "deps/DiscordSDK/discord.h"
#include "../linux/deps/png++/image.hpp"
#include "GL/gl.h" // OpenGL Rendering 

//#include "d3d9.h" // DirectX Rendering
#define BYTESTOGB(memory) round((double)memory/1073741824)  // Converts bytes to GB and then rounds it up. Used for converting RAM bytes into GBs
#define BYTESTOMB(memory) round((double)memory/1048576) // Converts bytes to MB and then rounds it up. Used for converting VRAM bytes into MBs.

/* Important Windows functions */
std::string wintest();
LRESULT CALLBACK WindowProc(HWND h, UINT msg, WPARAM param, LPARAM lparam);
LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam);

namespace RSGL{ /* It all starts with this, the namespace containing the Ultimate Power.*/
    const int KeyPressed=2; // a key has been pressed
    const int KeyReleased=3; // a key has been released
    const int MouseButtonPressed=4; // a mouse button has been pressed (left,middle,right)
    const int MouseButtonReleased=5; // a mouse button has been released (left,middle,right)
    const int MousePosChanged=6; // the position of the mouse has been changed
    const int quit = 33; // the user clicked the quit button
    const int dnd = 34; // a file has been dropped into the window
    const int MouseWheelScrolling = 11; /* Event type, indicating the mousewheel is scrolling up*/

    struct rect{ int x, y; int width, length; }; /*Rectangle structure.*/
    struct point{ int x, y;}; /* Point structure.*/
    struct circle { int x, y; int radius; }; /*Circle structure.*/
    struct triangle {  int x, y; int width, length; }; /*Triangle structure.*/
    struct color { int r, g, b; int a=255; }; /*Color structure in RGB.*/
    struct image {const char* filename; rect r; std::vector<std::vector<float>>  pixels; std::vector<RSGL::color> clr;}; /*Image structure.*/
    struct text{
      RSGL::circle rect;
      RSGL::color c; 
      std::string text; 
      const char* f;
    }; /*Text structure.*/
    struct resolution {int x,y,hz;};
    struct device_info {
        std::string os, model, name, serial_number; 
        int hdd_total, hdd_used, hdd_free;
        int ram_total, ram_used, ram_free, ram_percentage; 
        std::string cpu; int cpu_cores, cpu_percentage;  std::string cpu_architecture;
        std::string gpu, driver_version; int vram; 
        int architecture;
    }; /* The device information structure. */
    struct timer { 
        int h, min, s, ms; 
        std::chrono::time_point<std::chrono::_V2::system_clock> id;
        double past;
    }; /* The timer structure.*/
    struct debug {
        int fps, vram_used, vram_available;
        std::string os, cpu, gpu, driver;
    }; /*The debug table structure.*/

    struct drawable {rect r; color c; uint32_t enabled_flags; HWND hwnd; int old_key; int check;Bitmap_Operations* biop;}; /* Window information*/
    
    struct window : drawable { /* The `window` structure. Stores the important information about the window and program.*/
        public:
            const char* name; /* Name of the window.*/
            rect r;
            color c; /* RGB color of the window.*/
            uint32_t enabled_flags; /* Checks the windows flags that are enabled.*/
            HWND hwnd; /* The window */

            struct Event {
                int x; /*X position of the mouse.*/
                int y; /*Y position of the mouse.*/
                int type; /* Event type variable, telling what and which action happened.*/
                int button; /* Value indicating which mouse button was used.*/
                point pad; /*The X and Y positions of the L-stick.*/ 
                point r_pad; /*The X and Y positions of the R-stick.*/
                int triggers[2] = {0,0}; /*The values of the trigger buttons from 0 to 255. `trigger[0]` is LT, while `trigger[1]` is RT.*/
            };
            Event event; // The Event variable.
            RSGL::debug debug;

            /* Inits and creates a window.*/
            window(const char* name, rect r, color c, uint32_t flag=0);
            /* Updates everything what happened this exact frame. Required in every RSGL project.*/
            int checkEvents();
            /* Refreshes the screen. Reccomended to put at the end of the loop as you cannot draw on the screen after using this function before it jumps back to the start of the loop.*/
            int clear();
            /* Quits the program.*/
            int close();
            /*Adds a flag to the window if to make it resizable or not depending on the `value` parem.*/
            void resize(bool value);
            /*Adds a flag to the window if to make it fullscreen or not depending on the `value` parem.*/
            void fullscreen(bool value);
            /* Adds a flag to the window if to maximize the window.*/
            void maximize(bool value);
            /*Adds a flag to the window to change the resolution and position of the window.*/
            void position(rect window);
           /*Adds a flag to the window to make the window centralized depending on the width and height.*/
            void centralize();
            /* Returns the current screen resolution in a `point` struct.*/
            RSGL::resolution getScreenResolution();
            /* Returns a vector list of resolutions (and the frequency) that the computer supports. */
            std::vector<RSGL::resolution> getAvailableResolutions();
            /* Changes the Window's icon.*/
            int changeIcon(const char* filename);
            /* Changes the Window's name.*/
            int changeName(const char* name);
    
            /*Checks if a key is being pressed If so return true, else return false.*/
            bool isPressed(int key, int port=0);
            /*Checks if a key is being clicked __AT THIS EXACT FRAME__. If so return true, else return false.*/
            bool isClicked(int key, int port=0);
            /*Checks if a key was released this frame. If so return true, else return false.*/
            bool isReleased(int key, int port=0);
        private: 
            int ProcMSG();
    };
    extern drawable win;
    /*LPDIRECT3D9 d3d;
    LPDIRECT3DDEVICE9 d3ddev;
    LPDIRECT3DVERTEXBUFFER9 v_buffer = NULL;*/

    // ============ DRAWING FUNCTIONS ============
    /*Draws a rectangle. Parameter `solid` is always set to true.*/
    int drawRect(rect r, color c, bool solid=true, bool dotted=false, int border_size=3); 
    /*Draws a triangle. Parameter `solid` is always set to true.*/
    int drawTriangle(triangle t, color c, bool solid=true);
    /*Draws a circle. Parameter `solid` is always set to true.*/
    int drawCircle(circle c, color col, bool solid=true, int border_size=3);
    /* Loads the image.*/
    image loadImage(const char* filename, rect r, bool resize=false, bool upwards=false, bool reverse=false);
    /* Draws the image. Requires the usage of `RSGL::loadImage()` to get the `img` parameter.*/
    int drawImage(image ig); 
    /* Resizes the image.*/
    std::vector<std::vector<RSGL::color>> resizeImage(std::vector<std::vector<RSGL::color>> image, RSGL::rect newSize, RSGL::rect ogsize);
    /*Loads text into memory. (Highly) Advised not to load the text every frame, as doing so can decrease the performance very quickly, even on high-end devices.*/
    //text loadText(const char* word,rect r, const char* font, color c);
    /* Draws the text. Requires the usage of `RSGL::loadText()` to get the `txt` parameter.*/
    int drawText(std::string text, RSGL::circle r, const char* font, RSGL::color col); 
    /*Draws a line (rectangle). Parameter `solid` is always set to true.*/
    #define drawLine drawRect

    // ============ Collide FUNCTIONS ============
    /*Circle coliding with point.*/
    int CircleCollidePoint(RSGL::circle c, RSGL::point p);
    /*Circle coliding with rect.*/
    int CircleCollideRect(RSGL::circle c, RSGL::rect r);
    /*Circle coliding with another circle.*/
    int CircleCollideCircle(RSGL::circle cir1,RSGL::circle cir2);
    /*Rect coliding with point.*/
    int RectCollidePoint(RSGL::rect r, RSGL::point p);
    /*Rect coliding with another rect.*/
    int RectCollideRect(RSGL::rect r, RSGL::rect r2);
    /*Point coliding with another point.*/
    int PointCollidePoint(RSGL::point p, RSGL::point p2);
    /*Image coliding with rect.*/
    int ImageCollideRect(RSGL::image img, RSGL::rect r);
    /*Image coliding with circle.*/
    int ImageCollideCircle(RSGL::image img, RSGL::circle c);
    /*Image coliding with point.*/
    int ImageCollidePoint(RSGL::image img, RSGL::point p);
    /*Image coliding with another image.*/
    int ImageCollideImage(RSGL::image img, RSGL::image img2);

    /* ========== .INI Functions ========== */
    // Reads a key from an settings.ini
    std::string readIniKey(const char* path, std::string Key); 
    // Writes a key from settings.ini
    int writeIniKey(const char* path, std::string Key, std::string value); 
    // Deletes a key from settings.ini
    int deleteIniKey(const char* path, std::string Key);
    // Deletes a section from settings.ini
    int deleteIniSection(const char* path); 
    // Checks if a key exists in settings.ini
    bool iniKeyExists(const char* path, std::string Key);

    // ========== Rich Discord Presence Functions ==========
    /* Inits the Discord Presence by getting the application ID. Parameter `timer` is set to false by default.*/
    int discordCreateApplication(discord::ClientId clientID, bool timer=false);
    /* Updates the Discord Presence. All paremeters are set to nothing by default.*/
    int discordUpdatePresence(const char* details="", const char* state="", const char* largeImage="", const char* largeText="", const char* smallImage="", const char* smallText="");

    // ========== Device Information ==========
    /* Gets the important information of the PC. Note that when using this function, it takes at least 2 seconds to load up everything. Load time will vary depending on hardware, so load this only once.*/
    device_info getFullDeviceInfo();
    /* Gets only the RAM information.*/
    device_info getRAMDeviceInfo(bool total=true, bool available=true, bool used=true, bool percentage=true);
    /* Gets only the HDD information.*/
    device_info getHDDDeviceInfo(bool total=true, bool free=true, bool used=true);
    /* Gets only the OS information.*/
    device_info getOSDeviceInfo(bool os=true, bool model=true, bool pc_name=true, bool bit=true, bool serial_number=true);
    /* Gets only the GPU information.*/
    device_info getGPUDeviceInfo(bool gpu=true, bool driver_version=true, bool vram=true);
    /* Gets only the CPU information. This is the most time-consuming info function.*/
    device_info getCPUDeviceInfo(bool cpu=true, bool cores=true, bool percentage=true, bool architecture=true);

    // ============ Misc Functions ============
    /* Gets the username of the user and returns it as a string.*/
    std::string getUsername();
    /* Gets the language code of the user.*/
    int getLanguage();
    /* Gets the user's clipboard text and returns it as a string.*/
    std::string getClipboardText();
    /* Sets the user's clipboard text to parameter `txt`.*/
    int setClipboardText(std::string txt);
    /* Opens a file/folder dialog.
    When using the `multiple` parameter, the function will output "<directory> <file 1> <file 2> <file 3> (etc.)".
    Example for title - "All\0*.*\0Text\0*.TXT\0".*/
    std::string fileDialog(const char* title, bool multiple=false, bool save=false, bool directory=false);
    /*Opens a message box and returns a value, indicating which button the user clicked.
    `option` parameter list:
    0  - Error
    1  - Warning
    2  - Question
    `flags` parameter list:
    OK
    HELP
    OKCANCEL
    RETRYCANCEL
    YESNOCANCEL
    YESNO
    CANCELTRYCONTINUE*/
    int messageBox(std::string title, std::string message, int option=-1, UINT flags=0);
    /* Gets the current FPS of the program.*/
    int getFPS();
    /* Gets the numbers after the decimal point (eg. function will output 9 if `num` is something like 1.9)*/
    int findNumberAfterDecimalPoint(float num, int howManyKeep=-1);
    
    // ============ Timer Functions ============
    /* Starts a timer.*/
    timer startTimer();
    /* Updates the timer.*/
    int updateTimer(timer& t, int howManyKeep=-1);

    // ============ Xinput Functions  ============
    /* Check if a controller is connected. Parameter `port` is 0 by default, meaning it'll check if the 1st controller is connected.*/
    bool xinputConnected(int port=0);
    /* Vibrates the controller. Parameter `port` is 0 by default, parameters `leftVal` and `rightVal` are 65535 by default.*/
    int xinputVibrate(int leftVal=65535, int rightVal=65535, int port=0);
    /* Returns the LT and RT values in a `trigger` struct (x and y) from 0-255. Parameter `port` is 0 by default.*/
    point xinputGetTriggerValues(int port=0);
    /* Returns the battery values in a `battery` struct (x and y). Parameter `port` is 0 by default.
    X returns:
    0 - The controller isn't connected.
    1 - The controller is wired.
    2 - The controller uses an alkaline battery.
    3 - The controller uses a nickel metal hydride battery. 
    4 - The controller uses an unknown battery type.
    Y returns:
    0 - Battery level is empty.
    1 - Battery level is low.
    2 - Battery level is medium.
    3 - Battery level is full.
    */
    point xinputGetBattery(int port=0);
}