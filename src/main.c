#include <math.h>

#include "raylib.h"
#include "raymath.h"

#define MAX_SPEED 200.0f
#define ACCELARATION 200.0f
#define FIST_ACCELARATION 1000.0f
#define FRICTION 0.97f

typedef struct ball {
  Vector2 homePos;
  Vector2 realPos;
  Vector2 direction;
  Vector2 vol;
  Vector2 accel;
  float mass;
  float r;
  Color color;
} ball;

typedef struct sumo {
  ball body;
  bool slapping;
  double slapStartTime;
  ball fistR;
  ball fistL;
} sumo;

typedef struct ring {
  Vector2 pos;
  float r;
  Color color;
} ring;

sumo playerOne;
sumo playerTwo;
sumo fistPlayerOne;  // will get replaced by fist

ring dojo;

Vector2 fistGetToo;
Vector2 slapDirection;
Vector2 tempPos;
Vector2 temp;
Vector2 punchVector;
Vector2 tempPuchVol;

double gameTime;

void updatePlayer(float delta, ball* player);
void ballCrash(ball* a, ball* b);
void playerInputs();
void cpuInputs();
void fistInputs(sumo* a);
void fistUpdate(sumo* a, sumo* b);

int main(void) {
  int winner = 0;
  InitWindow(800, 800, "Sumo Prime Time");
  SetTargetFPS(60);

  // player one
  playerOne.body.realPos = (Vector2){300.0, 400.0};
  playerOne.body.vol = (Vector2){0.0, 0.0};
  playerOne.body.accel = (Vector2){0.0, 0.0};
  playerOne.body.mass = 500.0;
  playerOne.body.r = 50.0;
  playerOne.body.color = GREEN;
  playerOne.slapping = false;

  playerOne.fistL.realPos = (Vector2){300.0, 400.0};
  playerOne.fistL.vol = (Vector2){0.0, 0.0};
  playerOne.fistL.accel = (Vector2){0.0, 0.0};
  playerOne.fistL.mass = 500.0;
  playerOne.fistL.r = 20.0;
  playerOne.fistL.color = DARKGREEN;

  playerOne.fistR.realPos = (Vector2){300.0, 400.0};
  playerOne.fistR.vol = (Vector2){0.0, 0.0};
  playerOne.fistR.accel = (Vector2){0.0, 0.0};
  playerOne.fistR.mass = 500.0;
  playerOne.fistR.r = 20.0;
  playerOne.fistR.color = DARKPURPLE;

  // player two
  playerTwo.body.realPos = (Vector2){500, 400};
  playerTwo.body.vol = (Vector2){0.0, 0.0};
  playerTwo.body.accel = (Vector2){0.0, 0.0};
  playerTwo.body.mass = 500.0;
  playerTwo.body.r = 50.0;
  playerTwo.body.color = BLUE;

  playerTwo.fistL.realPos = (Vector2){300.0, 400.0};
  playerTwo.fistL.vol = (Vector2){0.0, 0.0};
  playerTwo.fistL.accel = (Vector2){0.0, 0.0};
  playerTwo.fistL.mass = 500.0;
  playerTwo.fistL.r = 20.0;
  playerTwo.fistL.color = DARKBLUE;

  playerTwo.fistR.realPos = (Vector2){300.0, 400.0};
  playerTwo.fistR.vol = (Vector2){0.0, 0.0};
  playerTwo.fistR.accel = (Vector2){0.0, 0.0};
  playerTwo.fistR.mass = 500.0;
  playerTwo.fistR.r = 20.0;
  playerTwo.fistR.color = MAROON;

  dojo.pos = (Vector2){400, 400};
  dojo.r = 275.0;
  dojo.color = DARKGRAY;

  // setting up fist!
  fistUpdate(&playerOne, &playerTwo);
  playerOne.fistL.realPos = playerOne.fistL.homePos;
  playerOne.fistR.realPos = playerOne.fistR.homePos;

  fistUpdate(&playerTwo, &playerOne);
  playerTwo.fistL.realPos = playerTwo.fistL.homePos;
  playerTwo.fistR.realPos = playerTwo.fistR.homePos;

  while (!WindowShouldClose()) {
    gameTime = GetTime();
    float deltaTime = GetFrameTime();
    if (winner == 0) {
      fistInputs(&playerOne);
      fistInputs(&playerTwo);
      playerInputs();
      cpuInputs();
    }

    updatePlayer(deltaTime, &playerOne.body);
    updatePlayer(deltaTime, &playerOne.fistL);
    updatePlayer(deltaTime, &playerOne.fistR);

    updatePlayer(deltaTime, &playerTwo.body);
    updatePlayer(deltaTime, &playerTwo.fistL);
    updatePlayer(deltaTime, &playerTwo.fistR);

    ballCrash(&playerOne.body, &playerTwo.body);
    ballCrash(&playerOne.fistL, &playerTwo.body);
    ballCrash(&playerOne.fistR, &playerTwo.body);
    ballCrash(&playerTwo.fistL, &playerOne.body);
    ballCrash(&playerTwo.fistR, &playerOne.body);

    ballCrash(&playerOne.fistL, &playerTwo.fistL);
    ballCrash(&playerOne.fistL, &playerTwo.fistR);

    ballCrash(&playerOne.fistR, &playerTwo.fistL);
    ballCrash(&playerOne.fistR, &playerTwo.fistR);

    playerOne.body.realPos = Vector2Add(playerOne.body.realPos, (Vector2){playerOne.body.vol.x * deltaTime, playerOne.body.vol.y * deltaTime});
    playerOne.fistL.realPos = Vector2Add(playerOne.fistL.realPos, (Vector2){playerOne.fistL.vol.x * deltaTime, playerOne.fistL.vol.y * deltaTime});
    playerOne.fistL.vol = Vector2Scale(playerOne.fistL.vol, 0.9);
    playerOne.fistR.realPos = Vector2Add(playerOne.fistR.realPos, (Vector2){playerOne.fistR.vol.x * deltaTime, playerOne.fistR.vol.y * deltaTime});
    playerOne.fistR.vol = Vector2Scale(playerOne.fistR.vol, 0.9);
    playerTwo.body.realPos = Vector2Add(playerTwo.body.realPos, (Vector2){playerTwo.body.vol.x * deltaTime, playerTwo.body.vol.y * deltaTime});
    playerTwo.fistL.realPos = Vector2Add(playerTwo.fistL.realPos, (Vector2){playerTwo.fistL.vol.x * deltaTime, playerTwo.fistL.vol.y * deltaTime});
    playerTwo.fistL.vol = Vector2Scale(playerTwo.fistL.vol, 0.9);
    playerTwo.fistR.realPos = Vector2Add(playerTwo.fistR.realPos, (Vector2){playerTwo.fistR.vol.x * deltaTime, playerTwo.fistR.vol.y * deltaTime});
    playerTwo.fistR.vol = Vector2Scale(playerTwo.fistR.vol, 0.9);

    if (!CheckCollisionCircles(playerOne.body.realPos, playerOne.body.r, dojo.pos, dojo.r - (playerOne.body.r * 2) + 10) && winner == 0) {
      winner = 2;
    }
    if (!CheckCollisionCircles(playerTwo.body.realPos, playerTwo.body.r, dojo.pos, dojo.r - (playerTwo.body.r * 2) + 10) && winner == 0) {
      winner = 1;
    }

    //  // new fist pos
    fistUpdate(&playerOne, &playerTwo);
    fistUpdate(&playerTwo, &playerOne);

    // draw shit
    BeginDrawing();
    ClearBackground(GRAY);
    DrawCircleV(dojo.pos, dojo.r, dojo.color);
    DrawCircleV(playerOne.body.realPos, playerOne.body.r, playerOne.body.color);
    DrawCircleV(playerOne.fistL.realPos, playerOne.fistL.r, playerOne.fistL.color);
    DrawCircleV(playerOne.fistR.realPos, playerOne.fistR.r, playerOne.fistR.color);
    DrawCircleV(playerTwo.body.realPos, playerTwo.body.r, playerTwo.body.color);
    DrawCircleV(playerTwo.fistL.realPos, playerTwo.fistL.r, playerTwo.fistL.color);
    DrawCircleV(playerTwo.fistR.realPos, playerTwo.fistR.r, playerTwo.fistR.color);
    DrawLineV(playerOne.body.realPos, playerOne.body.direction, BLACK);
    // DrawLineEx(playerOne.fistL.homePos, playerOne.fistL.realPos, playerOne.fistL.r + 10.0, ORANGE);
    // DrawText(TextFormat("time: %f", gameTime), 10, 220, 20, LIGHTGRAY);
    // DrawText(TextFormat("direction X: %f", playerOne.body.direction.x), 10, 100, 20, LIGHTGRAY);
    // DrawText(TextFormat("direction Y: %f", playerOne.body.direction.y), 10, 130, 20, LIGHTGRAY);
    // DrawText(TextFormat("is fist home: %f", Vector2Distance(playerOne.fistL.realPos, playerOne.fistL.homePos)), 10, 160, 20, LIGHTGRAY);
    // DrawText(TextFormat("is fist home: %f", Vector2Distance(playerOne.fistR.realPos, playerOne.fistR.homePos)), 10, 190, 20, LIGHTGRAY);
    if (winner != 0) {
      DrawText(TextFormat("PLAYER %d WINS", winner), 80, 350, 80, LIGHTGRAY);
    }
    EndDrawing();
  }
  CloseWindow();
  return 0;
}

