struct Point {
  GLdouble x, y, z;
  Point() : x(0), y(0), z(0) {}
  Point(GLdouble X, GLdouble Y, GLdouble Z) :
      x(X), y(Y), z(Z) {}
  Point operator + (const Point& u) const {
    return Point(x + u.x, y + u.y, z + u.z); }
  Point operator - (const Point& u) const {
    return Point(x - u.x, y - u.y, z - u.z); }
  Point operator * (const GLdouble u) const {
    return Point(x * u, y * u, z * u); }
  Point operator / (const GLdouble u) const {
    return Point(x / u, y / u, z / u); }
};
GLdouble dot(Point a, Point b) {
  return a.x * b.x + a.y * b.y + a.z * b.z; }
Point cross(Point a, Point b) {
  return Point(a.y*b.z - a.z*b.y,
     a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x); }
GLdouble length(Point a) { return sqrt(dot(a, a));}
GLdouble distance(Point a, Point b) {
  return length(a-b); }
Point unit(const Point &p) { return p/length(p); }
// Rotate p around axis x, with angle radians.
Point rotate(Point p, Point axis, GLdouble angle) {
  axis = unit(axis);Point comp1 = p * cos(angle);
  Point comp2 = axis*(1-cos(angle))*dot(axis,p);
  Point comp3 = cross(axis, p) * sin(angle);
  return comp1 + comp2 + comp3;
}