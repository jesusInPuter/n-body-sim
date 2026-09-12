#include "raylib.h"
#include <glm/detail/qualifier.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>

using namespace glm;

// constexpr double G = 6.67430e-11;
constexpr float G = 1.0f;

struct body {
  vec3 position;
  vec3 velocity;
  vec3 acceleration;
  float mass;
  float brightness;
};

vec3 force_on(const body &A, const body &B) {
  vec3 displacement = B.position - A.position;

  float r = length(displacement);

  if (r < 0.0001f)
    return vec3(0.0f);

  float magnitude = (G * A.mass * B.mass) / (r * r);

  vec3 direction = displacement / r;

  return magnitude * direction;
}

int main() {
  InitWindow(1600, 900, "2Body");
  SetTargetFPS(60);

  // body A{};
  // body B{};

  // A.position = vec3(200.0f, 300.0f, 0.0f);
  // A.velocity = vec3(0.0f);
  // A.acceleration = vec3(0.0f);
  // A.mass = 100000.0f;
  //
  // B.position = vec3(600.0f, 300.0f, 0.0f);
  // B.velocity = vec3(0.0f, 10.0f, 0.0f);
  // B.acceleration = vec3(0.0f);
  // B.mass = 1000.0f;
  float dt = 0.05f;

  int n = 1000;

  std::vector<body> bodies;

  body sun{};
  sun.position = vec3(800.0f, 450.0f, 0.0f);
  sun.velocity = vec3(0.0f);
  sun.acceleration = vec3(0.0f);
  sun.mass = 100000.0f;
  sun.brightness = 1;

  bodies.push_back(sun);
  for (int i = 0; i < 1000; i++) {

    float angle = ((float)rand() / RAND_MAX) * 2.0f * PI;

    float r = 100.0f + ((float)rand() / RAND_MAX) * 300.0f;

    vec3 offset(cos(angle) * r, sin(angle) * r, 0.0f);

    body p{};

    p.brightness = 0.3f + ((float)rand() / RAND_MAX) * 0.7f;
    p.position = sun.position + offset;

    vec3 tangent(-sin(angle), cos(angle), 0.0f);

    float orbital_speed = sqrt(G * sun.mass / r);

    p.velocity = tangent * orbital_speed;

    p.acceleration = vec3(0.0f);

    p.mass = 1.0f;

    bodies.push_back(p);
  }

  while (!WindowShouldClose()) {

    // vec3 force_A = force_on(A, B);
    //
    // vec3 force_B = force_on(B, A);
    //
    // A.acceleration = force_A / A.mass;
    //
    // B.acceleration = force_B / B.mass;
    //
    // A.velocity += A.acceleration * dt;
    //
    // B.velocity += B.acceleration * dt;
    //
    // A.position += A.velocity * dt;
    //
    // B.position += B.velocity * dt;
    //

    for (int i = 0; i < n; i++) {

      bodies[i].acceleration = vec3(0.0f);

      for (int j = 0; j < n; j++) {

        if (i == j)
          continue;

        bodies[i].acceleration +=
            force_on(bodies[i], bodies[j]) / bodies[i].mass;
      }
    }

    for (int i = 0; i < n; i++) {

      bodies[i].velocity += bodies[i].acceleration * dt;

      bodies[i].position += bodies[i].velocity * dt;
    }

    BeginDrawing();

    ClearBackground(BLACK);

    for (int i = 0; i < n; i++) {
      DrawPixel(bodies[i].position.x, bodies[i].position.y, WHITE);
      float b = bodies[i].brightness;

      Vector2 p = {bodies[i].position.x, bodies[i].position.y};
      DrawCircleV(p, 10.0f, Fade(WHITE, 0.02f * b));
      DrawCircleV(p, 7.0f, Fade(WHITE, 0.05f * b));
      DrawCircleV(p, 4.0f, Fade(WHITE, 0.12f * b));
      DrawCircleV(p, 1.5f, Fade(WHITE, b));
    }

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
