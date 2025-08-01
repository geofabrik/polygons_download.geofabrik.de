/*
 * This little program computes the intersection between two .poly files,
 * read on stdin, and writes a new .poly file on stdout.
 * 
 * usage:
 * cat p1.poly p2.poly ./intersectpoly > px.poly
 *
 * It can handle multipolygons but not holes; strange input data will
 * yield unpredictable results (segfaults, most likely). Feel free to 
 * add appropriate error checking.
 *
 * compile with:
 * c++ -o intersectpoly intersectpoly.cc
 *
 * Written by Frederik Ramm <frederik@remote.org>, public domain.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>

#include <boost/geometry/algorithms/append.hpp>
#include <boost/geometry/algorithms/correct.hpp>
#include <boost/geometry/algorithms/intersection.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/multi_polygon.hpp>

namespace bgeom = boost::geometry;
using geometry_numeric_type = double;
using bpoint_t = bgeom::model::d2::point_xy<geometry_numeric_type>;
using bring_t = bgeom::model::ring<bpoint_t>;
using bpolygon_t = bgeom::model::polygon<bpoint_t>;
using bmulti_polygon_t = bgeom::model::multi_polygon<bpolygon_t>;

int poly_count = 1;

bmulti_polygon_t readpoly()
{
    int end = 0;
    char buffer[256];
    bring_t coords;
    bmulti_polygon_t polys;
    bring_t outer;
    std::vector<bring_t> inners;
    bool hole = false;

    while (fgets(buffer, sizeof(buffer), stdin))
    {
        if (isspace(buffer[0]))
        {
            end = 0;
            char *first = strpbrk(buffer, "0123456789.-");
            char *second = strpbrk(first+1, " \t");
            bgeom::append(coords, bpoint_t(atof(first), atof(second)));
        }
        else if (!strncmp(buffer, "END", 3))
        {
            if (end) break;
            end++;
            if (!hole)
            {
                outer = std::move(coords);
            } 
            else
            {

                inners.push_back(std::move(coords));
            }
            coords = bring_t{};
            hole = false;
        }
        else
        {
            end = 0;
            hole = (buffer[0] == '!');
            if (!hole && !outer.empty())
            {
                bpolygon_t p;
                p.outer() = std::move(outer);
                p.inners() = std::move(inners);
                polys.push_back(std::move(p));
                outer = bring_t{};
                inners = std::vector<bring_t>{};
            }
        }
    }
    if (outer.size() > 3)
    {
        bpolygon_t p;
        p.outer() = std::move(outer);
        p.inners() = std::move(inners);
        polys.push_back(std::move(p));
    }
    return polys;
}

void print_points(const bring_t& ring)
{
    for (const auto& p : ring)
    {
        printf("   %10E   %10E\n", bgeom::get<0>(p), bgeom::get<1>(p));
    }
    printf("END\n");
}

void writepoly(const bpolygon_t& p)
{
    printf("%d\n", poly_count++);
    print_points(p.outer());
    for (const auto& inner : p.inners())
    {
        printf("!%d\n", poly_count++);
        print_points(inner);
    }
}

int main(int argc, char **argv)
{
    bmulti_polygon_t p1 = readpoly();
    bgeom::correct(p1);
    bmulti_polygon_t p2 = readpoly();
    bgeom::correct(p2);
    bmulti_polygon_t result;
    bgeom::intersection(p1, p2, result);
    printf("none\n");
    for (const auto& p : result) {
        writepoly(p);
    }
    printf("END\n");
}

