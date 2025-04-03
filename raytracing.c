#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_stdinc.h>
#include <stdint.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>

#define width 900
#define height 600
#define COLOUR_WHITE 0xFFFFFFFF
#define COLOUR_BLACK 0x00000000
#define COLOUR_GRAY 0x808080
#define COLOUR_YELLOW 0xFFFF00 
#define RAY_NUMBER 100

struct Circle{
  double x;
  double y;
  double r;
};

typedef struct Ray_details{
  double x_start, y_start;
  double angle;
  //double x_end, y_end;
}Ray;

void genrate_rays(struct Circle circle, Ray rays[RAY_NUMBER] ){

  for(int i = 0; i < RAY_NUMBER; i++){
    double angle = ((double)i/RAY_NUMBER) * 2 * M_PI ;
    struct Ray_details ray = {circle.x, circle.y, angle};
    rays[i] = ray;
    printf("andgle: %2f\n", angle);

  }

}

void FillCircle(SDL_Surface *surface, struct Circle circle, Uint32 color){
  
  double r2 = pow(circle.r, 2);
  for(double x = circle.x - circle.r; x <= circle.x + circle.r; x++){
    
    for(double y = circle.y - circle.r; y <= circle.y + circle.r; y++){
        
      double distance_squared = pow(x-circle.x, 2) + pow(y- circle.y, 2);
      if (distance_squared < r2 ){
        SDL_Rect pixel = {x,y,1,1};
        SDL_FillRect(surface, &pixel, color);
      }        
    }
  } 
} 

void FillRays(SDL_Surface * surface, Ray rays[RAY_NUMBER],struct Circle object, Uint32 color){
  
  for(int i = 0; i < RAY_NUMBER; i++){
    
    int end_of_screen = 0;
    int object_hit = 0;

    int step = 1;
    
    Ray ray = rays[i];
    double x_draw = ray.x_start;
    double y_draw = ray.y_start;
    
    double r2 = pow(object.r, 2);
    while(!end_of_screen && !object_hit){
      
      //Ray ray = rays[i];
      x_draw += i*cos(ray.angle) + ray.x_start;
      y_draw += i*sin(ray.angle)+ ray.y_start;
      SDL_Rect pixel = {x_draw, y_draw, 1, 1};
      SDL_FillRect(surface, &pixel, color);
      if(x_draw < 0 || x_draw > width ){
        end_of_screen = 1;
      }
      if(y_draw < 0 || y_draw > height){
        end_of_screen = 1;
      }
      double distance_squares = pow(x_draw - object.x, 2) + pow(y_draw - object.y, 2);
      if(distance_squares < r2){
        object_hit = 1;
      }
      
    }
      //SDL_Rect pixel = {rays[i].x_start, rays[i].y_start, 1, 1};
      //SDL_FillRect(surface, &pixel, color);
    
  }
}



int main(){
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("Raytracing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);

  SDL_Surface* surface = SDL_GetWindowSurface(window);
  //SDL_Rect rect = {200.0f,200.0f,200.0f,200.0f};
  //SDL_FillRect(surface, &rect, COLOUR_WHITE);
  
  struct Circle circle =  {200.0,200.0,80.0};
  struct Circle shadow_circle =  {650.0,350.0,140.0};
  SDL_Rect earse_rect = {0,0,width,height};
  
  Ray rays[RAY_NUMBER];
  genrate_rays(circle, rays);

  int simulation_runing = 1;
  while(simulation_runing){

    SDL_Event event;
    while(SDL_PollEvent(&event)){
      if(event.type == SDL_QUIT){
        simulation_runing = 0;
      }
      if (event.type == SDL_MOUSEMOTION && event.motion.state != 0){
        circle.x = event.motion.x;
        circle.y = event.motion.y;
        genrate_rays(circle, rays);
      } //SDL_BUTTON(SDL_MOUSEMOTION))
    }
    SDL_FillRect(surface, &earse_rect , COLOUR_BLACK);
    FillCircle(surface, circle, COLOUR_WHITE);
    FillCircle(surface, shadow_circle, COLOUR_WHITE);
    FillRays(surface, rays, shadow_circle, COLOUR_YELLOW);
    SDL_UpdateWindowSurface(window);

    SDL_Delay(10);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
