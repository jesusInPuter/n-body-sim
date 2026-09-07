#include "raylib.h"

void render(float Ax, float Ay, float Bx, float By) {
  BeginDrawing();

  ClearBackground(BLACK);

  DrawCircle(400 + Ax, 300 + Ay, 5, WHITE);
  DrawCircle(400 + Bx, 300 + By, 5, WHITE);

  EndDrawing();
}
