#include "GameMenuUI.h"

GameMenuUI::GameMenuUI(SDL_Renderer* renderer, int windowWidth, int windowHeight) : renderer(renderer), selectedButton(0), backgroundTexture(nullptr) {
    font = TTF_OpenFont("font/Clash-Regular.ttf", 24);
    if (!font) {
        SDL_Log("Failed to load font: %s", SDL_GetError());
    }
    SDL_Log("Font for the Menu UI loaded!");

    SDL_Log("Loading background image...");
    backgroundTexture = IMG_LoadTexture(renderer, "img/menubg.png");
    if (!backgroundTexture) {
        SDL_Log("Failed to load background image: %s", SDL_GetError());
    } else {
        SDL_Log("Background image loaded successfully!");
    }

    int buttonWidth = 200;
    int buttonHeight = 50;
    int totalButtonHeight = static_cast<int>(5 * (buttonHeight + 10));
    int startY = (windowHeight - totalButtonHeight) / 2;
    buttons.push_back({
        { static_cast<int>((windowWidth - buttonWidth) / 2), startY, buttonWidth, buttonHeight },
        { (windowWidth - static_cast<float>(buttonWidth)) / 2.0f, static_cast<float>(startY), static_cast<float>(buttonWidth), static_cast<float>(buttonHeight) },
        "Start Game", false, nullptr
    });
    buttons.push_back({
        { static_cast<int>((windowWidth - buttonWidth) / 2), startY + buttonHeight + 10, buttonWidth, buttonHeight },
        { (windowWidth - static_cast<float>(buttonWidth)) / 2.0f, static_cast<float>(startY + buttonHeight + 10), static_cast<float>(buttonWidth), static_cast<float>(buttonHeight) },
        "Load Game", false, nullptr
    });
    buttons.push_back({
        { static_cast<int>((windowWidth - buttonWidth) / 2), startY + 2 * (buttonHeight + 10), buttonWidth, buttonHeight },
        { (windowWidth - static_cast<float>(buttonWidth)) / 2.0f, static_cast<float>(startY + 2 * (buttonHeight + 10)), static_cast<float>(buttonWidth), static_cast<float>(buttonHeight) },
        "Analyze", false, nullptr
    });
    buttons.push_back({
        { static_cast<int>((windowWidth - buttonWidth) / 2), startY + 3 * (buttonHeight + 10), buttonWidth, buttonHeight },
        { (windowWidth - static_cast<float>(buttonWidth)) / 2.0f, static_cast<float>(startY + 3 * (buttonHeight + 10)), static_cast<float>(buttonWidth), static_cast<float>(buttonHeight) },
        "Settings", false, nullptr
    });
    buttons.push_back({
        { static_cast<int>((windowWidth - buttonWidth) / 2), startY + 4 * (buttonHeight + 10), buttonWidth, buttonHeight },
        { (windowWidth - static_cast<float>(buttonWidth)) / 2.0f, static_cast<float>(startY + 4 * (buttonHeight + 10)), static_cast<float>(buttonWidth), static_cast<float>(buttonHeight) },
        "Exit", false, nullptr
    });
    for (Button& button : buttons) {
        createButtonTexture(button);
    }
}


GameMenuUI::~GameMenuUI() {
    SDL_Log("Menu UI Destructor called!");

    if (backgroundTexture) {
        SDL_DestroyTexture(backgroundTexture);
        SDL_Log("Background texture destroyed!");
    }

    if (font) {
        TTF_CloseFont(font);
    }
    SDL_Log("Loaded font at the Menu UI closed!");

    for (Button& button : buttons) {
        if (button.texture) {
            SDL_DestroyTexture(button.texture);
        }
        SDL_Log("Destroyed a button texture!");
    }
}

void GameMenuUI::createButtonTexture(Button& button) {
    button.texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                                       button.frect.w, button.frect.h);
    if (!button.texture) {
        SDL_Log("Failed to create button texture: %s", SDL_GetError());
        return;
    }
    SDL_SetRenderTarget(renderer, button.texture);
    SDL_Color colorTop = button.isHovered ? SDL_Color{255, 200, 100, 255} : SDL_Color{150, 180, 220, 255};
    SDL_Color colorBottom = button.isHovered ? SDL_Color{255, 160, 50, 255} : SDL_Color{100, 140, 200, 255};
    for (int y = 0; y < button.rect.h; ++y) {
        float t = static_cast<float>(y) / button.rect.h;
        SDL_Color blendedColor = {
            static_cast<Uint8>((1 - t) * colorTop.r + t * colorBottom.r),
            static_cast<Uint8>((1 - t) * colorTop.g + t * colorBottom.g),
            static_cast<Uint8>((1 - t) * colorTop.b + t * colorBottom.b),
            255
        };

        SDL_SetRenderDrawColor(renderer, blendedColor.r, blendedColor.g, blendedColor.b, blendedColor.a);
        SDL_FRect line = {0, static_cast<float>(y), button.frect.w, 1};
        SDL_RenderFillRect(renderer, &line);
    }

    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderRect(renderer, &button.frect);

    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, button.label.c_str(), 0, textColor);
    if (textSurface) {
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        SDL_FRect textRect = {
            (button.frect.w - textSurface->w) / 2,
            (button.frect.h - textSurface->h) / 2,
            static_cast<float>(textSurface->w),
            static_cast<float>(textSurface->h)
        };

        SDL_RenderTexture(renderer, textTexture, nullptr, &textRect);
        SDL_DestroyTexture(textTexture);
        SDL_DestroySurface(textSurface);
    }
    SDL_SetRenderTarget(renderer, nullptr);
}


void GameMenuUI::render() {
    if (backgroundTexture) {
        SDL_RenderTexture(renderer, backgroundTexture, nullptr, nullptr); 
    }

    for (Button& button : buttons) {
        if (button.texture) {
            SDL_RenderTexture(renderer, button.texture, nullptr, &button.frect);
        }
    }
}

void GameMenuUI::handleEvents(SDL_Event& event) {
    if (event.type == SDL_EVENT_MOUSE_MOTION) {
        SDL_Point mousePoint = { static_cast<int>(event.motion.x), static_cast<int>(event.motion.y) };
        for (Button& button : buttons) {
            bool wasHovered = button.isHovered;
            button.isHovered = SDL_PointInRect(&mousePoint, &button.rect);
            if (button.isHovered != wasHovered) {
                createButtonTexture(button);
            }
        }
    } else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT) {
        SDL_Point mousePoint = { static_cast<int>(event.button.x), static_cast<int>(event.button.y) };
        for (Button& button : buttons) {
            if (SDL_PointInRect(&mousePoint, &button.rect)) {
                selectedButton = &button - &buttons[0];
                switch (selectedButton) {
                    case 0:
                        SDL_Log("Start Game Selected!");
                        break;
                    case 1:
                        SDL_Log("Load Game Selected!");
                        break;
                    case 2:
                        SDL_Log("Analyze Selected!");
                        break;
                    case 3:
                        SDL_Log("Settings Selected!");
                        break;
                    case 4:
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

