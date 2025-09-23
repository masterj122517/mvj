#include "mvj.h"

int main()
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL init failed: " << SDL_GetError() << std::endl;
    return -1;
  }

  SDL_Window* window = SDL_CreateWindow("Music Visualizer",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        800, 600,
                                        SDL_WINDOW_SHOWN);
  if (!window) {
    std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
    return -1;
  }

  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

  bool running = true;
  SDL_Event e;
  while (running) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT)
        running = false;
    }

    SDL_SetRenderDrawColor(renderer, 18, 18, 18, 18);
    SDL_RenderClear(renderer);

    SDL_Rect rect_1 = {0, 0, 10, 10};
    SDL_Rect rect_2 = {100, 100, 100, 100};
    SDL_Rect res;
    SDL_UnionRect(&rect_1, &rect_2, &res);
    SDL_SetRenderDrawColor(renderer, 67, 205, 127, 255);
    SDL_RenderFillRect(renderer, &res);

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
