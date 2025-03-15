#include "GameMenuUI.h"

GameMenuUI::GameMenuUI(SDL_Renderer* renderer) : renderer(renderer), selectedButton(0) {
	font = TTF_OpenFont("font/Clash-Regular.ttf", 24);
	if (!font) {
		SDL_Log("Failed to load font: %s", SDL_GetError());
	}
	SDL_Log("Font for the Menu UI loaded!");
	buttons.push_back({ {300, 200, 200, 50}, {300.0f, 200.0f, 200.0f, 50.0f},"Start Game", false });
	buttons.push_back({ {300, 300, 200, 50}, {300.0f, 300.0f, 200.0f, 50.0f}, "Load Game", false });
	buttons.push_back({ {300, 400, 200, 50}, {300.0f, 400.0f, 200.0f, 50.0f}, "Exit", false });

}

GameMenuUI::~GameMenuUI() {
	SDL_Log("Menu UI Destructor called!");
	if (font) {
		TTF_CloseFont(font);
	}
	SDL_Log("Loaded font at the Menu UI closed!");
}

void GameMenuUI::drawButton(const Button& button) {
    // Set the draw color and render the button rectangle
    SDL_SetRenderDrawColor(renderer, button.isHovered ? 200 : 100, 100, 200, 255);
    SDL_RenderFillRect(renderer, &button.frect);

    // Render the button label
    SDL_Color textColor = { 255, 255, 255, 255 }; // White text
    size_t textLength = button.label.length();    // Get the length of the label text
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, button.label.c_str(), textLength, textColor);

    if (textSurface) {
        // Create a texture from the surface
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        // Set the destination rectangle for the text
        SDL_FRect textRect = {
            button.frect.x + 10, 
            button.frect.y + 10, 
            static_cast<float>(textSurface->w), 
            static_cast<float>(textSurface->h)
        };

        // Render the text texture
        SDL_RenderTexture(renderer, textTexture, nullptr, &textRect);

        // Free resources
        SDL_DestroySurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }
}



void GameMenuUI::render() {
	for (Button& button : buttons) {
		drawButton(button);
	}
}

void GameMenuUI::handleEvents(SDL_Event& event) {
	if (event.type == SDL_EVENT_MOUSE_MOTION) {
		SDL_Point mousePoint = {  static_cast<int>(event.motion.x),  static_cast<int>(event.motion.y) };
		for (std::vector<Button>::iterator it = buttons.begin(); it != buttons.end(); ++it) {
			it->isHovered = SDL_PointInRect(&mousePoint, &it->rect);
		}
	} else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT) {
		SDL_Point mousePoint = {  static_cast<int>(event.button.x),  static_cast<int>(event.button.y) };
		for (std::vector<Button>::iterator it = buttons.begin(); it != buttons.end(); ++it) {
			if (SDL_PointInRect(&mousePoint, &it->rect)) {
				selectedButton = std::distance(buttons.begin(), it);
				switch (selectedButton) {
					case 0:
						SDL_Log("Start Game Selected!");
						break;
					case 1:
						SDL_Log("Load Game Selected!");
						break;
					case 2:
						SDL_Log("Exit Selected!");
						break;
					default:
						SDL_Log("Unknown Button Selected!");
						break;
				}
			}
		}
	}
}

