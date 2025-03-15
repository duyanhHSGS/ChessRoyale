#include"App.h"

App* app = nullptr;
constexpr int fps = 60;
constexpr int frameDelay = 1000 / fps;
constexpr SDL_WindowFlags flags = SDL_WINDOW_RESIZABLE;


int main(int argc, char* argv[]) {
	Uint32 frameStart {0};
	int frameTime {0};

	app = new App();
	if (!app->init("Chess Royale", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, flags)) {
		SDL_Log( "App Initialization Failed! \n" );
		return 1;
	}
	SDL_Log( "App Initiated! \n" );

	while (app->running()) {
		frameStart = SDL_GetTicks();
		app->handleEvents();
		app->update();
		app->render();
		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime) SDL_Delay(frameDelay-frameTime);
	}
	app->clean();
	return 0;
}