#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>

int main()
{
        SDL_Init(SDL_INIT_VIDEO);
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_CreateWindowAndRenderer(0, 0,
                        SDL_WINDOW_FULLSCREEN_DESKTOP,
                        &window,
                        &renderer);
        SDL_RenderSetLogicalSize(renderer, IMAGE_WIDTH, IMAGE_HEIGHT);

        SDL_Texture *texture = SDL_CreateTexture(renderer,
                        SDL_PIXELFORMAT_RGB888,
                        SDL_TEXTUREACCESS_STREAMING,
                        IMAGE_WIDTH, IMAGE_HEIGHT);

        int *pixels = malloc(IMAGE_WIDTH * IMAGE_HEIGHT * sizeof(int));
        for (int i = 0; i < IMAGE_WIDTH * IMAGE_HEIGHT; i++) {
                pixels[i] = 0;
        }

        SDL_ShowCursor(SDL_DISABLE);

        struct renderer *raytracer = malloc(sizeof(struct renderer));
        renderer_init(raytracer, pixels);

        int running = 1;
        while (running) {
                SDL_Event event;
                while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN) {
                                running = 0;
                        }
                }

                renderer_update(raytracer);
                SDL_UpdateTexture(texture, NULL, pixels, IMAGE_WIDTH * sizeof(int));
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, texture, NULL, NULL);
                SDL_RenderPresent(renderer);
        }

        SDL_Quit();
        return 0;
}
