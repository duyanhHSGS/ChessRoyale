#ifndef BATTLE_UI_H
#define BATTLE_UI_H

#include "ChessBoard.h"
#include <SDL3/SDL.h>
#include <vector>
#include <string>

class ChessBoard; 

class BattleUI {
public:
    BattleUI(SDL_Renderer* renderer, int windowWidth, int windowHeight);
    ~BattleUI();

    void handleEvents(SDL_Event& event);
    void render();
    void update();

private:
    SDL_Renderer* renderer;
    ChessBoard* chessBoard; 
    int windowWidth;
    int windowHeight;
};

#endif // BATTLE_UI_H
