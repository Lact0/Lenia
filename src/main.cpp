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

//WIDTH AND HEIGHT MUST BE POWERS OF 2
const int windowWidth = 128;
const int windowHeight = 128;
const double windowScale = 4;

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

double clamp(double n) {
    if(n > 1) {
        return 1;
    }
    if(n < 0) {
        return 0;
    }
    return n;
}

void setNoise(vector<double>* arr) {
    for(int i = 0; i < arr->size(); i++) {
        (*arr)[i] = (double) rand() / (RAND_MAX);

    }
}

void multNoise(vector<double>* arr) {
    for(int i = 0; i < arr->size(); i++) {
        (*arr)[i] *= (double) rand() / (RAND_MAX);

    }
}

void setCircle(vector<double>* arr, int width) {
    double centerX = width / 2;
    double centerY = arr->size() / width / 2;
    double rad = sqrt(centerX * centerX + centerY * centerY);
    for(int i = 0; i < arr->size(); i++) {
        double x = i % width;
        double y = i / width;
        double dist = sqrt(pow(centerX - x, 2.0) + pow(centerY - y, 2.0));
        (*arr)[i] = 1 - dist / rad;
    }
}

vector<int> mapColGrey(double n) {
    int col = n * 255;
    vector<int> colors({col, col, col});
    return colors;
}
vector<int> mapColBGR(double n) {
    int r = n * 255;
    int g = (1 - abs(1 - 2 * n)) * 255;
    int b = (1 - n) * 255;
    vector<int> colors({b, g, r});
    return colors;
}

int main(int argv, char** args) {

    ExpK k(windowWidth, windowHeight, 4, 40, vector<double>({1, .5}));
    cout << "Finished making the kernal!!\n";
    ExpGF g(.2, .015);

    if(!startup()) {
        return 1;
    }

    srand((unsigned) time(NULL));

    SDL_Event e;
    bool running = true;
    int frameStart, frameTime;

    vector<double> grid(windowWidth * windowHeight, 0);
    //setNoise(&grid);
    setCircle(&grid, windowWidth);
    multNoise(&grid);

    int tick = 0;
    while(running) {
        frameStart = SDL_GetTicks();

        SDL_LockSurface(screen);

        int x, y;
        SDL_GetMouseState(&x, &y);
        x /= windowScale;
        y /= windowScale;

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

        //Update
        double dt = 1 / (double)targetFPS;
        vector<double> conv = k.convolve(grid);
        vector<double> growth = g.growth(conv);
        for(int i = 0; i < grid.size(); i++) {
            grid[i] = clamp(grid[i] + dt * growth[i]);
        }

        //Draw
        uint8_t* pixels = (uint8_t*) screen->pixels;

        for(int i = 0; i < windowWidth; i++) {
            for(int j = 0; j < windowHeight; j++) {
                //uint8_t col = (uint8_t) (255 * k.getKernalPoint(x, y, i, j) * 1000);
                vector<int> col = mapColBGR(grid[i + j * windowWidth]);
                for(int k = 0; k < windowScale; k++) {
                    for(int l = 0; l < windowScale; l++) {
                        int ind = (j * windowScale + l) * screen->pitch + (i * windowScale + k) * screen->format->BytesPerPixel;
                        pixels[ind] = col[0];
                        pixels[ind + 1] = col[1];
                        pixels[ind + 2] = col[2];
                    }
                }
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