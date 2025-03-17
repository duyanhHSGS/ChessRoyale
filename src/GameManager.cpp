#include "GameManager.h"

GameManager::GameManager(SDL_Renderer* renderer, const int windowWidth, const int windowHeight) :
    currentState(GameState::MainMenu), menuUI(nullptr), battleUI(nullptr),
    windowWidth(windowWidth), windowHeight(windowHeight), renderer(renderer) {
    menuUI = new GameMenuUI(renderer, windowWidth, windowHeight, this);
}

GameManager::~GameManager() {
}

void GameManager::setState(GameState newState) {
    currentState = newState;
    if (newState == GameState::BattleScreen) {
        if (!battleUI) {
            createBattle();
        }
    }
}

GameState GameManager::getState() const {
    return currentState;
}

void GameManager::handleEvents(SDL_Event& event) {
    switch (currentState) {
        case GameState::MainMenu:
            if (menuUI) {
                menuUI->handleEvents(event); 
            }
            break;
        case GameState::BattleScreen:
            if (battleUI) {
                battleUI->handleEvents(event);
            }
            break;
        case GameState::SettingsScreen:
            break;
        case GameState::AnalyzeScreen:
            break;
        case GameState::Exit:
            break;
        default:
            break;
    }
}

void GameManager::update() {
    switch (currentState) {
        case GameState::MainMenu:
            break;
        case GameState::BattleScreen:
            if (battleUI) {
                battleUI->update();
            }
            break;
        default:
            break;
    }
}

void GameManager::render() {
    switch (currentState) {
        case GameState::MainMenu:
            if (menuUI) {
                menuUI->render();
            }
            break;
        case GameState::BattleScreen:
            if (battleUI) {
                battleUI->render();
            }
            break;
        default:
            break;
    }
}

void GameManager::clean() {
    if (menuUI) {
        delete menuUI;
        menuUI = nullptr;
    }
    if (battleUI) {
        delete battleUI;
        battleUI = nullptr;
    }
}

void GameManager::createBattle() {
	if (battleUI) {
        delete battleUI;
        battleUI = nullptr;
    }
    battleUI = new BattleUI(renderer,windowWidth,windowHeight);
}
