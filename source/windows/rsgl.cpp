#ifndef RSGL
#include "../../include/include/windows/rsgl.hpp"
#endif
#include "deps/Bitmap_OP.cpp"
#include "deps/win32_SetProcessDpiAware.c"

std::string wintest() { 
    return "windows gay";
}

HGLRC glrc;

RSGL::window::window(const char* name, RSGL::rect r, RSGL::color c, uint32_t flag/*=0*/) {
    if (flag & 32) {  }
    else { win32_SetProcessDpiAware(); } //We check if DPI is enabled

    HINSTANCE inh = GetModuleHandle(nullptr); // We get the instance

    WNDCLASSEXA Class;
	ZeroMemory(&Class, sizeof(WNDCLASSEXW));
    Class.cbSize = sizeof(WNDCLASSEXW);
    Class.lpszClassName = TEXT("RSGL Windows (Official)");
    Class.hInstance = inh;
    Class.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    Class.hCursor = LoadCursor(NULL, IDC_ARROW);
    Class.lpfnWndProc = WindowProc;
    Class.hbrBackground = CreateSolidBrush(RGB(c.r, c.g, c.b));
    Class.cbClsExtra = 0;
	Class.cbWndExtra = 0;
    Class.lpszMenuName = NULL;
    Class.hIconSm = NULL;
    RegisterClassExA(&Class);

    RECT Rect = {r.x, r.y, r.length, r.width}; 
    AdjustWindowRect(&Rect, WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_BORDER | WS_SIZEBOX, false);
    HWND window = CreateWindowEx(0, 
        TEXT("RSGL Windows (Official)"), 
        name, 
        WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_BORDER | WS_SIZEBOX, 
        r.x, r.y, r.width, r.length, 
        NULL, 
        NULL, 
        inh, 
        NULL
    );                
    RSGL::window::hwnd = window;
    if (flag & 64) { //GDI rendering
        Gdiplus::GdiplusStartupInput input = {0}; 
        input.GdiplusVersion = 1;
        ULONG_PTR token;
        Gdiplus::GdiplusStartup(&token, &input, NULL);
    }
    /*else if (RSGL::win.enabled_flags & 512) { // DirectX rendering
        d3d = Direct3DCreate9(D3D_SDK_VERSION);
        D3DPRESENT_PARAMETERS d3dpp;

        ZeroMemory(&d3dpp, sizeof(d3dpp));
        d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
        d3dpp.hDeviceWindow = window;
        d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
        d3dpp.BackBufferWidth = r.width;
        d3dpp.BackBufferHeight = r.length;

        d3d->CreateDevice(D3DADAPTER_DEFAULT,
                        D3DDEVTYPE_HAL,
                        window,
                        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                        &d3dpp,
                        &d3ddev);
        
        
    }*/
    else { // OpenGL rendering
        PIXELFORMATDESCRIPTOR pfd;
        ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
        pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 32;
        pfd.cDepthBits = 24;
        pfd.cStencilBits = 8;
        
        HDC hdc = GetDC(hwnd);
        int format = ChoosePixelFormat(hdc, &pfd);
        SetPixelFormat(hdc, format, &pfd);
        glrc = wglCreateContext(hdc);
        wglMakeCurrent(hdc, glrc);
        
        glEnable(GL_BLEND); //Enable blending.
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set blending function.
        glClearColor((float)c.r/255, (float)c.g/255, (float)c.b/255, float(c.a)/255);
    }

    RSGL::window::enabled_flags=flag;
    RSGL::window::r = r;
    RSGL::window::hwnd = window;

    int show = SW_SHOW;
    if (flag & 8) { show=SW_SHOWMAXIMIZED; } // MAXAMIZED_WINDOW flag
    else if (flag & 16) { show=SW_SHOWMINIMIZED; } // MINIMIZED_WINDOW flag

    if (flag & 1)  { }; // DEBUG, not implemented trolololol
    if (flag & 4)  { RSGL::window::fullscreen(true);}; // Fullscreen
    if (flag & 128) { RSGL::window::centralize();} //Centralized
    
    ShowWindow(window, show);
    UpdateWindow(window);
    SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, NULL, 0); // We check for mouse input
}


