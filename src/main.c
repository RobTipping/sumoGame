#include "raylib.h"

typedef struct sumo {
  Vector2 pos;
  float r;
  Color color;
} sumo;

sumo playerOne;
sumo playerTwo;

void updatePlayer();

int main(void) {
  InitWindow(800, 800, "Sumo Prime Time!");
  SetTargetFPS(60);
  playerOne.pos = (Vector2){50, 50};
  playerOne.r = 25.0;
  playerOne.color = GREEN;
  playerTwo.pos = (Vector2){750, 50};
  playerTwo.r = 25.0;
  playerTwo.color = BLUE;

  while (!WindowShouldClose()) {
    updatePlayer();
    // draw shit
    BeginDrawing();
    ClearBackground(GRAY);
    DrawCircleV(playerOne.pos, playerOne.r, playerOne.color);
    DrawCircleV(playerTwo.pos, playerTwo.r, playerTwo.color);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}

void updatePlayer() {
  float deltaTime = GetFrameTime();
  if (!CheckCollisionCircles(playerOne.pos, playerOne.r, playerTwo.pos, playerTwo.r)) {
    playerOne.pos.x += 20.0 * deltaTime;
    playerTwo.pos.x -= 20.0 * deltaTime;
  }
}
