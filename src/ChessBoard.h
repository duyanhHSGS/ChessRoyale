#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <SDL3/SDL.h>
#include <array>
#include <vector>
#include "ChessPiece.h"

class ChessBoard {
public:
    ChessBoard(SDL_Renderer* renderer, int windowWidth, int windowHeight);
    ~ChessBoard();

    void handleEvents(SDL_Event& event);
    void render();
    void update();

private:
    SDL_Renderer* renderer;
    SDL_Texture* chessboardTexture;
    SDL_FRect boardRect;
    int windowWidth;
    int windowHeight;

    std::array<std::array<ChessPiece*, 8>, 8> board;
    ChessPiece* selectedPiece;
    int selectedX, selectedY;
    float dragOffsetX, dragOffsetY;

    std::vector<std::pair<int, int>> possibleMoves;

    void initializeBoard();
    void preRenderChessboard();
    void renderSquare(float x, float y, float size, bool isWhite);
    void renderHighlightedMoves();
    void calculatePossibleMoves(int x, int y);
};

#endif // CHESS_BOARD_H