int RSGL::window::checkEvents(){
    RSGL::window::event.type = RSGL::window::ProcMSG();
    RSGL::win = {RSGL::window::r, RSGL::window::c, RSGL::window::enabled_flags, RSGL::window::hwnd, RSGL::win.old_key, RSGL::win.check, RSGL::win.biop};

    if (RSGL::window::event.type == RSGL::MousePosChanged) { //We get the mouse position (if the mouse position changed)
        POINT cursorPos;
        GetCursorPos(&cursorPos);
        RSGL::window::event.x = cursorPos.x-RSGL::window::r.x-15;
        RSGL::window::event.y = cursorPos.y-(RSGL::window::r.y);
    }

    if (RSGL::win.enabled_flags & 2) { // We check if xinput is enabled
        XINPUT_STATE state;
        ZeroMemory(&state, sizeof(XINPUT_STATE));
        XInputGetState(0, &state);

        RSGL::window::event.pad = {state.Gamepad.sThumbLX, state.Gamepad.sThumbLY}; // L-stick
        RSGL::window::event.r_pad = {state.Gamepad.sThumbRX, state.Gamepad.sThumbRY}; // R-stick
        RSGL::window::event.triggers[0] = state.Gamepad.bLeftTrigger;
        RSGL::window::event.triggers[1] = state.Gamepad.bRightTrigger;
    }
    if (RSGL::win.enabled_flags & 64) { // GDI rendering
        PAINTSTRUCT ps;
        BeginPaint(RSGL::window::hwnd, &ps);

        HBRUSH background_brush = CreateSolidBrush(RGB(RSGL::window::c.r, RSGL::window::c.g, RSGL::window::c.b));
        RECT rect;
        GetClientRect(RSGL::window::hwnd, &rect);
        FillRect(RSGL::win.biop->Get_DC_Buffer(0),&rect,background_brush);
        DeleteObject(background_brush);
    }
    /*else if (RSGL::win.enabled_flags & 512) { // DirectX rendering
        
    }*/
    else { glClear(GL_COLOR_BUFFER_BIT); } // OpenGL rendering
    /*if (RSGL::win.enabled_flags & 1) {
        if (!RSGL::check_hardware_info) {
            device_info info = RSGL::getGPUDeviceInfo();
            window::debug.gpu = info.gpu;
            window::debug.vram_available = info.vram;
            window::debug.driver = info.driver_version;
            info = RSGL::getCPUDeviceInfo(true, false, false, false);
            window::debug.cpu = info.cpu;
            info = RSGL::getOSDeviceInfo(true, false, false, false, false);
            window::debug.os = info.os;
            check_hardware_info=true;
        }
        PROCESS_MEMORY_COUNTERS_EX pmc;
        GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
        window::debug.fps = RSGL::getFPS();
        window::debug.vram_used =  BYTESTOMB(pmc.PeakWorkingSetSize)/2;
    }*/
    Sleep(1);
    return 0;
}

int RSGL::window::clear() {
    if (RSGL::win.enabled_flags & 64) { RSGL::win.biop->Copy_to_Screen(0);} // GDI Rendering
    /*else if (RSGL::win.enabled_flags & 512) {// DirectX rendering
        d3ddev->EndScene();
        d3ddev->Present(NULL, NULL, NULL, NULL);
    }*/
    else {  glFlush(); } // OpenGL rendering
    PAINTSTRUCT ps;
    EndPaint(RSGL::window::hwnd, &ps);
    return 0;
}

int RSGL::window::close() {
    if (RSGL::win.enabled_flags & 64) { Gdiplus::GdiplusShutdown((ULONG_PTR)0); } // GDI rendering
    /*else if (RSGL::win.enabled_flags & 512) { // DirectX rendering
        v_buffer->Release();
        d3ddev->Release();
        d3d->Release();
    }*/
    else { // OpenGL rendering
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(glrc);
    }
    DestroyWindow(RSGL::window::hwnd);
    PostQuitMessage(0);
    return 0;
}

void RSGL::window::resize(bool value){
    if (value) SetWindowLong(RSGL::window::hwnd, GWL_STYLE, GetWindowLong(RSGL::window::hwnd, GWL_STYLE)|WS_SIZEBOX);
    else SetWindowLong(RSGL::window::hwnd, GWL_STYLE, GetWindowLong(RSGL::window::hwnd, GWL_STYLE)&~WS_SIZEBOX);
}

