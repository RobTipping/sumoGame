#include <math.h>

#include "raylib.h"
#include "raymath.h"

#define MAX_SPEED 200.0f
#define ACCELARATION 200.0f
#define FRICTION 0.97f

typedef struct sumoFist {
  Vector2 pos;
  Vector2 vol;
  Vector2 accel;
  float mass;
  float r;
  Color color;
} sumoFist;

typedef struct sumo {
  Vector2 pos;
  Vector2 vol;
  Vector2 accel;
  float mass;
  float r;
  Color color;
  sumoFist fist;
} sumo;

typedef struct ring {
  Vector2 pos;
  float r;
  Color color;
} ring;

sumo playerOne;
sumo playerTwo;
ring dojo;

void updatePlayer(float delta, sumo *player);
void ballCrash(Vector2 v1, Vector2 v2, float m1, float m2, Vector2 x1, Vector2 x2);
void playerInputs();
void cpuInputs();

int main(void) {
  int winner = 0;
  InitWindow(800, 800, "Sumo Prime Time");
  SetTargetFPS(60);

  // player one
  playerOne.pos = (Vector2){300.0, 400.0};
  playerOne.vol = (Vector2){0.0, 0.0};
  playerOne.accel = (Vector2){0.0, 0.0};
  playerOne.mass = 500.0;
  playerOne.r = 50.0;
  playerOne.color = GREEN;

  playerOne.fist.pos = (Vector2){300.0, 400.0};
  playerOne.fist.vol = (Vector2){0.0, 0.0};
  playerOne.fist.accel = (Vector2){0.0, 0.0};
  playerOne.fist.mass = 500.0;
  playerOne.fist.r = 10.0;
  playerOne.fist.color = DARKGREEN;

  // player two
  playerTwo.pos = (Vector2){500, 400};
  playerTwo.vol = (Vector2){0.0, 0.0};
  playerTwo.accel = (Vector2){0.0, 0.0};
  playerTwo.mass = 500.0;
  playerTwo.r = 50.0;
  playerTwo.color = BLUE;
  dojo.pos = (Vector2){400, 400};
  dojo.r = 275.0;
  dojo.color = DARKGRAY;

  while (!WindowShouldClose()) {
    float deltaTime = GetFrameTime();
    if (winner == 0) {
      playerInputs();
      cpuInputs();
    }
    updatePlayer(deltaTime, &playerOne);
    updatePlayer(deltaTime, &playerTwo);
    if (CheckCollisionCircles(playerOne.pos, playerOne.r, playerTwo.pos, playerTwo.r)) {
      ballCrash(playerOne.vol, playerTwo.vol, playerOne.mass, playerTwo.mass, playerOne.pos, playerTwo.pos);
    }
    playerOne.pos = Vector2Add(playerOne.pos, (Vector2){playerOne.vol.x * deltaTime, playerOne.vol.y * deltaTime});
    playerTwo.pos = Vector2Add(playerTwo.pos, (Vector2){playerTwo.vol.x * deltaTime, playerTwo.vol.y * deltaTime});

    if (!CheckCollisionCircles(playerOne.pos, playerOne.r, dojo.pos, dojo.r - (playerOne.r * 2) + 10) && winner == 0) {
      winner = 2;
    }
    if (!CheckCollisionCircles(playerTwo.pos, playerTwo.r, dojo.pos, dojo.r - (playerTwo.r * 2) + 10) && winner == 0) {
      winner = 1;
    }
    Vector2 temp = Vector2Subtract(playerOne.pos, playerTwo.pos);
    float angelbetween = atan(temp.y / temp.x);
    Vector2 tempPos = {(playerOne.r * sin(angelbetween)), (playerOne.r * cos(angelbetween))};
    tempPos = Vector2Add(tempPos, playerOne.pos);

    // draw shit
    BeginDrawing();
    ClearBackground(GRAY);
    DrawCircleV(dojo.pos, dojo.r, dojo.color);
    DrawCircleV(playerOne.pos, playerOne.r, playerOne.color);
    DrawCircleV(playerOne.fist.pos, playerOne.fist.r, playerOne.fist.color);
    DrawCircleV(playerTwo.pos, playerTwo.r, playerTwo.color);
    DrawLineV(playerOne.pos, tempPos, BLACK);
    DrawText(TextFormat("player distance x: %f", temp.x), 10, 10, 20, LIGHTGRAY);
    DrawText(TextFormat("player sidtance y: %f", temp.y), 10, 40, 20, LIGHTGRAY);
    // DrawText(TextFormat("ball one velocity Y: %f", playerOne.vol.y), 10, 70, 20, LIGHTGRAY);
    // DrawText(TextFormat("ball two velocity X: %f", playerTwo.vol.x), 10, 100, 20, LIGHTGRAY);
    // DrawText(TextFormat("ball two velocity Y: %f", playerTwo.vol.y), 10, 130, 20, LIGHTGRAY);
    if (winner != 0) {
      DrawText(TextFormat("PLAYER %d WINS", winner), 80, 350, 80, LIGHTGRAY);
    }
    EndDrawing();
  }
  CloseWindow();
  return 0;
}

void updatePlayer(float delta, sumo *player) {
  // add ball on ball collision
  if ((player->pos.x + player->r) > 800 || (player->pos.x - player->r) < 0) {
    player->vol.x *= -1;
  }
  if ((player->pos.y + player->r) > 800 || (player->pos.y - player->r) < 0) {
    player->vol.y *= -1;
  }
  player->vol = Vector2Add(player->vol, (Vector2){player->accel.x * delta, player->accel.y * delta});
  player->vol = Vector2Scale(player->vol, FRICTION);

  if (player->vol.x > -0.5 && player->vol.x < 0.5) {
    player->vol.x = 0.0;
  }
  if (player->vol.y > -0.5 && player->vol.y < 0.5) {
    player->vol.y = 0.0;
  }
  if (player->vol.x > MAX_SPEED) {
    player->vol.x = MAX_SPEED;
  }
  if (player->vol.x < -MAX_SPEED) {
    player->vol.x = -MAX_SPEED;
  }
  player->accel = (Vector2){0.0, 0.0};
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

void playerInputs() {
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
}
void cpuInputs() {
  if (GetRandomValue(0, 100) > 50) {
    return;
  }
  if (playerOne.pos.x < playerTwo.pos.x) {
    playerTwo.accel.x = -ACCELARATION;
  }
  if (playerOne.pos.x > playerTwo.pos.x) {
    playerTwo.accel.x = ACCELARATION;
  }
  if (playerOne.pos.y < playerTwo.pos.y) {
    playerTwo.accel.y = -ACCELARATION;
  }
  if (playerOne.pos.y > playerTwo.pos.y) {
    playerTwo.accel.y = ACCELARATION;
  }
}
