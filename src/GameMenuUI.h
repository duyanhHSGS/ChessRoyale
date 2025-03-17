#ifndef GAME_MENU_UI_H
#define GAME_MENU_UI_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <vector>
#include <string>
#include <cmath>
#include "GameManager.h"

class GameManager;

struct Button {
    SDL_Rect rect;
    SDL_FRect frect;
    std::string label;
    bool isHovered;
    SDL_Texture* texture;
};


class GameMenuUI {
public:
    GameMenuUI(SDL_Renderer* renderer, int windowWidth, int windowHeight, GameManager* manager);
    ~GameMenuUI();
    void render();
    void handleEvents(SDL_Event& event);
private:
	GameManager* manager;
	SDL_Texture* backgroundTexture;
	TTF_Font* font;
    SDL_Renderer* renderer;
    std::vector<Button> buttons;
    int selectedButton;
    void createButtonTexture(Button& button);
    
};

#endif // GAME_MENU_UI_H