void RSGL::window::fullscreen(bool value) {
    DWORD style = GetWindowLong(RSGL::window::hwnd, GWL_STYLE);
    MONITORINFO mi = { sizeof(mi) };
	if (value) {
		RECT rect;
		GetWindowRect(RSGL::window::hwnd, &rect);
        RSGL::window::r = {rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top};

		GetMonitorInfo(MonitorFromWindow(RSGL::window::hwnd, MONITOR_DEFAULTTOPRIMARY), &mi);
		SetWindowLong(RSGL::window::hwnd, GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);
		SetWindowPos(RSGL::window::hwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top,
			mi.rcMonitor.right - mi.rcMonitor.left,
			mi.rcMonitor.bottom - mi.rcMonitor.top,
			SWP_NOOWNERZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
	}
	else {
		GetMonitorInfo(MonitorFromWindow(RSGL::window::hwnd, MONITOR_DEFAULTTOPRIMARY), &mi);
		SetWindowLong(RSGL::window::hwnd, GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
		SetWindowPos(RSGL::window::hwnd, HWND_NOTOPMOST, RSGL::window::r.x, RSGL::window::r.y, RSGL::window::r.width, RSGL::window::r.length, SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
	}
}
void RSGL::window::position(RSGL::rect window){ MoveWindow(RSGL::window::hwnd, window.x, window.y, window.width, window.length ,true); }
void RSGL::window::maximize(bool value) {
    int g;
    if (value) g = SW_SHOWMAXIMIZED;
    else g=SW_SHOWNORMAL;
    ShowWindow(RSGL::window::hwnd, g);
}

void RSGL::window::centralize() {
	MONITORINFO mi = { sizeof(mi) };

	GetMonitorInfo(MonitorFromWindow(RSGL::window::hwnd, MONITOR_DEFAULTTONEAREST), &mi);
	int x = (mi.rcMonitor.right - mi.rcMonitor.left - RSGL::window::r.width) / 2;
	int y = (mi.rcMonitor.bottom - mi.rcMonitor.top - RSGL::window::r.length) / 2;

	SetWindowPos(RSGL::window::hwnd, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_SHOWWINDOW);
}

RSGL::resolution RSGL::window::getScreenResolution() {
    HMONITOR monitor = MonitorFromWindow(RSGL::window::hwnd, MONITOR_DEFAULTTONEAREST);
    MONITORINFO info;
    info.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(monitor, &info);
    DEVMODE dev;
    dev.dmSize = sizeof(DEVMODE);
    EnumDisplaySettings(NULL, 0, &dev);
       
    return {info.rcMonitor.right - info.rcMonitor.left, info.rcMonitor.bottom - info.rcMonitor.top, (int)dev.dmDisplayFrequency};
}

std::vector<RSGL::resolution> RSGL::window::getAvailableResolutions() {
    DEVMODE dev;
    dev.dmSize = sizeof(DEVMODE);
    int i=0;
    std::vector<RSGL::resolution> list= { {0,0,0} };
    while (EnumDisplaySettings(NULL, i, &dev)) {
        if (list.begin()->x != (int)dev.dmPelsWidth && list.begin()->y != (int)dev.dmPelsHeight)  { list.insert(list.begin(), {(int)dev.dmPelsWidth, (int)dev.dmPelsHeight, (int)dev.dmDisplayFrequency}); }
        i++;
    }
    list.pop_back();
    return list;
}

int RSGL::window::changeIcon(const char* filename) {
    HANDLE hIcon = LoadImage(NULL, filename, IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
    if (hIcon) {
        SendMessage(RSGL::window::hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
        return 0;
    } 
    return -1;
}

int RSGL::window::changeName(const char* name) {
    SetWindowText(RSGL::window::hwnd, name);
    return 0;
}



bool RSGL::window::isPressed(int key, int port/*=0*/) {
    if (RSGL::win.enabled_flags & 2) {
        key-=100;
        if ((key == XINPUT_GAMEPAD_A || key == XINPUT_GAMEPAD_B || key == XINPUT_GAMEPAD_X || key == XINPUT_GAMEPAD_Y) || 
            (key == XINPUT_GAMEPAD_LEFT_THUMB || key == XINPUT_GAMEPAD_RIGHT_THUMB) || 
            (key == XINPUT_GAMEPAD_LEFT_SHOULDER || key == XINPUT_GAMEPAD_RIGHT_THUMB) ||
            (key == XINPUT_GAMEPAD_START || key == XINPUT_GAMEPAD_BACK) ||
            (key == XINPUT_GAMEPAD_DPAD_UP || key == XINPUT_GAMEPAD_DPAD_DOWN || key == XINPUT_GAMEPAD_DPAD_LEFT || key == XINPUT_GAMEPAD_DPAD_RIGHT) 
        ) {
            XINPUT_STATE state;
            ZeroMemory(&state, sizeof(XINPUT_STATE));
            
            XInputGetState(port, &state);
            return state.Gamepad.wButtons & key;
        }
        else if ((key == 0x5820 || key == 0x5821 || key == 0x5822 || key == 0x5823) || 
            (key == 0x5824 || key == 0x5825 || key == 0x5826 || key == 0x5827) || 
            (key == 0x5828 || key == 0x5829 || key == 0x5830 || key == 0x5831) || 
            (key == 0x5832 || key == 0x5833 || key == 0x5834 || key == 0x5835)) {
                XINPUT_STATE state;
                ZeroMemory(&state, sizeof(XINPUT_STATE));
                
                XInputGetState(port, &state);
                if (key == 0x5820 || key == 0x5821 || key == 0x5822 || key == 0x5823) {
                    int left[2] = {state.Gamepad.sThumbLX, state.Gamepad.sThumbLY};
                    return (left[0] >= 32767 && key == 0x5822) || (left[0] <= -32767 && key == 0x5823) || (left[1] >= 32767 && key == 0x5820) || (left[1] <= -32767 && key == 0x5821);
                }
                if (key == 0x5830 || key == 0x5831 || key == 0x5832 || key == 0x5833) {
                    int right[2] = {state.Gamepad.sThumbRX, state.Gamepad.sThumbRY};
                    return (right[0] >= 32767 && key == 0x5832) || (right[0] <= -32767 && key == 0x5833) || (right[1] >= 32767 && key == 0x5830) || (right[1] <= -32767 && key == 0x5831);
                }
            }
        else if (key == 0x5806 || key == 0x5807) { // RT and LT
            XINPUT_STATE state;
            ZeroMemory(&state, sizeof(XINPUT_STATE));

            XInputGetState(port, &state);
            return (state.Gamepad.bLeftTrigger > 0 || state.Gamepad.bRightTrigger > 0);
        }
        key+=100;
    }
    return GetKeyState(key) & 0x1000;
}

bool RSGL::window::isReleased(int key, int port/*=0*/) {
    if (RSGL::win.enabled_flags & 2) {
        /*Xinput needs to be implemented for isReleased.*/
    }
    int g = GetKeyState(key) & 0x1000; // We check if `key` is being pressed`
    if (g == 4096) { RSGL::win.old_key=key; return false; }  // If it is being pressed, set `RSGL::win.old_key` as `key` and return false
    else if (key==RSGL::win.old_key && g != 4096) {RSGL::win.old_key=695; return true;} // If `RSGL::win.old_key` is `key` but `key` isn't being pressed, then it means `key` was released recently. Set `RSGL::win.old_key` to anything to make sure it doesn't spam true
    return false;
}

bool RSGL::window::isClicked(int key, int port/*=0*/) {
    if (RSGL::win.enabled_flags & 2) {
        /*Xinput needs to be implemented for isClicked.*/
    }
    int g = GetKeyState(key) & 0x1000; 
    if (g == 4096 && key!=RSGL::win.old_key) { RSGL::win.old_key=key; return true; } 
    else if (g != 4096 && key==RSGL::win.old_key) {RSGL::win.old_key=635;}
    return false;
}

int RSGL::messageBox(std::string title, std::string message, int option/*=-1*/, UINT flags/*=0*/) {
    /* 
    -1 - Nothing
    0  - Error
    1  - Warning
    2  - Question
    */
    UINT opt=0;
    if (option == 0) opt = MB_ICONERROR | flags;
    else if (option == 1) opt = MB_ICONWARNING | flags;
    else if (option == 2) opt = MB_ICONQUESTION | flags;
    else if (option == -1) opt = flags;

    return MessageBox(RSGL::win.hwnd, message.c_str(), title.c_str(), opt);
}

std::string RSGL::fileDialog(const char* title, bool multiple/*=false*/, bool save/*=false*/, bool directory/*=false*/) {
    if (!directory) { 
        OPENFILENAME ofn;
        char szFile[260];
        
        // Initialize OPENFILENAME
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.lpstrFile = szFile;
        ofn.lpstrFile[0] = '\0';
        ofn.nMaxFile = 1024;
        ofn.lpstrFilter = title; 
        ofn.nFilterIndex = 0;//"All\0*.*\0Text\0*.TXT\0";
        if (multiple) ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER ;
        else ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        GetOpenFileNameA(&ofn);
        if (multiple) {
            char* ptr = ofn.lpstrFile;
            ptr[ofn.nFileOffset-1] = 0;
            ptr += ofn.nFileOffset;
            std::string list = ofn.lpstrFile;
            while (*ptr) {    
                list += " ";
                list += ptr;
                ptr += (lstrlen(ptr)+1);
            }
            return list;
        }
        return ofn.lpstrFile;
    }
    else {
        char szFolder[260];
        BROWSEINFO bi = { 0 };
        bi.pszDisplayName = szFolder;
        bi.lpszTitle  = title;
        bi.ulFlags        = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_SHAREABLE;
        LPITEMIDLIST pidl = SHBrowseForFolder (&bi);
        if (pidl && SHGetPathFromIDList(pidl, szFolder))
        return bi.pszDisplayName;
    }
    return "";
}

DiscordState discord_state;
int64_t duration;

int RSGL::discordCreateApplication(discord::ClientId clientID, bool timer/*=false*/) {
    discord::Core* core{}; 
    discord::Core::Create(clientID, DiscordCreateFlags_Default, &core); // Get the application from the `clientID`
    discord_state.core.reset(core);
    core->UserManager().OnCurrentUserUpdate.Connect([]() {
        discord_state.core->UserManager().GetCurrentUser(&discord_state.currentUser); // Get the discord user
    });
    if (timer) {
        auto now = std::chrono::system_clock::now();
        auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
        auto epoch = now_ms.time_since_epoch();
        auto value = std::chrono::duration_cast<std::chrono::seconds>(epoch);
        duration = value.count();
    }
    return 0;
}

int RSGL::discordUpdatePresence(const char* details/*="*/, const char* state/*="*/, const char* largeImage/*="*/, const char* largeText/*="*/, const char* smallImage/*="*/, const char* smallText/*=""*/) {
    discord::Activity activity{};
    if (strcmp(details, "")) activity.SetDetails(details); // We check if the parameters are empty or nah
    if (strcmp(state, "")) activity.SetState(state);
    if (strcmp(largeImage, "")) activity.GetAssets().SetLargeImage(largeImage);
    if (strcmp(largeText, "")) activity.GetAssets().SetLargeText(largeText);
    if (strcmp(smallImage, "")) activity.GetAssets().SetSmallImage(smallImage);
    if (strcmp(smallText, "")) activity.GetAssets().SetSmallText(smallText);
    if (duration != -1) activity.GetTimestamps().SetStart(duration);
    activity.SetType(discord::ActivityType::Playing); // We set the type to g a m i n g

    discord_state.core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {}); // Update
    discord_state.core->RunCallbacks(); // Run it back
    return 0;
}

std::string RSGL::readIniKey(const char* path, std::string Key) {
    char szValue[MAX_PATH];
    char fullFilename[MAX_PATH];
    GetFullPathName(path, MAX_PATH, fullFilename, nullptr);
    
    GetPrivateProfileString(NULL, Key.c_str(), TEXT("ERROR"), szValue, MAX_PATH, fullFilename);
    if ( strcmp(szValue,"ERROR") != 0) { return szValue; }
    return "";
}
int RSGL::writeIniKey(const char* path, std::string Key, std::string value) {
    char fullFilename[MAX_PATH];
    GetFullPathName(path, MAX_PATH, fullFilename, nullptr);
    WritePrivateProfileString (NULL, Key.c_str(), value.c_str(), fullFilename);
    return 0;
}

int RSGL::deleteIniKey(const char* path, std::string Key) {
    char fullFilename[MAX_PATH];
    GetFullPathName(path, MAX_PATH, fullFilename, nullptr);
    WritePrivateProfileString (NULL, Key.c_str(), NULL, fullFilename);
    return 0;
}

int RSGL::deleteIniSection(const char* path) {
    char fullFilename[MAX_PATH];
    GetFullPathName(path, MAX_PATH, fullFilename, nullptr);
    WritePrivateProfileString (NULL, NULL, NULL, fullFilename);
    return 0;
}
bool RSGL::iniKeyExists(const char* path, std::string Key) {
    return readIniKey(path, Key).size() > 0;
}

std::string RSGL::getUsername() {
    char username[257];
    DWORD username_len = 257;
    GetUserName(username, &username_len);
    
    return (std::string)username;
}

int RSGL::getLanguage() {
    return PRIMARYLANGID(GetUserDefaultLCID());
}
std::string RSGL::getClipboardText() {
    OpenClipboard(nullptr);
    HANDLE hData = GetClipboardData(CF_TEXT);
    char* text = static_cast<char*>(GlobalLock(hData));
    std::string txt(text);
    GlobalUnlock(hData);
    CloseClipboard();

    return txt;
}

int RSGL::setClipboardText(std::string txt) {
    HANDLE global = GlobalAlloc(GMEM_FIXED, 32);
    memcpy(global, txt.c_str(), txt.size());

    OpenClipboard(RSGL::win.hwnd);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, global);
    CloseClipboard();

    return 0;
}

int fps_lasttime=0;
int fps_current; 
int fps_frames = 0; 
auto start = std::chrono::system_clock::now();

int RSGL::getFPS() {
    std::chrono::duration<double> elapsed_seconds = std::chrono::system_clock::now()-start;
    fps_frames++;
    if (fps_lasttime < elapsed_seconds.count() - 1.0){
        fps_lasttime = elapsed_seconds.count();
        fps_current = fps_frames;
        fps_frames = 0;
    }
    return fps_current;
}

int RSGL::findNumberAfterDecimalPoint(float num, int howManyKeep/*=-1*/) {
    std::string s;
    std::stringstream out;
    out << num; s = out.str();
    s = s.substr(s.find(".")+1, -1);
    if (howManyKeep != -1) s = s.substr(0, howManyKeep);
    return std::stoi(s);
}

RSGL::timer RSGL::startTimer() {
    auto start = std::chrono::system_clock::now();
    return {0,0,0,0,start};
}

int RSGL::updateTimer(RSGL::timer& t, int howManyKeep/*=-1*/) {
    std::chrono::duration<double> elapsed_seconds = std::chrono::system_clock::now()-t.id;
    int h=0,
        m=0,
        s=0,
        ms=0;
    double time =  elapsed_seconds.count(); 
    ms=RSGL::findNumberAfterDecimalPoint(time, howManyKeep);
    s=time;
    if (m >= 60) { h=m/60;}
    if (time >= 60.0) { m=abs(s)/60; }

    t = {h, m, s-(m*60), ms,t.id};

    return 0;
}

LRESULT CALLBACK WindowProc(HWND h, UINT msg, WPARAM param, LPARAM lparam) {
    switch (msg) {
        case WM_CREATE: { 
         
		    if (RSGL::win.enabled_flags & 64) {
                RSGL::win.biop = new Bitmap_Operations();

		        RSGL::win.biop->Initialize_Buffers(h,1);
		        RSGL::win.biop->Create_Buffer(0);
            }
            else {
            }
            break;
		}
        case WM_SIZE:
		    if (!(RSGL::win.enabled_flags & 64) & !(RSGL::win.enabled_flags & 256))  glViewport(RSGL::win.r.x, RSGL::win.r.y, LOWORD(lparam), HIWORD(lparam));
            break;
        case WM_CLOSE:
		    if (RSGL::win.enabled_flags & 64)  {}
            else if (RSGL::win.enabled_flags & 256) {}
            else {
                wglMakeCurrent(NULL, NULL);
		        wglDeleteContext(glrc);
            }
		    DestroyWindow(h);
		    PostQuitMessage(0);
            break;
    }
    return DefWindowProc(h, msg, param, lparam);
}

int RSGL::window::ProcMSG() {
    MSG msg = {};
    while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE)) {
        switch (msg.message) {
            case WM_QUIT:
            return RSGL::quit;
            case WM_KEYDOWN:
            return RSGL::KeyPressed;
            case WM_KEYUP:
            return RSGL::KeyReleased;
            case WM_MOUSEMOVE:
            return RSGL::MousePosChanged;
            case WM_LBUTTONDOWN:
            return RSGL::MouseButtonPressed;
            case WM_LBUTTONUP:
            return RSGL::MouseButtonReleased;
            case WM_RBUTTONDOWN:
            return RSGL::MouseButtonPressed;
            case WM_RBUTTONUP:
            return RSGL::MouseButtonReleased;
            case WM_MBUTTONDOWN:
            return RSGL::MouseButtonPressed;
            case WM_MBUTTONUP:
            return RSGL::MouseButtonReleased;
            case WM_MOUSEWHEEL:
            return RSGL::win.check;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
  if (nCode >= 0) {
    if (wParam == WM_MOUSEWHEEL) {
        MSLLHOOKSTRUCT *pMhs = (MSLLHOOKSTRUCT *)lParam;
        short zdelta = HIWORD(pMhs->mouseData);
        if (zdelta > 0) RSGL::win.check= 11;
        else RSGL::win.check = 12;
    }
  }
  return CallNextHookEx(0, nCode, wParam, lParam);
}

namespace RSGL{RSGL::drawable win=win;};