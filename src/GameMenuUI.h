#ifndef GAME_MENU_UI_H
#define GAME_MENU_UI_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include <string>

struct Button {
    SDL_Rect rect;
    SDL_FRect frect;
    std::string label;
    bool isHovered;
};

class GameMenuUI {
public:
    GameMenuUI(SDL_Renderer* renderer);
    ~GameMenuUI();
    void render();
    void handleEvents(SDL_Event& event);
private:
	TTF_Font* font;
    SDL_Renderer* renderer;
    std::vector<Button> buttons;
    int selectedButton;
    void drawButton(const Button& button);
};

#endif // GAME_MENU_UI_H
