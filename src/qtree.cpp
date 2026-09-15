
#include <vector>

class point {
private:
  double x = 0;
  double y = 0;

public:
  point(double _x, double _y) : x(_x), y(_y) {}

  double getX() const { return x; }
  double getY() const { return y; }
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

public:
  ~node() {
    delete nw;
    delete ne;
    delete sw;
    delete se;
  }

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

    // Node has room.
    if (points.size() < N_MAX) {
      points.push_back(p);
      return;
    }

    // Node is full.
    if (nw == nullptr) {
      subdivide();
    }

    // Move existing point down.
    point old_point = points[0];
    points.clear();

    get_quadrant(old_point)->insert(old_point);

    // Insert new point.
    get_quadrant(p)->insert(p);
  }
};

class qtree {
private:
  node *root;

public:
  qtree(rect box) : root(new node(box)) {}

  ~qtree() { delete root; }

  void qinsert(const point &p) { root->insert(p); }
};
