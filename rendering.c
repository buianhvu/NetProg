#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include "./game.h"
#include "./rendering.h"

const SDL_Color GRID_COLOR = { .r = 255, .g = 255, .b = 255 };
const SDL_Color PLAYER_X_COLOR = { .r = 255, .g = 50, .b = 50 };
const SDL_Color PLAYER_O_COLOR = { .r = 50, .g = 100, .b = 255 };
const SDL_Color TIE_COLOR = { .r = 100, .g = 100, .b = 100 };

void render_grid(SDL_Renderer *renderer, const SDL_Color *color)
{
    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, 255);

    for (int i = 1; i < N; ++i) {
        SDL_RenderDrawLine(renderer,
                           i * CELL_WIDTH, 0,
                           i * CELL_WIDTH, SCREEN_HEIGHT);
        SDL_RenderDrawLine(renderer,
                           0,            i * CELL_HEIGHT,
                           SCREEN_WIDTH, i * CELL_HEIGHT);
    }
}

void render_x(SDL_Renderer *renderer,
              int row, int column,
              const SDL_Color *color)
{
    const float half_box_side = fmin(CELL_WIDTH, CELL_HEIGHT) * 0.25;
    const float center_x = CELL_WIDTH * 0.5 + column * CELL_WIDTH;
    const float center_y = CELL_HEIGHT * 0.5 + row * CELL_HEIGHT;

    thickLineRGBA(renderer,
                  center_x - half_box_side,
                  center_y - half_box_side,
                  center_x + half_box_side,
                  center_y + half_box_side,
                  10,
                  color->r,
                  color->g,
                  color->b,
                  255);
    thickLineRGBA(renderer,
                  center_x + half_box_side,
                  center_y - half_box_side,
                  center_x - half_box_side,
                  center_y + half_box_side,
                  10,
                  color->r,
                  color->g,
                  color->b,
                  255);
}

void render_o(SDL_Renderer *renderer,
              int row, int column,
              const SDL_Color *color)
{
    const float half_box_side = fmin(CELL_WIDTH, CELL_HEIGHT) * 0.25;
    const float center_x = CELL_WIDTH * 0.5 + column * CELL_WIDTH;
    const float center_y = CELL_HEIGHT * 0.5 + row * CELL_HEIGHT;

    filledCircleRGBA(renderer,
                     center_x, center_y, half_box_side + 5,
                     color->r, color->g, color->b, 255);
    filledCircleRGBA(renderer,
                     center_x, center_y, half_box_side - 5,
                     0, 0, 0, 255);
}

void render_board(SDL_Renderer *renderer,
                  const int *board,
                  const SDL_Color *player_x_color,
                  const SDL_Color *player_o_color)
{
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            switch (board[i * N + j]) {
            case PLAYER_X:
                render_x(renderer, i, j, player_x_color);
                break;

            case PLAYER_O:
                render_o(renderer, i, j, player_o_color);
                break;

            default: {}
            }
        }
    }
}

void render_running_state(SDL_Renderer *renderer, game_t *game)
{
    render_grid(renderer, &GRID_COLOR);
    render_board(renderer,
                 game->board,
                 &PLAYER_X_COLOR,
                 &PLAYER_O_COLOR);
}

void render_game_over_state(SDL_Renderer *renderer,
                            game_t *game,
                            const SDL_Color *color)
{
    render_grid(renderer, color);
    render_board(renderer,
                 game->board,
                 color,
                 color);
}


void render_welcome(SDL_Renderer *renderer){
char* welcome_string = "Welcome to TicTacToe Game GROUP11-ICTK59";
  TTF_Init();
  TTF_Font * font = TTF_OpenFont("Roboto-Black.ttf", 20);
  if(!font) {
    printf("TTF_OpenFont: %s\n", TTF_GetError());
  }
  SDL_Color color = { 125, 255, 125 };
  SDL_Surface * surface = TTF_RenderText_Solid(font,welcome_string, color);
  SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  

//   int texW = 300;
//   int texH = 300;
//   SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
//   SDL_Rect dstrect = { 0, 0, texW, texH };
//   SDL_RenderCopy(renderer, texture, NULL, &dstrect);
//   SDL_RenderPresent(renderer);
  SDL_Rect srcRest;
  SDL_Rect desRect;
  TTF_SizeText(font, welcome_string, &srcRest.w, &srcRest.h);
 
  srcRest.x = 0;
  srcRest.y =  0;
 
  desRect.x = 100;
  desRect.y = 220;
 
  desRect.w = srcRest.w;
  desRect.h = srcRest.h;
  TTF_CloseFont(font);
  
  SDL_RenderCopy(renderer, texture, &srcRest, &desRect);
  SDL_RenderPresent(renderer);
}
void render_login_state(SDL_Renderer *renderer,game_t *game){
  render_welcome(renderer);
  printf("Enter your name: ");
  scanf("%s", game->player_1);
  game->state = FIND_GAME_STATE;
 }
void render_find_game(game_t *game){
  int invite;
  printf("1. An\n");
  printf("2. Phong\n");
  printf("3. Thanh\n");
  printf("Invite one: ");
  scanf("%d", &invite);
  game->state = RUNNING_STATE;
}


void render_game(SDL_Renderer *renderer, game_t *game)
{
    switch (game->state) {
    case LOGIN_STATE:
        render_login_state(renderer, game);
        break;
    case FIND_GAME_STATE:
        render_find_game(game);
        break;
    case RUNNING_STATE:
        render_running_state(renderer, game);
        break;

    case PLAYER_X_WON_STATE:
        render_game_over_state(renderer, game, &PLAYER_X_COLOR);
        break;

    case PLAYER_O_WON_STATE:
        render_game_over_state(renderer, game, &PLAYER_O_COLOR);
        break;

    case TIE_STATE:
        render_game_over_state(renderer, game, &TIE_COLOR);
        break;

    default: {}
    }
}
