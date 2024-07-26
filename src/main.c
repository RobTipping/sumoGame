#include "raylib.h"
#include "raymath.h"

#define MAX_SPEED 200.0f
#define ACCELARATION 200.0f
#define FRICTION 1.0f

typedef struct sumo {
  Vector2 pos;
  Vector2 vol;
  Vector2 accel;
  float mass;
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
void ballCrash(Vector2 v1, Vector2 v2, float m1, float m2, Vector2 x1, Vector2 x2);

int main(void) {
  InitWindow(800, 800, "test");
  SetTargetFPS(60);
  playerOne.pos = (Vector2){200.0, 400.0};
  playerOne.vol = (Vector2){300.0, 200.0};
  playerOne.accel = (Vector2){0.0, 0.0};
  playerOne.mass = 150.0;
  playerOne.r = 100.0;
  playerOne.color = GREEN;
  playerTwo.pos = (Vector2){600, 425};
  playerTwo.vol = (Vector2){-200.0, -500.0};
  playerTwo.accel = (Vector2){0.0, 0.0};
  playerTwo.mass = 100.0;
  playerTwo.r = 75.0;
  playerTwo.color = BLUE;
  dojo.pos = (Vector2){400, 400};
  dojo.r = 300.0;
  dojo.color = RED;

  while (!WindowShouldClose()) {
    updatePlayer();
    // draw shit
    BeginDrawing();
    ClearBackground(GRAY);
    // DrawCircleV(dojo.pos, dojo.r, dojo.color);
    DrawCircleV(playerOne.pos, playerOne.r, playerOne.color);
    DrawCircleV(playerTwo.pos, playerTwo.r, playerTwo.color);
    DrawText(TextFormat("ball one velocity X: %f", playerOne.vol.x), 10, 40, 20, LIGHTGRAY);
    DrawText(TextFormat("ball one velocity Y: %f", playerOne.vol.y), 10, 70, 20, LIGHTGRAY);
    DrawText(TextFormat("ball two velocity X: %f", playerTwo.vol.x), 10, 100, 20, LIGHTGRAY);
    DrawText(TextFormat("ball two velocity Y: %f", playerTwo.vol.y), 10, 130, 20, LIGHTGRAY);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}

void updatePlayer() {
  float deltaTime = GetFrameTime();
  /*
  if (IsKeyDown(KEY_S)) {
    playerOne.accel.x = -ACCELARATION;
  }
  if (IsKeyDown(KEY_F)) {
    playerOne.accel.x = ACCELARATION;
  }
  if (IsKeyDown(KEY_E)) {
    playerOne.accel.y = -ACCELARATION;
  }
  if (IsKeyDown(KEY_D)) {
    playerOne.accel.y = ACCELARATION;
  }
  */
  // add ball on ball collision
  if ((playerOne.pos.x + playerOne.r) > 800 || (playerOne.pos.x - playerOne.r) < 0) {
    playerOne.vol.x *= -1;
  }
  if ((playerOne.pos.y + playerOne.r) > 800 || (playerOne.pos.y - playerOne.r) < 0) {
    playerOne.vol.y *= -1;
  }
  playerOne.vol = Vector2Add(playerOne.vol, (Vector2){playerOne.accel.x * deltaTime, playerOne.accel.y * deltaTime});
  playerOne.pos = Vector2Add(playerOne.pos, (Vector2){playerOne.vol.x * deltaTime, playerOne.vol.y * deltaTime});

  if ((playerTwo.pos.x + playerTwo.r) > 800 || (playerTwo.pos.x - playerTwo.r) < 0) {
    playerTwo.vol.x *= -1;
  }
  if ((playerTwo.pos.y + playerTwo.r) > 800 || (playerTwo.pos.y - playerTwo.r) < 0) {
    playerTwo.vol.y *= -1;
  }
  playerTwo.vol = Vector2Add(playerTwo.vol, (Vector2){playerTwo.accel.x * deltaTime, playerTwo.accel.y * deltaTime});
  playerTwo.pos = Vector2Add(playerTwo.pos, (Vector2){playerTwo.vol.x * deltaTime, playerTwo.vol.y * deltaTime});
  if (Vector2Distance(playerOne.pos, playerTwo.pos) < (playerOne.r + playerTwo.r)) {
    ballCrash(playerOne.vol, playerTwo.vol, playerOne.mass, playerTwo.mass, playerOne.pos, playerTwo.pos);
    playerOne.pos = Vector2Add(playerOne.pos, (Vector2){playerOne.vol.x * deltaTime, playerOne.vol.y * deltaTime});
    playerTwo.pos = Vector2Add(playerTwo.pos, (Vector2){playerTwo.vol.x * deltaTime, playerTwo.vol.y * deltaTime});
    return;
  }
  /*
  if (playerOne.vol.x > MAX_SPEED) {
    playerOne.vol.x = MAX_SPEED;
  }
  if (playerOne.vol.x < -MAX_SPEED) {
    playerOne.vol.x = -MAX_SPEED;
  }
  if (playerOne.vol.x > 0.5) {
    playerOne.vol.x -= FRICTION;
  } else if (playerOne.vol.x < 0.5) {
    playerOne.vol.x += FRICTION;
  } else {
    playerOne.vol.x = 0.0;
  }
  if (playerOne.vol.y > MAX_SPEED) {
    playerOne.vol.y = MAX_SPEED;
  }
  if (playerOne.vol.y < -MAX_SPEED) {
    playerOne.vol.y = -MAX_SPEED;
  }
  if (playerOne.vol.y > 0.5) {
    playerOne.vol.y -= FRICTION;
  } else if (playerOne.vol.y < 0.5) {
    playerOne.vol.y += FRICTION;
  } else {
    playerOne.vol.y = 0.0;
  }
  playerOne.accel = (Vector2){0.0, 0.0};
  */
}
void ballCrash(Vector2 v1, Vector2 v2, float m1, float m2, Vector2 x1, Vector2 x2) {
  float fDistance = Vector2Distance(x1, x2);
  // Normal
  float nx = (x2.x - x1.x) / fDistance;
  float ny = (x2.y - x1.y) / fDistance;

  float kx = (v1.x - v2.x);
  float ky = (v1.y - v2.y);
  float p = 2.0 * (nx * kx + ny * ky) / (m1 + m2);
  playerOne.vol.x = playerOne.vol.x - p * m2 * nx;
  playerOne.vol.y = playerOne.vol.y - p * m2 * ny;
  playerTwo.vol.x = playerTwo.vol.x + p * m1 * nx;
  playerTwo.vol.y = playerTwo.vol.y + p * m1 * ny;
}
