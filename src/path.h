#ifndef PATH_H
#define PATH_H

#include "spline/src/spline.h"

#include <vector>

typedef struct double_point {
    double x;
    double y;
} Point;

class Path
{
public:
    Path();
    ~Path();
    void add_point(Point p);
    double get_value(double x);

private:
    std::vector<double> m_x;
    std::vector<double> m_y;
    tk::spline m_spline;
    bool values_added;

    void comb_sort();
};
#endif // PATH_H