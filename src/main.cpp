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
  InitWindow(800, 600, "2Body");
  SetTargetFPS(60);

  body A{};
  body B{};

  A.position = vec3(200.0f, 300.0f, 0.0f);
  A.velocity = vec3(0.0f);
  A.acceleration = vec3(0.0f);
  A.mass = 100000.0f;

  B.position = vec3(600.0f, 300.0f, 0.0f);
  B.velocity = vec3(0.0f, 10.0f, 0.0f);
  B.acceleration = vec3(0.0f);
  B.mass = 1000.0f;

  float dt = 0.05f;

  /*
  int n = 1000;

  std::vector<body> bodies;

  for (int i = 0; i < n; i++) {

      body a{};

      a.position = vec3(
          rand() % 800,
          rand() % 600,
          rand() % 300
      );

      a.velocity = vec3(0.0f);
      a.acceleration = vec3(0.0f);

      a.mass = 1000.0f;

      bodies.push_back(a);
  }
  */

  while (!WindowShouldClose()) {

    vec3 force_A = force_on(A, B);

    vec3 force_B = force_on(B, A);

    A.acceleration = force_A / A.mass;

    B.acceleration = force_B / B.mass;

    A.velocity += A.acceleration * dt;

    B.velocity += B.acceleration * dt;

    A.position += A.velocity * dt;

    B.position += B.velocity * dt;

    /*
    for (int i = 0; i < n; i++) {

        bodies[i].acceleration = vec3(0.0f);

        for (int j = 0; j < n; j++) {

            if (i == j)
                continue;

            bodies[i].acceleration +=
                force_on(bodies[i], bodies[j])
                / bodies[i].mass;
        }
    }


    for (int i = 0; i < n; i++) {

        bodies[i].velocity +=
            bodies[i].acceleration * dt;

        bodies[i].position +=
            bodies[i].velocity * dt;
    }
    */

    BeginDrawing();

    ClearBackground(BLACK);

    // Draw A
    DrawCircle((int)A.position.x, (int)A.position.y, 8, WHITE);

    DrawCircle((int)B.position.x, (int)B.position.y, 8, WHITE);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
