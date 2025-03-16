#include "App.h"

GameMenuUI* menuUI = nullptr;
App::App()
	: window(nullptr), renderer(nullptr),
	  isRunning(false), windowWidth(0), windowHeight(0) {}

App::~App() {
	SDL_Log("App Destructor has just been called!\n");
}

int App::getWindowWidth() {
	return windowWidth;
}
int App::getWindowHeight() {
	return windowHeight;
}

bool App::running() {
	return isRunning;
}

bool App::init(const std::string& title, int xpos, int ypos, int windowWidth, int windowHeight, SDL_WindowFlags flags) {
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
// init the sdl
	if (!SDL_Init( SDL_INIT_VIDEO )) {
		SDL_Log( "SDL could not initialize! SDL error: %s\n", SDL_GetError() );
		SDL_Quit();
		return false;
	}
	SDL_Log( "SDL initiated! \n" );
// init font
	if (!TTF_Init()) {
		SDL_Log( "TTF could not initialize!: SDL error: %s\n", SDL_GetError() );
		SDL_Quit();
		return false;
	}
	SDL_Log( "TTF initiated! \n" );
// init window
	SDL_Log( "Initiating Window... \n" );
	SDL_Log( "Creating Window... \n" );
	window = SDL_CreateWindow("Chess Royale", windowWidth, windowHeight, flags);
	if (window == nullptr) {
		SDL_Log( "Window could not initialize! SDL error: %s\n", SDL_GetError() );
		SDL_Quit();
		return false;
	}
	SDL_Log( "Window created! \n" );
	if (!SDL_SetWindowPosition(window, xpos, ypos)) {
		SDL_Log( "Window's position failed to set! SDL error: %s\n", SDL_GetError() );
		return false;
	}
	SDL_Log( "Window's position set! \n" );
	SDL_Log( "Window initiated! \n" );
// init renderer
	SDL_Log("Creating Renderer...\n");
	SDL_Log("Selecting renderer driver automatically...\n");
	renderer = SDL_CreateRenderer(window, NULL);
	if (!renderer) {
		SDL_Log("Renderer failed to create! SDL error: %s\n", SDL_GetError());
		return false;
	}
	SDL_Log("Renderer created!");
	const char* currentRendererName = SDL_GetRendererName(renderer);
	if (currentRendererName) {
		SDL_Log("Renderer in use: %s\n", currentRendererName);
	} else {
		SDL_Log("Failed to retrieve renderer name! SDL error: %s\n", SDL_GetError());
	}
	int numRenderDrivers = SDL_GetNumRenderDrivers();
	SDL_Log("Available rendering drivers:");
	for (int i = 0; i < numRenderDrivers; ++i) {
		const char* driverName = SDL_GetRenderDriver(i);
		SDL_Log(" - Renderer %d: %s", i, driverName);
	}
//init the menu UI
	menuUI = new GameMenuUI(renderer, windowWidth, windowHeight);
// running = on
	isRunning = true;
	return true;
}

float mx=0, my=0; //mouse pos
void App::handleEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            isRunning = false;
        }
        if (menuUI) {
            menuUI->handleEvents(event); 
        }
    }
//    SDL_GetMouseState(&mx, &my);
//    SDL_Log("Mouse Position: (%f, %f)", mx, my);
}



void App::update() {
	
}

void App::render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); 
	SDL_RenderClear(renderer);
	menuUI->render();
	
	SDL_RenderPresent(renderer);
}

void App::clean() {
	if (renderer) {
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
		SDL_Log("Renderer destroyed.\n");
	}
	if (window) {
		SDL_DestroyWindow(window);
		window = nullptr;
		SDL_Log("Window destroyed.\n");
	}
	if (menuUI) {
		delete menuUI;
		menuUI = nullptr;
		SDL_Log("Menu destroyed.\n");
	}
	TTF_Quit();
	SDL_Log("TTF quit.\n");
	SDL_Quit();
	SDL_Log("SDL subsystem quit.\n");
	SDL_Log("App cleaned up successfully.\n");
}