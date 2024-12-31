#include<stdio.h>
#include<SDL2/SDL.h>
#include<math.h>
#include<stdbool.h>


#define SURF_WIDTH 900
#define SURF_HEIGHT 600
#define COLOR_WHITE 0xffffffff
#define COLOR_GRAY 0x1f1f1f1f
#define COLOR_BLACK 0x00000000
#define COLOR_BLUE 0x34c3eb
#define CELL_SIZE 10
#define COLUMNS SURF_WIDTH/CELL_SIZE
#define ROWS SURF_HEIGHT/CELL_SIZE
#define MODE_TYPE true // true - solid, false - water


struct Cell {
  int type;
 
  int filled;

};

struct Cell grid[ROWS][COLUMNS];

void draw_grid(SDL_Surface* surface) {
  
  for (int i = 0; i < COLUMNS; i++) {
    SDL_Rect column = (SDL_Rect) {i*CELL_SIZE, 0, 2, SURF_HEIGHT};
    SDL_FillRect(surface, &column, COLOR_GRAY);
  }

  for (int j = 0; j < ROWS; j++) {
    SDL_Rect row = (SDL_Rect) {0, j*CELL_SIZE, SURF_WIDTH, 2};
    SDL_FillRect(surface, &row, COLOR_GRAY);
  }
}

void color_cell(SDL_Surface* surface) {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLUMNS; j++) {
      if (grid[i][j].filled == 1) {
	if (grid[i][j].type) {
	  //printf("x = %d, y = %d\n", i, j);
	  int pixel_x = j * CELL_SIZE;
	  int pixel_y = i * CELL_SIZE;
	  SDL_Rect cell_rect = (SDL_Rect) {pixel_x, pixel_y, CELL_SIZE, CELL_SIZE};
	  SDL_FillRect(surface, &cell_rect, COLOR_WHITE);
	} else if (!grid[i][j].type) {
	  int pixel_x = j * CELL_SIZE;
	  int pixel_y = i * CELL_SIZE;
	  SDL_Rect cell_rect = (SDL_Rect) {pixel_x, pixel_y, CELL_SIZE, CELL_SIZE};
	  SDL_FillRect(surface, &cell_rect, COLOR_BLUE);
	}
      } else if (grid[i][j].filled == 0) {
	int pixel_x = j * CELL_SIZE;
	int pixel_y = i * CELL_SIZE;
	SDL_Rect cell_rect = (SDL_Rect) {pixel_x, pixel_y, CELL_SIZE, CELL_SIZE};
	SDL_FillRect(surface, &cell_rect, COLOR_BLACK);
      } 
    }
  }
}

void water_drop(SDL_Surface* surface) {
  for (int i = ROWS - 1; i >= 0; i--) {
    for (int j = 0; j < COLUMNS; j++) {
      if (grid[i][j].filled) {
	if(!grid[i][j].type) {
	  if (i + 1 < ROWS && !grid[i+1][j].filled) {
	    grid[i+1][j] = grid[i][j];
	    grid[i][j].filled = 0;
	    
	  }
	  if (grid[i+1][j].filled) {
	    if (j > 0 && j < COLUMNS && !grid[i+1][j-1].filled && !grid[i][j-1].filled) {
	      grid[i+1][j-1] = grid[i][j];
	      grid[i][j].filled = 0;
	    }
	    if (i + 1 < ROWS && j > 0 && j < COLUMNS && !grid[i+1][j+1].filled && !grid[i][j+1].filled) {
	      grid[i+1][j+1] = grid[i][j]; 
	      grid[i][j].filled = 0;
	    }
	    if (i + 1 < ROWS && j > 0 && j < COLUMNS && grid[i+1][j-1].filled && grid[i+1][j-1].filled && !grid[i][j+1].filled) {
	      grid[i][j+1] = grid[i][j];
	      grid[i][j].filled = 0;
	    }
	    if (i + 1 < ROWS && j > 0 && j < COLUMNS && grid[i+1][j-1].filled && grid[i+1][j-1].filled && !grid[i][j-1].filled) {

	      grid[i][j-1] = grid[i][j];
	      grid[i][j].filled = 0;
	    }
	  }
	}
      }
    }
  }
}


void fill_cell(int x, int y, int type){
  if (x >= 0 && x < SURF_WIDTH && y >= 0 && y <  SURF_HEIGHT) {
    grid[y][x].filled = 1;
    grid[y][x].type = type;
    //printf("x = %d, y = %d, filled = %d\t", x, y, grid[y][x].filled);

  }
  
}


int main(int argc, char *argv[]){
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window* window = SDL_CreateWindow("Liquid Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SURF_WIDTH, SURF_HEIGHT, 0);
  SDL_Surface* surface = SDL_GetWindowSurface(window);
  int cell_x_j, cell_y_i;
  bool type = MODE_TYPE;
  SDL_Event event;
  int running = 1;
  while (running) {
    while(SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT){
	running = 0;
      }
      if (event.type == SDL_MOUSEMOTION) {
	if (event.motion.state != 0) {
	  cell_x_j = event.motion.x / CELL_SIZE;
	  cell_y_i = event.motion.y / CELL_SIZE;
	  fill_cell(cell_x_j, cell_y_i, type);
	}      
      }
      if (event.type = SDL_KEYDOWN) {
	if (event.key.keysym.sym == SDLK_SPACE) {
	  type = !type;
	}
      }
    }
    water_drop(surface);
    

    SDL_FillRect(surface, NULL, 0x00000000);
    color_cell(surface);
    draw_grid(surface);
    SDL_UpdateWindowSurface(window);
    SDL_Delay(40);
  }

}
