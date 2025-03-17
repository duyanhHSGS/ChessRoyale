#include "ChessPiece.h"
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL_log.h>

ChessPiece::ChessPiece(SDL_Renderer* renderer, const std::string& name, const std::string& texturePath, bool isWhite)
	: name(name), texture(nullptr), isWhite(isWhite) {
	texture = IMG_LoadTexture(renderer, texturePath.c_str());
	if (!texture) {
		SDL_Log("Failed to load texture for %s: %s", name.c_str(), SDL_GetError());
	}
}

ChessPiece::~ChessPiece() {
	if (texture) {
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}
}

void ChessPiece::render(SDL_Renderer* renderer, SDL_FRect destRect) {
	if (texture) {
		SDL_RenderTexture(renderer, texture, nullptr, &destRect);
	}
}

const std::string& ChessPiece::getName() const {
	return name;
}

bool ChessPiece::isWhitePiece() const {
	return isWhite;
}

std::vector<std::pair<int, int>> ChessPiece::calculatePossibleMoves(const std::array<std::array<ChessPiece*, 8>, 8>& board, int x, int y) {
	std::vector<std::pair<int, int>> possibleMoves;

	if (name == "wpawn" || name == "bpawn") {
		int direction = (isWhite ? -1 : 1);
		int startRow = (isWhite ? 6 : 1);

		if (y + direction >= 0 && y + direction < 8 && board[y + direction][x] == nullptr) {
			possibleMoves.emplace_back(x, y + direction);
		}

		if (y == startRow && board[y + direction][x] == nullptr && board[y + 2 * direction][x] == nullptr) {
			possibleMoves.emplace_back(x, y + 2 * direction);
		}

		if (x - 1 >= 0 && y + direction >= 0 && y + direction < 8 && board[y + direction][x - 1] != nullptr) {
			if (board[y + direction][x - 1]->isWhitePiece() != isWhite) {
				possibleMoves.emplace_back(x - 1, y + direction);
			}
		}
		if (x + 1 < 8 && y + direction >= 0 && y + direction < 8 && board[y + direction][x + 1] != nullptr) {
			if (board[y + direction][x + 1]->isWhitePiece() != isWhite) {
				possibleMoves.emplace_back(x + 1, y + direction);
			}
		}
	}

	if (name == "wknight" || name == "bknight") {
		std::vector<std::pair<int, int>> knightMoves = {
			{x + 1, y + 2}, {x - 1, y + 2}, {x + 1, y - 2}, {x - 1, y - 2},
			{x + 2, y + 1}, {x - 2, y + 1}, {x + 2, y - 1}, {x - 2, y - 1}
		};
		for (const auto& move : knightMoves) {
			int newX = move.first;
			int newY = move.second;
			if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
				if (board[newY][newX] == nullptr || board[newY][newX]->isWhitePiece() != isWhite) {
					possibleMoves.emplace_back(newX, newY);
				}
			}
		}
	}

	if (name == "wrook" || name == "brook" || name == "wqueen" || name == "bqueen") {
		const std::vector<std::pair<int, int>> directions = {
			{0, 1}, {0, -1}, {1, 0}, {-1, 0}
		};

		for (const auto& direction : directions) {
			int dx = direction.first, dy = direction.second;
			int newX = x + dx, newY = y + dy;
			while (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
				if (board[newY][newX] == nullptr) {
					possibleMoves.emplace_back(newX, newY);
				} else {
					if (board[newY][newX]->isWhitePiece() != isWhite) {
						possibleMoves.emplace_back(newX, newY);
					}
					break;
				}
				newX += dx;
				newY += dy;
			}
		}
	}

	if (name == "wbishop" || name == "bbishop" || name == "wqueen" || name == "bqueen") {
		const std::vector<std::pair<int, int>> directions = {
			{1, 1}, {-1, 1}, {1, -1}, {-1, -1}
		};

		for (const auto& direction : directions) {
			int dx = direction.first, dy = direction.second;
			int newX = x + dx, newY = y + dy;
			while (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
				if (board[newY][newX] == nullptr) {
					possibleMoves.emplace_back(newX, newY);
				} else {
					if (board[newY][newX]->isWhitePiece() != isWhite) {
						possibleMoves.emplace_back(newX, newY);
					}
					break;
				}
				newX += dx;
				newY += dy;
			}
		}
	}

	if (name == "wking" || name == "bking") {
		const std::vector<std::pair<int, int>> directions = {
			{0, 1}, {0, -1}, {1, 0}, {-1, 0},
			{1, 1}, {-1, 1}, {1, -1}, {-1, -1}
		};
		for (const auto& direction : directions) {
			int newX = x + direction.first, newY = y + direction.second;
			if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
				if (board[newY][newX] == nullptr || board[newY][newX]->isWhitePiece() != isWhite) {
					possibleMoves.emplace_back(newX, newY);
				}
			}
		}
	}

	return possibleMoves;
}
