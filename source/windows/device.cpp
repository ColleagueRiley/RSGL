#ifndef RSGL
#include "../../include/include/windows/rsgl.hpp"
#endif

void windowsCMDInfoFetch(const char* cmd, std::string& obj) {
    char filename[1024];
    FILE* f = popen(cmd, "r");
    fgets(filename, 1024, f);
    obj=(std::string)filename;
    fclose(f);
}

RSGL::device_info RSGL::getFullDeviceInfo() {
    // RAM stuff
    MEMORYSTATUSEX memory;
    memory.dwLength = sizeof(memory);
    GlobalMemoryStatusEx(&memory);
    int total=BYTESTOGB(memory.ullTotalPhys);
    int avail = BYTESTOGB(memory.ullAvailPhys);
    std::string os, cpu, cpu_cores, model, pc_name, hdd_total, hdd_free, n64or32questionmark, serial_number, cpu_percentage, cpu_architecture, driver, vram;
    // Gets the OS
    
    windowsCMDInfoFetch("wmic OS get Caption | findstr [0-9] | tr -d '\n'", os); 
    
    windowsCMDInfoFetch("wmic computersystem get model | findstr [0-9] | tr -d '\n'", model); 
    
    windowsCMDInfoFetch("hostname | tr -d '\n'", pc_name); 
    
    windowsCMDInfoFetch("wmic os get serialnumber | findstr [0-9] | tr -d '\n'", serial_number);
    
    windowsCMDInfoFetch("wmic OS get OSArchitecture | findstr [0-9] | tr -d '\n'", n64or32questionmark);
    
    // Get the hard-drive.
    
    windowsCMDInfoFetch("wmic logicaldisk get Size | findstr [0-9] | tr -d '\n'", hdd_total); 
    
    windowsCMDInfoFetch("wmic logicaldisk get FreeSpace | findstr [0-9] | tr -d '\n'", hdd_free);
    // GPU
    const GLubyte* renderer = glGetString(GL_RENDERER); 
    
    windowsCMDInfoFetch("wmic PATH Win32_videocontroller GET driverversion | findstr [0-9] | tr -d '\n'", driver); 
    
    windowsCMDInfoFetch("wmic PATH Win32_videocontroller GET adapterram | findstr [0-9] | tr -d '\n'", vram);
    // CPU
    
    windowsCMDInfoFetch("wmic CPU get Name | findstr [0-9] | tr -d '\n'", cpu);
    
    windowsCMDInfoFetch("wmic CPU get NumberOfCores | findstr [0-9] | tr -d '\n'", cpu_cores); 
    
    windowsCMDInfoFetch("wmic cpu get loadpercentage | findstr [0-9] | tr -d '\n'", cpu_percentage);
    
    windowsCMDInfoFetch("wmic cpu get Architecture | findstr [0-9] | tr -d '\n'", cpu_architecture);

    int hdd_int_total = std::stoi(hdd_total.substr(0,3));
    
    int hdd_int_free = std::stoi(hdd_free.substr(0,3));
    
    switch (std::stoi(cpu_architecture)) {
        case 0:
        cpu_architecture="x86";
        break;
        case 1:
        cpu_architecture="MIPS";
        break;
        case 2:
        cpu_architecture="Alpha";
        break;
        case 3:
        cpu_architecture="PowerPC";
        break;
        case 5:
        cpu_architecture="ARM";
        break;
        case 6:
        cpu_architecture="ia64";
        break;
        case 9:
        cpu_architecture="x64";
        break;
        default:
        cpu_architecture="N/A";
        break;
    }
    
    int i_vram = -1;// BYTESTOMB(std::stoi(vram));
    

    return {
        os, model, pc_name, serial_number,
        hdd_int_total, hdd_int_total-hdd_int_free, hdd_int_free, 
        total, total-avail, avail, (int)memory.dwMemoryLoad, 
        cpu, std::stoi(cpu_cores), std::stoi(cpu_percentage), cpu_architecture,
        (const char*)renderer, driver, i_vram,
        std::stoi(n64or32questionmark.substr(0,2))
    };
}

RSGL::device_info RSGL::getRAMDeviceInfo(bool total/*=true*/, bool available/*=true*/, bool used/*=true*/, bool percentage/*=true*/) {
    // RAM stuff
    MEMORYSTATUSEX memory;
    memory.dwLength = sizeof(memory);
    GlobalMemoryStatusEx(&memory);
    int i_total=0,
        i_avail=0, 
        i_percentage=0,
        i_used=0;

    if (total) i_total=BYTESTOGB(memory.ullTotalPhys);
    if (available) i_avail = BYTESTOGB(memory.ullAvailPhys);
    if (used) i_used = i_total-i_avail;
    if (percentage) i_percentage = (int)memory.dwMemoryLoad;

    return {
        "", "", "", "",
        0, 0, 0, 
        i_total,i_used, i_avail, i_percentage, 
        "", 0, 0, "",
        "", 
        0
    };
}

