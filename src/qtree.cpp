#pragma once
#include "raylib.h"
#include <vector>

class point {
private:
  double x = 0;
  double y = 0;

public:
  point(double _x, double _y) : x(_x), y(_y) {}

  double getX() const { return x; }
  double getY() const { return y; }

  point operator+(const point &other) const {
    return {x + other.x, y + other.y};
  }

  point operator*(double scalar) const { return {x * scalar, y * scalar}; }
  point operator/(double scalar) const { return {x / scalar, y / scalar}; }
};

class rect {
private:
  double x_start;
  double y_start;
  double width;
  double height;

public:
  rect(double _x, double _y, double _width, double _height)
      : x_start(_x), y_start(_y), width(_width), height(_height) {}

  point relative_origin_for_box() { return {x_start, y_start}; }

  friend class node;
};

class node {
private:
  const int N_MAX = 1;
  std::vector<point> points;
  rect boundary;
  node *nw;
  node *ne;
  node *sw;
  node *se;
  point center_of_mass = {0, 0};
  double totalMass{0};

public:
  ~node() {
    delete nw;
    delete ne;
    delete sw;
    delete se;
  }
  void draw() const {
    DrawRectangleLines(boundary.x_start, boundary.y_start, boundary.width,
                       boundary.height, DARKGRAY);

    if (nw)
      nw->draw();

    if (ne)
      ne->draw();

    if (sw)
      sw->draw();

    if (se)
      se->draw();
  }
  point coord(node &p) const { return p.points[0]; }

  node(rect _boundary)
      : boundary(_boundary), nw(nullptr), ne(nullptr), sw(nullptr),
        se(nullptr) {}

  bool contains(const point &p) const {
    return p.getX() >= boundary.x_start &&
           p.getX() < boundary.x_start + boundary.width &&
           p.getY() >= boundary.y_start &&
           p.getY() < boundary.y_start + boundary.height;
  }

  void subdivide() {
    nw = new node(rect(boundary.x_start, boundary.y_start, boundary.width / 2,
                       boundary.height / 2));

    ne = new node(rect(boundary.x_start + boundary.width / 2, boundary.y_start,
                       boundary.width / 2, boundary.height / 2));

    sw = new node(rect(boundary.x_start, boundary.y_start + boundary.height / 2,
                       boundary.width / 2, boundary.height / 2));

    se = new node(rect(boundary.x_start + boundary.width / 2,
                       boundary.y_start + boundary.height / 2,
                       boundary.width / 2, boundary.height / 2));
  }

  node *get_quadrant(const point &p) {

    double x_mid = boundary.x_start + boundary.width / 2;

    double y_mid = boundary.y_start + boundary.height / 2;

    if (p.getX() < x_mid && p.getY() < y_mid)
      return nw;

    if (p.getX() >= x_mid && p.getY() < y_mid)
      return ne;

    if (p.getX() < x_mid && p.getY() >= y_mid)
      return sw;

    return se;
  }

  void insert(const point &p) {

    if (!contains(p)) {
      return;
    }

    // Node has room
    if (points.size() < N_MAX) {
      points.push_back(p);
      return;
    }

    // Node is full
    if (nw == nullptr) {
      subdivide();
    }

    // Move existing point down
    point old_point = points[0];
    points.clear();

    get_quadrant(old_point)->insert(old_point);

    // Insert new point
    get_quadrant(p)->insert(p);
  }

  bool leaf(node &p) const { return p.nw == nullptr; }
  bool leaf() const { return nw == nullptr; }
  bool empty() const { return points.empty() && nw == nullptr; }
  point get_point() const { return points[0]; }
  point get_center_of_mass() const { return center_of_mass; }
  double get_total_mass() const { return totalMass; }

  node *get_nw() const { return nw; }
  node *get_ne() const { return ne; }
  node *get_sw() const { return sw; }
  node *get_se() const { return se; }

  point get_center_of_mass(node &p) const {

    // if that node  is a leaf we get can do coords of
    // base case:

    if (leaf(p)) {
      return {coord(p)};

      // if the node is not a leaf we nw->find point coord take avg of all the

    } else {

      point nw_com = get_center_of_mass(*nw);
      point ne_com = get_center_of_mass(*ne);
      point sw_com = get_center_of_mass(*sw);
      point se_com = get_center_of_mass(*se);

      return (nw_com + ne_com + sw_com + se_com) / 4;
    }
  }

  double get_width() const { return boundary.width; }

  double get_height() const { return boundary.height; }
};

class qtree {
private:
  node *root;
  rect boundary;

public:
  qtree(rect box) : root(new node(box)), boundary(box) {}

  ~qtree() { delete root; }

  void qinsert(const point &p) { root->insert(p); }

  void clear() {
    delete root;
    root = new node(boundary);
  }

  node *get_root() const { return root; }
};
