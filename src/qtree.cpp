// points
class point {
private:
  double x = 0, y = 0;

public:
  point(double _x, double _y) : x(_x), y(_y) {}
};

// rectangle
class rect {
private:
  double x_start, y_start;
  double width, height;

public:
  rect(double _x, double _y, double _width, double _height)
      : x_start(_x), y_start(_y), width(_width), height(_height) {}
};

// node
class node {
private:
  rect boundary;
  node *nw;
  node *ne;
  node *sw;
  node *se;

public:
  node(rect _boundary)
      : boundary(_boundary), nw(nullptr), ne(nullptr), sw(nullptr),
        se(nullptr) {}
};

// tree
class qtree {
private:
  node *root;

public:
  qtree(rect box) : root(new node(box)) {}
  ~qtree() { delete root; }
};
