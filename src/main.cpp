#include "raylib.h"
#include <cstdlib>
#include <glm/detail/qualifier.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

// constexpr double G = 6.67430e-11;
constexpr double G = 1;
using namespace glm;

struct body {
  vec3 position;
  vec3 velocity;
  vec3 acceleration;
  float mass;
};

vec3 force_on(const body &A, const body &B) {
  vec3 displacement = B.position - A.position;
  float r = glm::length(displacement);
  float magnitude = (G * A.mass * B.mass) / (r * r);
  vec3 direction = glm::normalize(displacement);
  return magnitude * direction;
}

void render(float Ax, float Ay, float Bx, float By);

int main() {

  InitWindow(800, 600, "2body");
  SetTargetFPS(120);
  // body A, B;
  // A.position = vec3(0, 0, 0);
  // A.velocity = vec3(0, 0, 0);
  // A.acceleration = vec3(0, 0, 0);
  // A.mass = 1000;
  //
  // B.position = vec3(100, 0, 0);
  // B.velocity = vec3(0, 0, 0);
  // B.acceleration = vec3(0, 0, 0);
  // B.mass = 1000;
  //
  int n = 1000;

  std::vector<body> bodies;

  for (int i = 0; i < n; i++) {
    body a;
    a.position = vec3(rand() % 800, rand() % 600, rand() % 300);
    a.mass = 1000;
    bodies.push_back(a);
  }

  while (!WindowShouldClose()) {

    // float dt = 1;
    // A.acceleration = force_on(A, B) / A.mass;
    // A.velocity += A.acceleration * dt;
    // A.position += A.velocity * dt;
    //
    // B.acceleration = force_on(B, A) / B.mass;
    // B.velocity += B.acceleration * dt;
    // B.position += B.velocity * dt;
    //
    // render(A.position.x, A.position.y, B.position.x, B.position.y);

    float dt = 0.05f;

    for (int i = 0; i < n; i++) {

      bodies[i].acceleration = vec3(0);

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
      DrawCircle(bodies[i].position.x, bodies[i].position.y, 3, WHITE);
    }

    EndDrawing();
  }

  return 0;
}
