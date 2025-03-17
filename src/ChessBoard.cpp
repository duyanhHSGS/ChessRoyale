#include "ChessBoard.h"
#include <algorithm> 

ChessBoard::ChessBoard(SDL_Renderer* renderer, int windowWidth, int windowHeight)
    : renderer(renderer), chessboardTexture(nullptr), selectedPiece(nullptr),
      windowWidth(windowWidth), windowHeight(windowHeight), selectedX(-1), selectedY(-1),
      dragOffsetX(0.0f), dragOffsetY(0.0f) {
    initializeBoard();
    preRenderChessboard();
}

ChessBoard::~ChessBoard() {
    if (chessboardTexture) {
        SDL_DestroyTexture(chessboardTexture);
        chessboardTexture = nullptr;
    }
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            if (board[y][x] != nullptr) {
                delete board[y][x];
                board[y][x] = nullptr;
            }
        }
    }
}

void ChessBoard::initializeBoard() {
    std::string basePath = "img/pieces/";

    board = {{
        {new ChessPiece(renderer, "brook", basePath + "brook.png", false), 
         new ChessPiece(renderer, "bknight", basePath + "bknight.png", false), 
         new ChessPiece(renderer, "bbishop", basePath + "bbishop.png", false), 
         new ChessPiece(renderer, "bqueen", basePath + "bqueen.png", false), 
         new ChessPiece(renderer, "bking", basePath + "bking.png", false), 
         new ChessPiece(renderer, "bbishop", basePath + "bbishop.png", false), 
         new ChessPiece(renderer, "bknight", basePath + "bknight.png", false), 
         new ChessPiece(renderer, "brook", basePath + "brook.png", false)},

        {new ChessPiece(renderer, "bpawn", basePath + "bpawn.png", false), 
         new ChessPiece(renderer, "bpawn", basePath + "bpawn.png", false), 
         new ChessPiece(renderer, "bpawn", basePath + "bpawn.png", false), 
         new ChessPiece(renderer, "bpawn", basePath + "bpawn.png", false), 
         new ChessPiece(renderer, "bpawn", basePath + "bpawn.png", false), 
         new ChessPiece(renderer, "bpawn", basePath + "bpawn.png", false), 
         new ChessPiece(renderer, "bpawn", basePath + "bpawn.png", false), 
         new ChessPiece(renderer, "bpawn", basePath + "bpawn.png", false)},

        {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
        {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
        {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
        {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

        {new ChessPiece(renderer, "wpawn", basePath + "wpawn.png", true), 
         new ChessPiece(renderer, "wpawn", basePath + "wpawn.png", true), 
         new ChessPiece(renderer, "wpawn", basePath + "wpawn.png", true), 
         new ChessPiece(renderer, "wpawn", basePath + "wpawn.png", true), 
         new ChessPiece(renderer, "wpawn", basePath + "wpawn.png", true), 
         new ChessPiece(renderer, "wpawn", basePath + "wpawn.png", true), 
         new ChessPiece(renderer, "wpawn", basePath + "wpawn.png", true), 
         new ChessPiece(renderer, "wpawn", basePath + "wpawn.png", true)},

        {new ChessPiece(renderer, "wrook", basePath + "wrook.png", true), 
         new ChessPiece(renderer, "wknight", basePath + "wknight.png", true), 
         new ChessPiece(renderer, "wbishop", basePath + "wbishop.png", true), 
         new ChessPiece(renderer, "wqueen", basePath + "wqueen.png", true), 
         new ChessPiece(renderer, "wking", basePath + "wking.png", true), 
         new ChessPiece(renderer, "wbishop", basePath + "wbishop.png", true), 
         new ChessPiece(renderer, "wknight", basePath + "wknight.png", true), 
         new ChessPiece(renderer, "wrook", basePath + "wrook.png", true)}
    }};
}


void ChessBoard::preRenderChessboard() {
    chessboardTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, windowWidth, windowHeight);
    SDL_SetRenderTarget(renderer, chessboardTexture);

    float squareSize = static_cast<float>(windowHeight) / 8.0f;
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            bool isWhite = (x + y) % 2 == 0;
            renderSquare(x * squareSize, y * squareSize, squareSize, isWhite);
        }
    }

    SDL_SetRenderTarget(renderer, nullptr);
    boardRect = {0.0f, 0.0f, static_cast<float>(windowWidth), static_cast<float>(windowHeight)};
}

void ChessBoard::handleEvents(SDL_Event& event) {
    float squareSize = static_cast<float>(windowHeight) / 8.0f;
    float mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    int hoverX = static_cast<int>(mouseX / squareSize);
    int hoverY = static_cast<int>(mouseY / squareSize);

    if (event.type == SDL_EVENT_MOUSE_MOTION) {
        if (hoverX >= 0 && hoverX < 8 && hoverY >= 0 && hoverY < 8 && board[hoverY][hoverX] != nullptr && selectedPiece == nullptr) {
            calculatePossibleMoves(hoverX, hoverY);
        }
    } else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT) {
        if (hoverX >= 0 && hoverX < 8 && hoverY >= 0 && hoverY < 8 && board[hoverY][hoverX] != nullptr) {
            selectedPiece = board[hoverY][hoverX];
            selectedX = hoverX;
            selectedY = hoverY;
            dragOffsetX = mouseX - (hoverX * squareSize);
            dragOffsetY = mouseY - (hoverY * squareSize);
        }
    } else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP && event.button.button == SDL_BUTTON_LEFT) {
        if (selectedPiece != nullptr) {
            int dropX = static_cast<int>(mouseX / squareSize);
            int dropY = static_cast<int>(mouseY / squareSize);

            if (std::find(possibleMoves.begin(), possibleMoves.end(), std::make_pair(dropX, dropY)) != possibleMoves.end()) {
                board[dropY][dropX] = selectedPiece;
                board[selectedY][selectedX] = nullptr;
            } else {
                board[selectedY][selectedX] = selectedPiece;
            }
            selectedPiece = nullptr;
            possibleMoves.clear();
        }
    }
}

