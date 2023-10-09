#include <iostream>
#include <SDL2/SDL.h>
#include <cstdlib>
#include "growth.h"
#include "kernal.h"
#include <cmath>

using namespace std;

SDL_Window *window = nullptr;
SDL_Surface *screen = nullptr;

const int targetFPS = 60;
const int frameDelay = 1000 / targetFPS;

const int windowWidth = 800;
const int windowHeight = 800;
const double windowScale = 1;

bool startup() {
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        return false;
    }

    window = SDL_CreateWindow("Test", 
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
            windowWidth * windowScale, windowHeight * windowScale, SDL_WINDOW_SHOWN);
    screen = SDL_GetWindowSurface(window);

    return true;
}

void quit() {
    SDL_DestroyWindowSurface(window);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argv, char** args) {

    ExpK k(windowWidth, windowHeight, 4, 50, vector<double>({1}));

    cout << "Finished making the kernal!!";

    if(!startup()) {
        return 1;
    }

    srand((unsigned) time(NULL));

    SDL_Event e;
    bool running = true;
    int frameStart, frameTime;

    vector<vector<double>> grid;

    ExpGF g(.25, .03);
    
    while(running) {
        frameStart = SDL_GetTicks();

        SDL_LockSurface(screen);

        int x, y;
        SDL_GetMouseState(&x, &y);

        while(SDL_PollEvent(&e) != 0) {
            switch(e.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_MOUSEBUTTONDOWN:

                    break;
                case SDL_KEYDOWN:
                    switch(e.key.keysym.sym) {
                        case SDLK_SPACE:
                            break;
                    }
                    break;
            }
        }

        uint8_t* pixels = (uint8_t*) screen->pixels;

        //Update
        for(int i = 0; i < windowWidth; i++) {
            for(int j = 0; j < windowHeight; j++) {
                uint8_t col = (uint8_t) (255 * k.getKernalPoint(x, y, i, j) * 1000);
                int ind = j * screen->pitch + i * screen->format->BytesPerPixel;
                pixels[ind] = col;
                pixels[ind + 1] = col;
                pixels[ind + 2] = col;
            }
        }

        SDL_UnlockSurface(screen);
        SDL_UpdateWindowSurface(window);
        
        frameTime = SDL_GetTicks() - frameStart;
        if(frameTime < frameDelay) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    quit();
    return 0;
}