#define DEBUG_ENABLED            1
#define XINPUT_ENABLED           2
#define FULLSCREEN_ENABLED       4
#define WINDOW_MAXIMIZED         8
#define WINDOW_MINIMIZED         16
#define ENABLE_DPI_SCALING       32
#define GDI_RENDERING            64
#define WINDOW_CENTRALIZED       128
#define DISABLE_WINDOW_SCALING   256
#define DIRECTX_RENDERING        512

#define RK_Q                     0x51
#define RK_W                     0x57
#define RK_E                     0x45
#define RK_R                     0x52
#define RK_T                     0x54
#define RK_Y                     0x59
#define RK_U                     0x55
#define RK_I                     0x49
#define RK_O                     0x4F
#define RK_P                     0x50
#define RK_A                     0X41
#define RK_S                     0x53
#define RK_D                     0x44
#define RK_F                     0x46
#define RK_G                     0x47
#define RK_H                     0x48
#define RK_J                     0x4A
#define RK_K                     0x4B
#define RK_L                     0x4C
#define RK_Z                     0x5A
#define RK_X                     0x58
#define RK_C                     0x43
#define RK_V                     0x56
#define RK_B                     0x42
#define RK_N                     0x4E
#define RK_M                     0x4D

#define RK_0                     0x30
#define RK_1                     0x31
#define RK_2                     0x32
#define RK_3                     0x33
#define RK_4                     0x34
#define RK_5                     0x35
#define RK_6                     0x36
#define RK_7                     0x37
#define RK_8                     0x38
#define RK_9                     0x39

#define RK_NUMPAD_0              0x60
#define RK_NUMPAD_1              0x61
#define RK_NUMPAD_2              0x62
#define RK_NUMPAD_3              0x63
#define RK_NUMPAD_4              0x64
#define RK_NUMPAD_5              0x65
#define RK_NUMPAD_6              0x66
#define RK_NUMPAD_7              0x67
#define RK_NUMPAD_8              0x68
#define RK_NUMPAD_9              0x69

#define RK_Up                    0x26
#define RK_Down                  0x28
#define RK_Left                  0x25
#define RK_Right                 0x27

#define RK_Esc                   0x1b
#define RK_ESC                   0x1b
#define RK_Space                 0x20
#define RK_Backspace             0x08
#define RK_Tab                   0x09
#define RK_Clear                 0x0C
#define RK_Return                0x0D
#define RK_Enter                 0x0D

#define RK_CTRL                  0x11
#define RK_Control               0x11
#define RK_L_CTRL                0xA2
#define RK_R_CTRL                0xA3
#define RK_L_Control             0xA2
#define RK_R_Control             0xA3

#define RK_Shift                 0x10
#define RK_L_Shift               0xA0
#define RK_R_Shift               0xA1

#define RK_F1                    0x70
#define RK_F2                    0x71
#define RK_F3                    0x72
#define RK_F4                    0x73
#define RK_F5                    0x74
#define RK_F6                    0x75
#define RK_F7                    0x76
#define RK_F8                    0x77
#define RK_F9                    0x78
#define RK_F10                   0x79
#define RK_F11                   0x7A
#define RK_F12                   0x7B
#define RK_F13                   0x7C
#define RK_F14                   0x7D
#define RK_F15                   0x7E
#define RK_F16                   0x7F
#define RK_F17                   0x80
#define RK_F18                   0x81
#define RK_F19                   0x82
#define RK_F20                   0x83
#define RK_F21                   0x84
#define RK_F22                   0x85
#define RK_F23                   0x86
#define RK_F24                   0x87


#define OK                       MB_OK                  
#define HELP                     MB_HELP                
#define OKCANCEL                 MB_OKCANCEL            
#define RETRYCANCEL              MB_RETRYCANCEL         
#define YESNOCANCEL              MB_YESNOCANCEL         
#define YESNO                    MB_YESNO               
#define CANCELTRYCONTINUE        MB_CANCELTRYCONTINUE 

#define RK_PAD_A                 XINPUT_GAMEPAD_A+100              
#define RK_PAD_B                 XINPUT_GAMEPAD_B+100              
#define RK_PAD_X                 XINPUT_GAMEPAD_X+100              
#define RK_PAD_Y                 XINPUT_GAMEPAD_Y+100              
   
#define RK_PAD_L                 XINPUT_GAMEPAD_LEFT_SHOULDER+100    
#define RK_PAD_R                 XINPUT_GAMEPAD_RIGHT_SHOULDER+100     
#define RK_PAD_LT                0x5806+100       
#define RK_PAD_RT                0x5807+100       
   
#define RK_PAD_UP                XINPUT_GAMEPAD_DPAD_UP+100        
#define RK_PAD_DOWN              XINPUT_GAMEPAD_DPAD_DOWN+100      
#define RK_PAD_LEFT              XINPUT_GAMEPAD_DPAD_LEFT+100      
#define RK_PAD_RIGHT             XINPUT_GAMEPAD_DPAD_RIGHT+100     
   
#define RK_PAD_START             XINPUT_GAMEPAD_START+100          
#define RK_PAD_BACK              XINPUT_GAMEPAD_BACK+100           
   
#define RK_PAD_LSTICK_PRESS      XINPUT_GAMEPAD_LEFT_THUMB+100    
#define RK_PAD_RSTICK_PRESS      XINPUT_GAMEPAD_RIGHT_THUMB+100   
   
#define RK_PAD_LSTICK_UP         0x5820+100      
#define RK_PAD_LSTICK_DOWN       0x5821+100
#define RK_PAD_LSTICK_RIGHT      0x5822+100
#define RK_PAD_LSTICK_LEFT       0x5823+100
   
#define RK_PAD_LSTICK_UPLEFT     0x5824+100
#define RK_PAD_LSTICK_UPRIGHT    0x5825+100
#define RK_PAD_LSTICK_DOWNRIGHT  0x5826+100
#define RK_PAD_LSTICK_DOWNLEFT   0x5827+100

#define RK_PAD_RSTICK_UP         0x5830+100     
#define RK_PAD_RSTICK_DOWN       0x5831+100  
#define RK_PAD_RSTICK_RIGHT      0x5832+100  
#define RK_PAD_RSTICK_LEFT       0x5833+100  

#define RK_PAD_RSTICK_UPLEFT     0x5834+100
#define RK_PAD_RSTICK_UPRIGHT    0x5835+100
#define RK_PAD_RSTICK_DOWNRIGHT  0x5836+100
#define RK_PAD_RSTICK_DOWNLEFT   0x5837+100

#define l_stick_x                pad_x                    
#define l_stick_y                pad_y
#define cpad_x                   pad_x 
#define cpad_y                   pad_y

#define r_stick_x                r_pad_x 
#define r_stick_y                r_pad_y
#define r_cpad_x                 r_pad_x 
#define r_cpad_y                 r_pad_y

#define triggers                 point
#define battery                  point
#define screen                   point