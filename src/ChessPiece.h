#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include <SDL3/SDL.h>
#include <string>
#include <vector>
#include <utility> 
#include <array>

class ChessPiece {
public:
    ChessPiece(SDL_Renderer* renderer, const std::string& name, const std::string& texturePath, bool isWhite);
    ~ChessPiece();
    void render(SDL_Renderer* renderer, SDL_FRect destRect);
    const std::string& getName() const;
    bool isWhitePiece() const;
    std::vector<std::pair<int, int>> calculatePossibleMoves(const std::array<std::array<ChessPiece*, 8>, 8>& board, int x, int y);

private:
    std::string name;
    SDL_Texture* texture;
    bool isWhite;
};

#endif // CHESS_PIECE_H
