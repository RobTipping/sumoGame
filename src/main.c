#include "raylib.h"

#define MAX_SPEED 100.0f

typedef struct sumo {
  Vector2 pos;
  float r;
  Color color;
} sumo;

typedef struct ring {
  Vector2 pos;
  float r;
  Color color;
} ring;

sumo playerOne;
sumo playerTwo;
ring dojo;

void updatePlayer();

int main(void) {
  InitWindow(800, 800, "Sumo Prime Time!");
  SetTargetFPS(60);
  playerOne.pos = (Vector2){200, 400};
  playerOne.r = 25.0;
  playerOne.color = GREEN;
  playerTwo.pos = (Vector2){600, 300};
  playerTwo.r = 25.0;
  playerTwo.color = BLUE;
  dojo.pos = (Vector2){400, 400};
  dojo.r = 300.0;
  dojo.color = RED;

  while (!WindowShouldClose()) {
    updatePlayer();
    // draw shit
    BeginDrawing();
    ClearBackground(GRAY);
    DrawCircleV(dojo.pos, dojo.r, dojo.color);
    DrawCircleV(playerOne.pos, playerOne.r, playerOne.color);
    DrawCircleV(playerTwo.pos, playerTwo.r, playerTwo.color);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}

void updatePlayer() {
  float deltaTime = GetFrameTime();
  if (CheckCollisionCircles(playerOne.pos, playerOne.r, dojo.pos, dojo.r - (playerOne.r * 2.0))) {
    if (!CheckCollisionCircles(playerOne.pos, playerOne.r, playerTwo.pos, playerTwo.r)) {
      playerOne.pos.x += MAX_SPEED * deltaTime;
      playerTwo.pos.x -= MAX_SPEED * deltaTime;
    }
  }
}
