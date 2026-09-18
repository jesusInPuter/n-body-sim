#include "qtree.cpp"
#include "raylib.h"
#include <glm/detail/qualifier.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>

using namespace glm;

// constexpr double G = 6.67430e-11;
constexpr float G = 1.0f;
constexpr float theta = 0.1f;

struct body {
  vec3 position;
  vec3 velocity;
  vec3 acceleration;
  float mass;
  float brightness;
};

vec3 force_by(node &p, const body &b) {

  // empty node
  if (p.empty()) {
    return vec3(0.0f);
  }

  // leaf calculate exact force
  if (p.leaf()) {

    point q = p.get_point();

    vec3 other_position{static_cast<float>(q.getX()),
                        static_cast<float>(q.getY()), 0.0f};

    vec3 displacement = other_position - b.position;

    float r = length(displacement);

    if (r < 0.0001f) {
      return vec3(0.0f);
    }

    float magnitude = G * b.mass * 1.0f / (r * r);

    vec3 direction = displacement / r;

    return magnitude * direction;
  }

  // Internal node

  point com = p.get_center_of_mass();

  vec3 com_position{static_cast<float>(com.getX()),
                    static_cast<float>(com.getY()), 0.0f};

  vec3 displacement = com_position - b.position;

  float d = length(displacement);

  float s = std::max(p.get_width(), p.get_height());

  if (s / d < theta) {

    float magnitude = G * b.mass * p.get_total_mass() / (d * d);

    vec3 direction = displacement / d;

    return magnitude * direction;
  }

  // if too close open the node
  vec3 force{0.0f};

  if (p.get_nw())
    force += force_by(*p.get_nw(), b);

  if (p.get_ne())
    force += force_by(*p.get_ne(), b);

  if (p.get_sw())
    force += force_by(*p.get_sw(), b);

  if (p.get_se())
    force += force_by(*p.get_se(), b);

  return force;
}

// vec3 force_on(const body &A, const body &B) {
//   vec3 displacement = B.position - A.position;
//
//   float r = length(displacement);
//
//   if (r < 0.0001f)
//     return vec3(0.0f);
//
//   float magnitude = (G * A.mass * B.mass) / (r * r);
//
//   vec3 direction = displacement / r;
//
//   return magnitude * direction;
// }
//
int main() {
  InitWindow(1600, 900, "2Body");
  SetTargetFPS(60);

  float dt = 0.05f;
  int n = 10'000;

  std::vector<body> bodies;
  bodies.reserve(n);

  vec3 center(800.0f, 450.0f, 0.0f);

  float max_radius = 400.0f;

  for (int i = 0; i < n; i++) {
    float u = (float)rand() / RAND_MAX;
    float v = (float)rand() / RAND_MAX;

    float r = max_radius * sqrt(u);
    float angle = v * 2.0f * PI;

    float x = cos(angle) * r;
    float y = sin(angle) * r;

    vec3 tangent(-sin(angle), cos(angle), 0.0f);

    float speed = 45.0f * sqrt(1.0f / (r + 50.0f)) * 20.0f;

    speed += ((float)rand() / RAND_MAX - 0.5f) * 3.0f;

    body p{};

    p.position = center + vec3(x, y, 0.0f);
    p.velocity = tangent * speed;
    p.acceleration = vec3(0.0f);
    p.mass = 1.0f;

    p.brightness = 0.2f + 0.8f * (1.0f - r / max_radius);

    bodies.push_back(p);
  }

  qtree qt(rect(0, 0, 1600, 900));
  while (!WindowShouldClose()) {

    qt.clear();

    for (const auto &b : bodies)
      qt.qinsert({b.position.x, b.position.y});

    for (auto &b : bodies)
      b.acceleration = force_by(*qt.get_root(), b) / b.mass;

    for (auto &b : bodies) {
      b.velocity += b.acceleration * dt;
      b.position += b.velocity * dt;
    }

    BeginDrawing();

    ClearBackground(BLACK);
    DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 20, WHITE);
    DrawText(TextFormat("Particles: %d", n), 10, 35, 20, WHITE);
    DrawText(TextFormat("Theta: %.2f", theta), 10, 60, 20, WHITE);
    DrawText(TextFormat("dt: %.3f", dt), 10, 85, 20, WHITE);
    for (const auto &b : bodies) {
      DrawPixel(b.position.x, b.position.y, WHITE);

      float brightness = b.brightness;
      Vector2 p = {b.position.x, b.position.y};

      DrawCircleV(p, 10.0f, Fade(WHITE, 0.02f * brightness));
      DrawCircleV(p, 7.0f, Fade(WHITE, 0.05f * brightness));
      DrawCircleV(p, 4.0f, Fade(WHITE, 0.12f * brightness));
      DrawCircleV(p, 1.5f, Fade(WHITE, brightness));
    }

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
