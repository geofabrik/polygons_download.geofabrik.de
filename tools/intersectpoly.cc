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
 * c++ -o intersectpoly intersectpoly.cc -lgeos
 *
 * Written by Frederik Ramm <frederik@remote.org>, public domain.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>

#include <geos/geom/LinearRing.h>
#include <geos/geom/LinearRing.h>
#include <geos/geom/Polygon.h>
#include <geos/geom/MultiPolygon.h>
#include <geos/geom/Coordinate.h>
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/CoordinateSequenceFactory.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/io/WKTWriter.h>

using namespace geos::geom;
using namespace std;

GeometryFactory::unique_ptr gfactory;
int count = 1;
geos::io::WKTWriter wrt;

MultiPolygon* readpoly()
{
    int end = 0;
    char buffer[256];
    vector<Coordinate> *coords = new vector<Coordinate>();
    vector<Geometry *> polys;
    LinearRing *outer = NULL;
    vector<Geometry*> *inner = new vector<Geometry *>;
    bool hole = false;

    while (fgets(buffer, sizeof(buffer), stdin))
    {
        if (isspace(buffer[0]))
        {
            end = 0;
            char *first = strpbrk(buffer, "0123456789.-");
            char *second = strpbrk(first+1, " \t");
            coords->push_back(Coordinate(atof(first), atof(second)));
        }
        else if (!strncmp(buffer, "END", 3))
        {
            if (end) break;
            end++;
            CoordinateSequence *cs = gfactory->getCoordinateSequenceFactory()->create(coords);
            LinearRing *lr = gfactory->createLinearRing(cs);
            if (!hole)
            {
                outer = lr;
            } 
            else
            {
                inner->push_back(lr);
            }
            coords = new vector<Coordinate>();
            hole = false;
        }
        else
        {
            end = 0;
            hole = (buffer[0] == '!');
            if (!hole && outer != NULL)
            {
                Polygon *p = gfactory->createPolygon(outer, inner);
                polys.push_back(p);
                outer = NULL;
                inner = new vector<Geometry *>;
            }
        }
    }
    delete coords;
    if (outer)
    {
       Polygon *p = gfactory->createPolygon(outer, inner);
       polys.push_back(p);
    }
    return gfactory->createMultiPolygon(polys);
}

void writepoly(const Geometry *p)
{
    const Polygon *pp = dynamic_cast<const Polygon *>(p);
    int numir = pp->getNumInteriorRing(); 
    for (int i=-1; i<numir; i++)
    {
        printf("%s%d\n", (i==-1) ? "" : "!", count++);
        const LineString *ls = (i== -1) ? pp->getExteriorRing() : pp->getInteriorRingN(i);
        CoordinateSequence *cs = ls->getCoordinates();
        for (int j=0; j<cs->getSize(); j++)
        {
            printf("   %10E   %10E\n", cs->getAt(j).x, cs->getAt(j).y);
        }
        printf("END\n");
    }
}

int main(int argc, char **argv)
{
    gfactory = GeometryFactory::create();
    MultiPolygon *p1 = readpoly();
    MultiPolygon *p2 = readpoly();
    Geometry *intersect = p1->intersection(p2);
    printf("none\n");
    if (intersect->getGeometryTypeId() == GEOS_POLYGON)
    {
        writepoly(intersect);
    }
    else if (intersect->getGeometryTypeId() == GEOS_MULTIPOLYGON)
    {
        for (int i=0; i< intersect->getNumGeometries(); i++)
        {
            writepoly(intersect->getGeometryN(i));
        }
    }
    printf("END\n");
}