void updatePlayer(float delta, ball* player) {
  // updateMainCircle
  if ((player->realPos.x + player->r) > 800 || (player->realPos.x - player->r) < 0) {
    player->vol.x *= -1;
  }
  if ((player->realPos.y + player->r) > 800 || (player->realPos.y - player->r) < 0) {
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

void ballCrash(ball* a, ball* b) {
  if (!CheckCollisionCircles(a->realPos, a->r, b->realPos, b->r)) {
    return;
  }
  // Static collision
  //  Distance between ball centers
  float fDistance = Vector2Distance(a->realPos, b->realPos);

  // Calculate displacement required
  float fOverlap = 0.5f * (fDistance - a->r - b->r);

  // Displace Current Ball away from collision
  a->realPos.x -= fOverlap * (a->realPos.x - b->realPos.x) / fDistance;
  a->realPos.y -= fOverlap * (a->realPos.y - b->realPos.y) / fDistance;

  // Displace Target Ball away from collision
  b->realPos.x += fOverlap * (a->realPos.x - b->realPos.x) / fDistance;
  b->realPos.y += fOverlap * (a->realPos.y - b->realPos.y) / fDistance;

  // dynamic collison
  // float fDistance = Vector2Distance(a->realPos, b->realPos);
  // Normal

  float nx = (b->realPos.x - a->realPos.x) / fDistance;
  float ny = (b->realPos.y - a->realPos.y) / fDistance;

  float kx = (a->vol.x - b->vol.x);
  float ky = (a->vol.y - b->vol.y);
  float p = 2.0 * (nx * kx + ny * ky) / (a->mass + b->mass);
  a->vol.x = a->vol.x - p * b->mass * nx;
  a->vol.y = a->vol.y - p * b->mass * ny;
  b->vol.x = b->vol.x + p * a->mass * nx;
  b->vol.y = b->vol.y + p * a->mass * ny;
}

void playerInputs() {
  if (IsKeyDown(KEY_S)) {
    playerOne.body.accel.x = -ACCELARATION;
  }
  if (IsKeyDown(KEY_F)) {
    playerOne.body.accel.x = ACCELARATION;
  }
  if (IsKeyDown(KEY_E)) {
    playerOne.body.accel.y = -ACCELARATION;
  }
  if (IsKeyDown(KEY_D)) {
    playerOne.body.accel.y = ACCELARATION;
  }
  if (IsKeyDown(KEY_SPACE) && playerOne.slapping == false && Vector2Distance(playerOne.fistL.realPos, playerOne.fistL.homePos) < 10.0) {
    playerOne.fistL.accel = Vector2Scale(playerOne.fistL.direction, 40.0);
    playerOne.fistR.accel = Vector2Scale(playerOne.fistR.direction, 40.0);
    playerOne.slapping = true;
    playerOne.slapStartTime = gameTime;
  }
}

void cpuInputs() {
  if (GetRandomValue(0, 100) > 50) {
    return;
  }
  if (playerOne.body.realPos.x < playerTwo.body.realPos.x) {
    playerTwo.body.accel.x = -ACCELARATION;
  }
  if (playerOne.body.realPos.x > playerTwo.body.realPos.x) {
    playerTwo.body.accel.x = ACCELARATION;
  }
  if (playerOne.body.realPos.y < playerTwo.body.realPos.y) {
    playerTwo.body.accel.y = -ACCELARATION;
  }
  if (playerOne.body.realPos.y > playerTwo.body.realPos.y) {
    playerTwo.body.accel.y = ACCELARATION;
  }
  if (GetRandomValue(0, 100) < 100) {
    return;
  }
  if (playerTwo.slapping == false && Vector2Distance(playerTwo.fistL.realPos, playerTwo.fistL.homePos) < 10.0) {
    playerTwo.fistL.accel = Vector2Scale(playerTwo.fistL.direction, 40.0);
    playerTwo.fistR.accel = Vector2Scale(playerTwo.fistR.direction, 40.0);
    playerTwo.slapping = true;
    playerTwo.slapStartTime = gameTime;
  }
}

void fistInputs(sumo* a) {
  if (a->slapping == false) {
    if (a->fistL.homePos.x < a->fistL.realPos.x) {
      a->fistL.accel.x = -FIST_ACCELARATION;
    }
    if (a->fistL.homePos.x > a->fistL.realPos.x) {
      a->fistL.accel.x = FIST_ACCELARATION;
    }
    if (a->fistL.homePos.y < a->fistL.realPos.y) {
      a->fistL.accel.y = -FIST_ACCELARATION;
    }
    if (a->fistL.homePos.y > a->fistL.realPos.y) {
      a->fistL.accel.y = FIST_ACCELARATION;
    }

    if (a->fistR.homePos.x < a->fistR.realPos.x) {
      a->fistR.accel.x = -FIST_ACCELARATION;
    }
    if (a->fistR.homePos.x > a->fistR.realPos.x) {
      a->fistR.accel.x = FIST_ACCELARATION;
    }
    if (a->fistR.homePos.y < a->fistR.realPos.y) {
      a->fistR.accel.y = -FIST_ACCELARATION;
    }
    if (a->fistR.homePos.y > a->fistR.realPos.y) {
      a->fistR.accel.y = FIST_ACCELARATION;
    }
  } else {
    if (gameTime < (a->slapStartTime + 0.2)) {
      a->fistL.accel = Vector2Scale(a->fistL.direction, 40.0);
      a->fistR.accel = Vector2Scale(a->fistR.direction, 40.0);
    }
    if (gameTime > (a->slapStartTime + 0.25)) {
      a->slapping = false;
    }
  }
}

void fistUpdate(sumo* a, sumo* b) {
  Vector2 distanceToTarget = Vector2Subtract(b->body.realPos, a->body.realPos);
  float angleToTarget = atan(-distanceToTarget.y / distanceToTarget.x);
  float fistAngleL = angleToTarget + 0.785398;
  float fistAngleR = angleToTarget - 0.785398;

  if (distanceToTarget.x < 0) {
    a->fistL.direction = (Vector2){-(a->body.r * cos(angleToTarget)), (a->body.r * sin(angleToTarget))};
    a->fistR.direction = a->fistL.direction;
    a->fistL.homePos = (Vector2){-(a->body.r * cos(fistAngleL)), (a->body.r * sin(fistAngleL))};
    a->fistR.homePos = (Vector2){-(a->body.r * cos(fistAngleR)), (a->body.r * sin(fistAngleR))};
  } else {
    a->fistL.direction = (Vector2){(a->body.r * cos(angleToTarget)), -(a->body.r * sin(angleToTarget))};
    a->fistR.direction = a->fistL.direction;
    a->fistL.homePos = (Vector2){(a->body.r * cos(fistAngleL)), -(a->body.r * sin(fistAngleL))};
    a->fistR.homePos = (Vector2){(a->body.r * cos(fistAngleR)), -(a->body.r * sin(fistAngleR))};
  }
  a->body.direction = Vector2Add(a->body.realPos, a->fistL.direction);
  a->fistL.homePos = Vector2Add(a->body.realPos, a->fistL.homePos);
  a->fistR.homePos = Vector2Add(a->body.realPos, a->fistR.homePos);
}