RSGL::device_info RSGL::getOSDeviceInfo(bool os/*=true*/, bool model/*=true*/, bool pc_name/*=true*/, bool bit/*=true*/, bool serial_number/*=true*/) {
    std::string s_os, s_model, s_pc_name, s_n64or32questionmark, s_serial_number;

    // Gets the OS
    if (os) windowsCMDInfoFetch("wmic OS get Caption | findstr [0-9] | tr -d '\n'", s_os); 
    if (model) windowsCMDInfoFetch("wmic computersystem get model | findstr [0-9] | tr -d '\n'", s_model); 
    if (pc_name) windowsCMDInfoFetch("hostname", s_pc_name); 
    if (bit) windowsCMDInfoFetch("wmic os get serialnumber | findstr [0-9] | tr -d '\n'", s_serial_number);
    if (serial_number) windowsCMDInfoFetch("wmic OS get OSArchitecture | findstr [0-9] | tr -d '\n'", s_n64or32questionmark);

    return {
        s_os, s_model, s_pc_name, s_serial_number,
        0, 0, 0, 
        0, 0, 0, 0, 
        "", 0, 0, "",
        "", "", 0,
        std::stoi(s_n64or32questionmark)
    };
}

RSGL::device_info RSGL::getHDDDeviceInfo(bool total/*=true*/, bool free/*=true*/, bool used/*=true*/) {
    std::string hdd_total, hdd_free;
    
    if (total) windowsCMDInfoFetch("wmic logicaldisk get Size | findstr [0-9] | tr -d '\n'", hdd_total); 
    if (free) windowsCMDInfoFetch("wmic logicaldisk get FreeSpace | findstr [0-9] | tr -d '\n'", hdd_free);

    int hdd_int_total = std::stoi(hdd_total.substr(0,3));
    int hdd_int_free = std::stoi(hdd_free.substr(0,3));
    int hdd_int_used=0;
    if (used) hdd_int_used = hdd_int_total-hdd_int_free;

    return {
        "", "", "", "",
        hdd_int_total, hdd_int_free, hdd_int_used, 
        0, 0, 0, 0, 
        "", 0, 0, "",
        "", "", 0,
        0
    };
}

RSGL::device_info RSGL::getGPUDeviceInfo(bool gpu/*=true*/, bool driver_version/*=true*/, bool vram/*=true*/) {
    const GLubyte* renderer;
    std::string s_driver, s_vram;
    int mb_vram=0;
    
    if (driver_version) windowsCMDInfoFetch("wmic PATH Win32_videocontroller GET driverversion | findstr [0-9] | tr -d '\n'", s_driver); 
    if (vram) {windowsCMDInfoFetch("wmic PATH Win32_videocontroller GET adapterram | findstr [0-9] | tr -d '\n'", s_vram); mb_vram=BYTESTOMB(std::stoi(s_vram));}
    if (gpu) renderer = glGetString(GL_RENDERER); 

    return {
        "", "", "", "",
        0,0,0, 
        0, 0, 0, 0, 
        "", 0, 0, "",
        (const char*)renderer, s_driver, mb_vram,
        0
    };
}

RSGL::device_info RSGL::getCPUDeviceInfo(bool cpu/*=true*/, bool cores/*=true*/, bool percentage/*=true*/, bool architecture/*=true*/) {
    std::string s_cpu, s_cpu_cores, s_cpu_percentage, s_cpu_architecture;

    // CPU
    if (cpu) windowsCMDInfoFetch("wmic CPU get Name | findstr [0-9] | tr -d '\n'", s_cpu);
    if (cores) windowsCMDInfoFetch("wmic CPU get NumberOfCores | findstr [0-9] | tr -d '\n'", s_cpu_cores); 
    if (percentage) windowsCMDInfoFetch("wmic cpu get loadpercentage | findstr [0-9] | tr -d '\n'", s_cpu_percentage);
    if (architecture) {
        windowsCMDInfoFetch("wmic cpu get Architecture | findstr [0-9] | tr -d '\n'", s_cpu_architecture);
        switch (std::stoi(s_cpu_architecture)) {
            case 0: s_cpu_architecture="x86"; break;
            case 1: s_cpu_architecture="MIPS"; break;
            case 2: s_cpu_architecture="Alpha"; break;
            case 3: s_cpu_architecture="PowerPC"; break;
            case 5: s_cpu_architecture="ARM"; break;
            case 6: s_cpu_architecture="ia64"; break;
            case 9: s_cpu_architecture="x64";  break;
            default: s_cpu_architecture="N/A"; break;
        }
    }

    return {
        "", "", "", "",
        0, 0, 0, 
        0, 0, 0, 0, 
        s_cpu, std::stoi(s_cpu_cores), std::stoi(s_cpu_percentage), s_cpu_architecture,
        "", "", 0,
        0
    };
}