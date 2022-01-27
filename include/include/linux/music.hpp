#pragma once
#include <SFML/Audio.hpp>
#include "rsgl.hpp"

struct music{
    bool isLoaded=false;
    std::string title;
    void loadFile(std::string file);
    void play();
    void stop();
    void pause();
    private:
        sf::Music mus;
};
};

#ifndef Event
struct Event{
  int type;
  int button;
  int x,y;
  void CheckEvents();
};
#endif