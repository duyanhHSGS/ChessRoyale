#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "GameMenuUI.h"
#include "BattleUI.h"

class GameMenuUI;
class BattleUI;

enum struct GameState {
    MainMenu,
    BattleScreen,
    SettingsScreen,
    AnalyzeScreen,
    Exit
};

class GameManager {
public:
    GameManager(SDL_Renderer* renderer, const int windowWidth, const int windowHeight);
    ~GameManager();
    void setState(GameState newState);
    GameState getState() const;
    void handleEvents(SDL_Event& event);
    void update();
    void render();
    void clean();
    void createBattle();
private:
	SDL_Renderer* renderer;
    GameState currentState;
    GameMenuUI* menuUI;
    BattleUI* battleUI;
    int windowWidth;
    int windowHeight;
};

#endif // GAME_MANAGER_H
