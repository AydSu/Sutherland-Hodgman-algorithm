#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

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

pt intersect(pt a, pt b, pt c, pt d)
{
  int A1 = a.y - b.y, B1 = b.x - a.x, C1 = -A1 * a.x - B1 * a.y;
  int A2 = c.y - d.y, B2 = d.x - c.x, C2 = -A2 * c.x - B2 * c.y;
  int zn = det(A1, B1, A2, B2);
  pt out;
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
double k, b;

double clip_line(double x)
{
  return k * x + b;
}

pt clipper(double x)
{
  pt clip;
  clip.x = x;
  clip.y = clip_line(x);
  return clip;
}

vector<pt> calc_c(vector<pt> vertexes, vector<double> clipper_params)
{
  k = clipper_params.at(0);
  b = clipper_params.at(1);
  vector<pt> crop_vertexes;

  pt clip_1, clip_2;
  clip_1 = clipper(-100);
  clip_2 = clipper(100);

  for (int i = 0; i < vertexes.size(); i++)
  {
    // cout << "x = " << vertexes.at(i).x << ", y = " << vertexes.at(i).y << endl;
    pt vertex_begin = vertexes.at(i);
    pt vertex_end;
    if (i < vertexes.size() - 1)
    {
      vertex_end = vertexes.at(i + 1);
    }
    else
    {
      vertex_end = vertexes.at(0);
    }

    if (vertex_begin.y <= clipper(vertex_begin.x).y && vertex_end.y <= clipper(vertex_end.x).y)
    {
      crop_vertexes.push_back(vertex_begin);
    }
    else if (vertex_begin.y < clipper(vertex_begin.x).y && vertex_end.y > clipper(vertex_end.x).y)
    {
      crop_vertexes.push_back(vertex_begin);
      pt intersection;
      intersection = intersect(vertex_begin, vertex_end, clip_1, clip_2);
      crop_vertexes.push_back(intersection);
      // cout << intersection.x << intersection.y << endl;
    }
    else if (vertex_begin.y > clipper(vertex_begin.x).y && vertex_end.y < clipper(vertex_end.x).y)
    {
      pt intersection;
      intersection = intersect(vertex_begin, vertex_end, clip_1, clip_2);
      crop_vertexes.push_back(intersection);
      // cout << intersection.x << intersection.y << endl;
    }
    else if (vertex_begin.y == clipper(vertex_begin.x).y)
    {
      crop_vertexes.push_back(vertex_begin);
    }
  }
  return crop_vertexes;
}

vector<double> transformFromPtsToList(vector<pt> crop_vertexes)
{
  vector<double> vertexes_list;
  for (int i = 0; i < crop_vertexes.size(); i++)
  {
    vertexes_list.push_back(crop_vertexes.at(i).x);
    vertexes_list.push_back(crop_vertexes.at(i).y);
    // cout << "x = " << crop_vertexes.at(i).x << ", y = " << crop_vertexes.at(i).y;
  }
  return vertexes_list;
}

vector<pt> transformFromListToPts(vector<double> input_vertexes)
{
  pt point;
  vector<pt> output_vetrexes;
  for (int i = 0; i < input_vertexes.size(); i = i + 2)
  {
    point.x = input_vertexes.at(i);
    point.y = input_vertexes.at(i + 1);
    output_vetrexes.push_back(point);
  }
  return output_vetrexes;
}

vector<double> calc(vector<double> input_vertexes, vector<double> clipper_params)
{
  vector<pt> vertexes = transformFromListToPts(input_vertexes);
  vector<pt> crop_vertexes = calc_c(vertexes, clipper_params);

  vector<double> vertexes_list;
  vertexes_list = transformFromPtsToList(crop_vertexes);

  return vertexes_list;
}

// int main()
// {
//   vector<pt> crop_vertexes;
//   crop_vertexes = calc();
//   cout << "x = " << crop_vertexes.size();
//   for (int i = 0; i < crop_vertexes.size(); i++)
//   {
//     cout << "x = " << crop_vertexes.at(i).x << ", y = " << crop_vertexes.at(i).y;
//   }
// }

PYBIND11_MODULE(CModule, m)
{
  m.doc() = "My prog documentation"; // optional module docstring
  m.def("calc", &calc, "A function that calculates clipping");
}

// g++ main.cpp -o hello
// ???????????????????? ???????????? ?? ?????????????????? ?????????????????????? ???? ?????????? ?????????????????????? ???????? ????????????????