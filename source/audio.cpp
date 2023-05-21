/*
* Copyright (c) 2021-23 ColleagueRiley ColleagueRiley@gmail.com
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*
*
*/

#ifdef _WIN32
#ifdef __linux__
#define WINE
#endif
#endif

#ifdef WINE
#include <wine/math.h>

#define MA_NO_WFOPEN

#endif


#ifndef RSGL_H
#include "../RSGL.hpp"
#endif

#define MINIAUDIO_IMPLEMENTATION
#include "deps/miniaudio.h"

namespace RSGL{
    struct audioData {
        ma_decoder decoder;
        ma_result result;
        ma_device device;

        bool free = true;
    };

    void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);
}

void RSGL::audio::play(const char* file){
    if (strlen(file) && !isPlaying()) {
        data = (audioData*)malloc(sizeof(audioData));
        data->free = false;

        this->file = (char*)file;
        data->result = ma_decoder_init_file(file, NULL, &data->decoder);
        
        if (data->result != MA_SUCCESS)
            printf("Could not load file: %s\n", file);

        ma_device_config deviceConfig;
        deviceConfig = ma_device_config_init(ma_device_type_playback);
        deviceConfig.playback.format   = data->decoder.outputFormat;
        deviceConfig.playback.channels = data->decoder.outputChannels;
        deviceConfig.sampleRate        = data->decoder.outputSampleRate;
        deviceConfig.dataCallback      = data_callback;
        deviceConfig.pUserData         = &data->decoder;

        if (ma_device_init(NULL, &deviceConfig, &data->device) != MA_SUCCESS) {
            printf("Failed to open playback device.\n");
            ma_decoder_uninit(&data->decoder);
        }

        if (ma_device_start(&data->device) != MA_SUCCESS) {
            printf("Failed to start playback device.\n");
            ma_device_uninit(&data->device);
            ma_decoder_uninit(&data->decoder);
        }
    }
    
    play();    
}

void RSGL::audio::play(){
    ma_device_start(&data->device);
    if (position() == length())
        seek(0);
}

void RSGL::audio::pause(){
    ma_device_stop(&data->device);
}

void RSGL::audio::seek(int pos){
    data->result = ma_decoder_seek_to_pcm_frame(&data->decoder, pos * 44100);
}

void RSGL::audio::stop(){
    pause();
    ma_device_uninit(&data->device);
    ma_decoder_uninit(&data->decoder);
}


void RSGL::audio::setVolume(int vol){
    data->result = ma_device_set_master_volume(&data->device, vol/100.0f);
}

int RSGL::audio::volume(){
    float volume;
    data->result = ma_device_get_master_volume(&data->device, &volume);
    return volume*100;
}

unsigned int RSGL::audio::position(){
    long long unsigned int pos;
    ma_decoder_get_cursor_in_pcm_frames(&data->decoder, &pos);
    return pos/44100;
}

bool RSGL::audio::isPlaying(){
    return ma_device_is_started(&data->device);
}

int RSGL::audio::length(){
    long long unsigned int len;
    ma_decoder_get_length_in_pcm_frames(&data->decoder, &len);
    return len/44100;
}

RSGL::audio::~audio() {
    if (data->free)
        free(data);
    data->free = true;
}

void RSGL::data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount){    
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if (pDecoder == NULL) {
        return;
    }
    
    ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);
}
