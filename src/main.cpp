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

double creature[20][20] = {{0,0,0,0,0,0,0.1,0.14,0.1,0,0,0.03,0.03,0,0,0.3,0,0,0,0}, {0,0,0,0,0,0.08,0.24,0.3,0.3,0.18,0.14,0.15,0.16,0.15,0.09,0.2,0,0,0,0}, {0,0,0,0,0,0.15,0.34,0.44,0.46,0.38,0.18,0.14,0.11,0.13,0.19,0.18,0.45,0,0,0}, {0,0,0,0,0.06,0.13,0.39,0.5,0.5,0.37,0.06,0,0,0,0.02,0.16,0.68,0,0,0}, {0,0,0,0.11,0.17,0.17,0.33,0.4,0.38,0.28,0.14,0,0,0,0,0,0.18,0.42,0,0}, {0,0,0.09,0.18,0.13,0.06,0.08,0.26,0.32,0.32,0.27,0,0,0,0,0,0,0.82,0,0}, {0.27,0,0.16,0.12,0,0,0,0.25,0.38,0.44,0.45,0.34,0,0,0,0,0,0.22,0.17,0}, {0,0.07,0.2,0.02,0,0,0,0.31,0.48,0.57,0.6,0.57,0,0,0,0,0,0,0.49,0}, {0,0.59,0.19,0,0,0,0,0.2,0.57,0.69,0.76,0.76,0.49,0,0,0,0,0,0.36,0}, {0,0.58,0.19,0,0,0,0,0,0.67,0.83,0.9,0.92,0.87,0.12,0,0,0,0,0.22,0.07}, {0,0,0.46,0,0,0,0,0,0.7,0.93,1,1,1,0.61,0,0,0,0,0.18,0.11}, {0,0,0.82,0,0,0,0,0,0.47,1,1,0.98,1,0.96,0.27,0,0,0,0.19,0.1}, {0,0,0.46,0,0,0,0,0,0.25,1,1,0.84,0.92,0.97,0.54,0.14,0.04,0.1,0.21,0.05}, {0,0,0,0.4,0,0,0,0,0.09,0.8,1,0.82,0.8,0.85,0.63,0.31,0.18,0.19,0.2,0.01}, {0,0,0,0.36,0.1,0,0,0,0.05,0.54,0.86,0.79,0.74,0.72,0.6,0.39,0.28,0.24,0.13,0}, {0,0,0,0.01,0.3,0.07,0,0,0.08,0.36,0.64,0.7,0.64,0.6,0.51,0.39,0.29,0.19,0.04,0}, {0,0,0,0,0.1,0.24,0.14,0.1,0.15,0.29,0.45,0.53,0.52,0.46,0.4,0.31,0.21,0.08,0,0}, {0,0,0,0,0,0.08,0.21,0.21,0.22,0.29,0.36,0.39,0.37,0.33,0.26,0.18,0.09,0,0,0}, {0,0,0,0,0,0,0.03,0.13,0.19,0.22,0.24,0.24,0.23,0.18,0.13,0.05,0,0,0,0}, {0,0,0,0,0,0,0,0,0.02,0.06,0.08,0.09,0.07,0.05,0.01,0,0,0,0,0}};


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

void drawCreature(vector<double>* grid) {
    int offsetX = 64;
    int offsetY = 64;
    for(int i = 0; i < 20; i++) {
        for(int j = 0; j < 20; j++) {
            (*grid)[(i + offsetX) + (j + offsetY) * windowWidth] = creature[i][j];
        }
    }
}

int main(int argv, char** args) {

    double speed = 6;

    ExpK k(windowWidth, windowHeight, 4, 13, vector<double>({1}));
    cout << "Finished making the kernal!!\n";
    ExpGF g(.15, .02);

    if(!startup()) {
        return 1;
    }

    srand((unsigned) time(NULL));

    SDL_Event e;
    bool running = true;
    int frameStart, frameTime;

    vector<double> grid(windowWidth * windowHeight, 0);
    //setNoise(&grid);
    //setCircle(&grid, windowWidth);
    //multNoise(&grid);
    drawCreature(&grid);

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
                            grid[x + y * windowWidth] = 1;
                            break;
                    }
                    break;
            }
        }

        //Update
        double dt = 1 / (double)targetFPS * speed;
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