void ChessBoard::render() {
    SDL_RenderTexture(renderer, chessboardTexture, nullptr, &boardRect);
    renderHighlightedMoves();

    float squareSize = static_cast<float>(windowHeight) / 8.0f;
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            if (board[y][x] != nullptr && board[y][x] != selectedPiece) {
                SDL_FRect destRect = {x * squareSize, y * squareSize, squareSize, squareSize};
                board[y][x]->render(renderer, destRect);
            }
        }
    }

    if (selectedPiece != nullptr) {
        float mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_FRect destRect = {mouseX - dragOffsetX, mouseY - dragOffsetY, squareSize, squareSize};
        selectedPiece->render(renderer, destRect);
    }
}

void ChessBoard::renderSquare(float x, float y, float size, bool isWhite) {
    SDL_SetRenderDrawColor(renderer, isWhite ? 240 : 80, isWhite ? 240 : 80, isWhite ? 240 : 80, 255);
    SDL_FRect square = {x, y, size, size};
    SDL_RenderFillRect(renderer, &square);
}

void ChessBoard::renderHighlightedMoves() {
    float squareSize = static_cast<float>(windowHeight) / 8.0f;

    for (const std::pair<int, int>& move : possibleMoves) {
        int x = move.first;
        int y = move.second;

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 128); 
        SDL_FRect highlightRect = {x * squareSize, y * squareSize, squareSize, squareSize};
        SDL_RenderFillRect(renderer, &highlightRect);
    }
}

void ChessBoard::calculatePossibleMoves(int x, int y) {
    possibleMoves.clear();
    if (board[y][x] != nullptr) {
        possibleMoves = board[y][x]->calculatePossibleMoves(board, x, y);
    }
}


void ChessBoard::update() {
	
}
