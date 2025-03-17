#ifndef APP_H
#define APP_H

#include "GameManager.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <string>

class App {
public:
    App();
    ~App();
    // Initialization: title, x position, y position, width, height, window flag
    bool init(const std::string&, int xpos, int ypos, int width, int height, SDL_WindowFlags flag);
    void handleEvents();
    void update();
    void render();
    void clean();
    bool running();
    int getWindowWidth();
    int getWindowHeight();
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool isRunning;
    int windowWidth,windowHeight;
    GameManager* manager;
};

#endif // App_H
