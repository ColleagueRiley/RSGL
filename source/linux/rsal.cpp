#include "../RSAL"
#include <pthread.h>
#include <string.h>
#include "deps/backend.hpp"

void* RAWPTHREADRSALplayAudio(void* ap){
    RSAL::audio* a = (RSAL::audio*)ap;
    if (a->getFile().size() > 0){ 
        a->done=false; std::string file = a->getFile();
    	std::string ext = file; 
        ext.replace(ext.begin(),ext.begin()+file.find_last_of('.'),"");
        if (ext == ".mp3"){ 
            std::string wav = file+".wav";
            MP3toWAV(file,wav); file=wav;
        }
        playWav(file,&a->Pause,ext == ".mp3",&a->Seek);
    } a->done=true;
    return NULL;
}

std::string RSAL::audio::getFile(){return File;}

bool RSGL::audio::isPlaying(){return !done;}

void RSGL::audio::play(std::string file){
    File=file; done=false;
    pthread_create(&t,NULL,RAWPTHREADRSALplayAudio,this);
}  

void RSGL::audio::play(){
    if (File.size() > 0){
        done=false;
        pthread_create(&t,NULL,RAWPTHREADRSALplayAudio,this); 
    }
}   

void RSGL::audio::seek(int seconds){ Seek=seconds; }

void RSGL::audio::stop(){ pthread_cancel(t); done=true; }   
void RSGL::audio::pause(){ Pause=true; }
void RSGL::audio::resume(){ Pause=false; }
bool RSGL::audio::isPaused(){ return Pause; }