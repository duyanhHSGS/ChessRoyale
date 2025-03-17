#include "BattleUI.h"

BattleUI::BattleUI(SDL_Renderer* renderer, int windowWidth, int windowHeight)
    : renderer(renderer), windowWidth(windowWidth), windowHeight(windowHeight) {
    chessBoard = new ChessBoard(renderer, windowWidth, windowHeight);
}

BattleUI::~BattleUI() {
    if (chessBoard) {
        delete chessBoard;
        chessBoard = nullptr;
    }
}

void BattleUI::handleEvents(SDL_Event& event) {
    if (chessBoard) {
        chessBoard->handleEvents(event);
    }
}

void BattleUI::render() {
    if (chessBoard) {
        chessBoard->render();
    }
}

void BattleUI::update() {
    if (chessBoard) {
        chessBoard->update();
    }
}
