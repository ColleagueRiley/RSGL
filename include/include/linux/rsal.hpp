#pragma once
#include <iostream>


namespace RSGL{
    struct audio{
        void play(std::string file);
        void play();
        std::string getFile();
        void stop();

        void pause();
        void resume();

        bool isPlaying();
        bool done=false;

        audio(){}

        private:
            std::string File;
            pthread_t t;
        public: audio(std::string file){File=file;}
    };
};