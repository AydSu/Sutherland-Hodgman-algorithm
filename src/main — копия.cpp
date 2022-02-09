#include <iostream>
#include <pybind11/pybind11.h>

using namespace std;
#define ERR_OUT -999;
#include <vector>

struct pt
{
  float x, y;
};

const double EPS = 1E-9;

inline int det(int a, int b, int c, int d)
{
  return a * d - b * c;
}

inline bool between(int a, int b, double c)
{
  return min(a, b) <= c + EPS && c <= max(a, b) + EPS;
}

inline bool intersect_1(int a, int b, int c, int d)
{
  if (a > b)
    swap(a, b);
  if (c > d)
    swap(c, d);
  return max(a, c) <= min(b, d);
}

struct pt intersect(pt a, pt b, pt c, pt d)
{
  int A1 = a.y - b.y, B1 = b.x - a.x, C1 = -A1 * a.x - B1 * a.y;
  int A2 = c.y - d.y, B2 = d.x - c.x, C2 = -A2 * c.x - B2 * c.y;
  int zn = det(A1, B1, A2, B2);
  struct pt out;
  if (zn != 0)
  {

    out.x = -det(C1, B1, C2, B2) * 1. / zn;
    out.y = -det(A1, C1, A2, C2) * 1. / zn;

    return out;
    // return between (a.x, b.x, x) && between (a.y, b.y, y)
    // 	&& between (c.x, d.x, x) && between (c.y, d.y, y);
  }
  else
  {
    out.x = ERR_OUT;
    out.y = ERR_OUT;
    return out;
    // return det (A1, C1, A2, C2) == 0 && det (B1, C1, B2, C2) == 0
    // 	&& intersect_1 (a.x, b.x, c.x, d.x)
    // 	&& intersect_1 (a.y, b.y, c.y, d.y);
  }
}

struct pt clipper(double x)
{
  struct pt clip;
  clip.x = x;
  clip.y = 1 * x - 0.5;
  return clip;
}

vector<struct pt> vertexes;
vector<struct pt> crop_vertexes;

void calc()
{
  struct pt a, b, c, d;
  a.x = 0;
  a.y = 0;
  b.x = 0;
  b.y = 1;
  c.x = 1;
  c.y = 1;
  d.x = 1;
  d.y = 0;
  vertexes.push_back(a);
  vertexes.push_back(b);
  vertexes.push_back(c);
  vertexes.push_back(d);

  struct pt clip_1, clip_2;
  clip_1 = clipper(-100);
  clip_2 = clipper(100);

  for (int i = 0; i < vertexes.size(); i++)
  {
    // cout << "x = " << vertexes.at(i).x << ", y = " << vertexes.at(i).y << endl;
    struct pt vertex_begin = vertexes.at(i);
    struct pt vertex_end;
    if (i < vertexes.size() - 1)
    {
      vertex_end = vertexes.at(i + 1);
    }
    else
    {
      vertex_end = vertexes.at(0);
    }
    // struct pt clipper_vb = clipper(vertex_begin.x);
    // struct pt clipper_ve = clipper(vertex_end.x);

    if (vertex_begin.y <= clipper(vertex_begin.x).y && vertex_end.y <= clipper(vertex_end.x).y)
    {
      crop_vertexes.push_back(vertex_begin);
    }
    else if (vertex_begin.y < clipper(vertex_begin.x).y && vertex_end.y > clipper(vertex_end.x).y)
    {
      crop_vertexes.push_back(vertex_begin);
      struct pt intersection;
      intersection = intersect(vertex_begin, vertex_end, clip_1, clip_2);
      crop_vertexes.push_back(intersection);
      cout << intersection.x << intersection.y << endl;
    }
    else if (vertex_begin.y > clipper(vertex_begin.x).y && vertex_end.y < clipper(vertex_end.x).y)
    {
      struct pt intersection;
      intersection = intersect(vertex_begin, vertex_end, clip_1, clip_2);
      crop_vertexes.push_back(intersection);
      cout << intersection.x << intersection.y << endl;
    }
    else if (vertex_begin.y == clipper(vertex_begin.x).y)
    {
      crop_vertexes.push_back(vertex_begin);
    }
    else
    {
    }
  }

  struct pt z;

  // z = intersect(a, b, c, d);
  // z = vertexes.back();
  for (int i = 0; i < crop_vertexes.size(); i++)
  {
    cout << "x = " << crop_vertexes.at(i).x << ", y = " << crop_vertexes.at(i).y;
  }

  // cout << "x = " << a.x << ", y = " << a.y << z.y << endl;
}

PYBIND11_MODULE(CModule, m)
{
  m.doc() = "My prog documentation"; // optional module docstring
  m.def("calc", &calc, "A function that calculates clipping");
}

// g++ main.cpp -o hello
// приравнять прямые и проверить принадлежит ли точка пересечения этим отрезкам