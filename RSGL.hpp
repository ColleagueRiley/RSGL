#ifdef wii
    #include "include/wii.hpp"
#endif
#ifdef WIN32
    #include "include/windows.hpp"
#endif
#if __linux__
    #include "include/linux/rsgl.hpp"
    #include "include/linux/joystick.hpp"
    #include "include/linux/keys.hpp"
#endif
#if __APPLE__
    #include "include/linux.hpp"
#endif
