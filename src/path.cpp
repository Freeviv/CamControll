#include "path.h"

#include "user_interface.h"

#define	SYS_CPU_80MHz   80
#define	SYS_CPU_160MHz  160
#define OVERCLOCK

Path::Path()
{
    values_added = false;
}

Path::~Path()
{

}

void Path::add_point(Point p)
{
    m_x.push_back(p.x);
    m_y.push_back(p.y);
    values_added = true;
}

double Path::get_value(double x)
{
#ifdef OVERCLOCK
    system_update_cpu_freq(SYS_CPU_160MHz);
#endif
    if(!values_added)
    {
        comb_sort();
        m_spline.set_points(m_x,m_y);
    }
    double r = m_spline(x);
#ifdef OVERCLOCK
    system_update_cpu_freq(SYS_CPU_80MHz);
#endif
    return r;
}

void Path::comb_sort()
{
    uint32_t gap = m_x.size();
    bool swapped = true;
    while(gap > 1 || swapped == true)
    {
        gap = (gap *10) / 13;
        swapped = false;
        for(uint32_t i = 0; i < m_x.size() - gap; ++i)
        {
            if(m_x.at(i) > m_x.at(i + gap))
            {
                double tmp = m_x.at(i);
                m_x.at(i) = m_x.at(i + gap);
                m_x.at(i + gap) = tmp;

                tmp = m_y.at(i);
                m_y.at(i) = m_y.at(i + gap);
                m_y.at(i + gap) = tmp;

                swapped = true;
            }
        }
    }
}
