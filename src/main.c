#include <math.h>

#include "raylib.h"
#include "raymath.h"

#define MAX_SPEED 200.0f
#define ACCELARATION 200.0f
#define FIST_ACCELARATION 1000.0f
#define FRICTION 0.97f

typedef struct sumo {
  Vector2 pos;
  Vector2 vol;
  Vector2 accel;
  float mass;
  float r;
  Color color;
  bool slapping;
} sumo;

typedef struct ring {
  Vector2 pos;
  float r;
  Color color;
} ring;

sumo playerOne;
sumo playerTwo;
sumo fistPlayerOne;

ring dojo;

Vector2 fistGetToo;
Vector2 slapDirection;
Vector2 tempPos;
Vector2 temp;
Vector2 punchVector;
Vector2 tempPuchVol;

double gameTime;
double slapStartTime;

void updatePlayer(float delta, sumo* player);
void ballCrash(sumo* a, sumo* b);
void playerInputs();
void cpuInputs();
void fistInputs();

int main(void) {
  int winner = 0;
  InitWindow(800, 800, "Sumo Prime Time");
  SetTargetFPS(60);

  // player one
  playerOne.pos = (Vector2){500.0, 400.0};
  playerOne.vol = (Vector2){0.0, 0.0};
  playerOne.accel = (Vector2){0.0, 0.0};
  playerOne.mass = 500.0;
  playerOne.r = 50.0;
  playerOne.color = GREEN;
  playerOne.slapping = false;

  fistPlayerOne.pos = (Vector2){300.0, 400.0};
  fistPlayerOne.vol = (Vector2){0.0, 0.0};
  fistPlayerOne.accel = (Vector2){0.0, 0.0};
  fistPlayerOne.mass = 1000.0;
  fistPlayerOne.r = 10.0;
  fistPlayerOne.color = DARKGREEN;

  // player two
  playerTwo.pos = (Vector2){300, 300};
  playerTwo.vol = (Vector2){0.0, 0.0};
  playerTwo.accel = (Vector2){0.0, 0.0};
  playerTwo.mass = 500.0;
  playerTwo.r = 50.0;
  playerTwo.color = BLUE;
  dojo.pos = (Vector2){400, 400};
  dojo.r = 275.0;
  dojo.color = DARKGRAY;

  temp = Vector2Subtract(playerTwo.pos, playerOne.pos);
  float angelbetween = atan(-temp.y / temp.x);
  float fistAngle = angelbetween + 0.785398;
  if (temp.x < 0) {
    punchVector = (Vector2){-(playerOne.r * cos(angelbetween)), (playerOne.r * sin(angelbetween))};
    fistPlayerOne.pos = (Vector2){-(playerOne.r * cos(fistAngle)), (playerOne.r * sin(fistAngle))};
  } else {
    punchVector = (Vector2){(playerOne.r * cos(angelbetween)), -(playerOne.r * sin(angelbetween))};
    fistPlayerOne.pos = (Vector2){(playerOne.r * cos(fistAngle)), -(playerOne.r * sin(fistAngle))};
  }
  tempPos = Vector2Add(playerOne.pos, punchVector);
  fistPlayerOne.pos = Vector2Add(playerOne.pos, fistPlayerOne.pos);
  /*
  Vector2 angleMid = (Vector2){400, 400};
  Vector2 angleOne = (Vector2){200, 200};
  Vector2 angleTwo = (Vector2){200, 600};
  Vector2 angleThree = (Vector2){600, 200};
  Vector2 angleFour = (Vector2){600, 600};
  Vector2 distanceOne = Vector2Subtract(angleOne, angleMid);
  Vector2 distanceTwo = Vector2Subtract(angleTwo, angleMid);
  Vector2 distanceThree = Vector2Subtract(angleThree, angleMid);
  Vector2 distanceFour = Vector2Subtract(angleFour, angleMid);
  float angleBetweenOne = atan(-distanceOne.y / distanceOne.x);
  float angleBetweenTwo = atan(-distanceTwo.y / distanceTwo.x);
  float angleBetweenThree = atan(-distanceThree.y / distanceThree.x);
  float angleBetweenFour = atan(-distanceFour.y / distanceFour.x);
  */

  while (!WindowShouldClose()) {
    gameTime = GetTime();
    float deltaTime = GetFrameTime();
    if (winner == 0) {
      fistInputs();
      playerInputs();
      cpuInputs();
    }
    updatePlayer(deltaTime, &playerOne);
    updatePlayer(deltaTime, &playerTwo);
    updatePlayer(deltaTime, &fistPlayerOne);
    if (CheckCollisionCircles(playerOne.pos, playerOne.r, playerTwo.pos, playerTwo.r)) {
      ballCrash(&playerOne, &playerTwo);
    }
    if (CheckCollisionCircles(fistPlayerOne.pos, fistPlayerOne.r, playerTwo.pos, playerTwo.r)) {
      ballCrash(&fistPlayerOne, &playerTwo);
    }
    playerOne.pos = Vector2Add(playerOne.pos, (Vector2){playerOne.vol.x * deltaTime, playerOne.vol.y * deltaTime});
    fistPlayerOne.pos = Vector2Add(fistPlayerOne.pos, (Vector2){fistPlayerOne.vol.x * deltaTime, fistPlayerOne.vol.y * deltaTime});
    fistPlayerOne.vol = Vector2Scale(fistPlayerOne.vol, 0.9);
    playerTwo.pos = Vector2Add(playerTwo.pos, (Vector2){playerTwo.vol.x * deltaTime, playerTwo.vol.y * deltaTime});

    if (!CheckCollisionCircles(playerOne.pos, playerOne.r, dojo.pos, dojo.r - (playerOne.r * 2) + 10) && winner == 0) {
      winner = 2;
    }
    if (!CheckCollisionCircles(playerTwo.pos, playerTwo.r, dojo.pos, dojo.r - (playerTwo.r * 2) + 10) && winner == 0) {
      winner = 1;
    }
    temp = Vector2Subtract(playerTwo.pos, playerOne.pos);
    float angelbetween = atan(-temp.y / temp.x);
    float fistAngle = angelbetween + 0.785398;
    if (temp.x < 0) {
      punchVector = (Vector2){-(playerOne.r * cos(angelbetween)), (playerOne.r * sin(angelbetween))};
      fistGetToo = (Vector2){-(playerOne.r * cos(fistAngle)), (playerOne.r * sin(fistAngle))};
      slapDirection = (Vector2){-(playerOne.r * 2 * cos(fistAngle)), (playerOne.r * sin(fistAngle))};
    } else {
      punchVector = (Vector2){(playerOne.r * cos(angelbetween)), -(playerOne.r * sin(angelbetween))};
      fistGetToo = (Vector2){(playerOne.r * cos(fistAngle)), -(playerOne.r * sin(fistAngle))};
      slapDirection = (Vector2){(playerOne.r * 2 * cos(fistAngle)), -(playerOne.r * sin(fistAngle))};
    }
    tempPos = Vector2Add(playerOne.pos, punchVector);
    fistGetToo = Vector2Add(playerOne.pos, fistGetToo);
    slapDirection = Vector2Add(playerOne.pos, slapDirection);
    //  tempPos.y -= (tempPos.y - playerOne.pos.y) * 2;

    // draw shit
    BeginDrawing();
    ClearBackground(GRAY);
    DrawCircleV(dojo.pos, dojo.r, dojo.color);
    DrawCircleV(playerOne.pos, playerOne.r, playerOne.color);
    DrawCircleV(fistPlayerOne.pos, fistPlayerOne.r, fistPlayerOne.color);
    DrawCircleV(playerTwo.pos, playerTwo.r, playerTwo.color);
    DrawLineV(playerOne.pos, tempPos, BLACK);
    DrawText(TextFormat("player distance x: %f", temp.x), 10, 10, 20, LIGHTGRAY);
    DrawText(TextFormat("player sidtance y: %f", temp.y), 10, 40, 20, LIGHTGRAY);
    DrawText(TextFormat("angle: %f", angelbetween), 10, 70, 20, LIGHTGRAY);
    DrawText(TextFormat("punchVector X: %f", punchVector.x), 10, 100, 20, LIGHTGRAY);
    DrawText(TextFormat("punchVector Y: %f", punchVector.y), 10, 130, 20, LIGHTGRAY);
    DrawText(TextFormat("punchvol X: %f", tempPuchVol.x), 10, 160, 20, LIGHTGRAY);
    DrawText(TextFormat("punchvol Y: %f", tempPuchVol.y), 10, 190, 20, LIGHTGRAY);
    DrawText(TextFormat("time: %f", gameTime), 10, 220, 20, LIGHTGRAY);
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

void updatePlayer(float delta, sumo* player) {
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

void ballCrash(sumo* a, sumo* b) {
  float fDistance = Vector2Distance(a->pos, b->pos);
  // Normal
  float nx = (b->pos.x - a->pos.x) / fDistance;
  float ny = (b->pos.y - a->pos.y) / fDistance;

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
  if (IsKeyDown(KEY_SPACE) && playerOne.slapping == false) {
    fistPlayerOne.accel = Vector2Scale(punchVector, 40.0);
    tempPuchVol = fistPlayerOne.accel;
    playerOne.slapping = true;
    slapStartTime = gameTime;
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
void fistInputs() {
  if (playerOne.slapping == false) {
    if (fistGetToo.x < fistPlayerOne.pos.x) {
      fistPlayerOne.accel.x = -FIST_ACCELARATION;
    }
    if (fistGetToo.x > fistPlayerOne.pos.x) {
      fistPlayerOne.accel.x = FIST_ACCELARATION;
    }
    if (fistGetToo.y < fistPlayerOne.pos.y) {
      fistPlayerOne.accel.y = -FIST_ACCELARATION;
    }
    if (fistGetToo.y > fistPlayerOne.pos.y) {
      fistPlayerOne.accel.y = FIST_ACCELARATION;
    }
  } else {
    if (gameTime > (slapStartTime + 0.2)) {
      playerOne.slapping = false;
    }
    fistPlayerOne.accel = Vector2Scale(punchVector, 40.0);
  }
}
