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
#include <geos/geom/Polygon.h>
#include <geos/geom/MultiPolygon.h>
#include <geos/geom/Coordinate.h>
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/CoordinateSequenceFactory.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/io/WKTWriter.h>

const geos::geom::GeometryFactory* gfactory;;
int poly_count = 1;
geos::io::WKTWriter wrt;

std::unique_ptr<geos::geom::MultiPolygon> readpoly()
{
    int end = 0;
    char buffer[256];
    std::vector<geos::geom::Coordinate> *coords = new std::vector<geos::geom::Coordinate>();
    std::vector<std::unique_ptr<geos::geom::Polygon>> polys;
    std::unique_ptr<geos::geom::LinearRing> outer;
    std::vector<std::unique_ptr<geos::geom::LinearRing>> inner;
    bool hole = false;

    while (fgets(buffer, sizeof(buffer), stdin))
    {
        if (isspace(buffer[0]))
        {
            end = 0;
            char *first = strpbrk(buffer, "0123456789.-");
            char *second = strpbrk(first+1, " \t");
            coords->push_back(geos::geom::Coordinate(atof(first), atof(second)));
        }
        else if (!strncmp(buffer, "END", 3))
        {
            if (end) break;
            end++;
            std::unique_ptr<geos::geom::CoordinateSequence> cs = gfactory->getCoordinateSequenceFactory()->create(coords);
            std::unique_ptr<geos::geom::LinearRing> lr = gfactory->createLinearRing(std::move(cs));
            if (!hole)
            {
                outer = move(lr);
            } 
            else
            {
                inner.push_back(move(lr));
            }
            coords = new std::vector<geos::geom::Coordinate>();
            hole = false;
        }
        else
        {
            end = 0;
            hole = (buffer[0] == '!');
            if (!hole && outer != NULL)
            {
                std::unique_ptr<geos::geom::Polygon> p = gfactory->createPolygon(move(outer), move(inner));
                polys.push_back(move(p));
                outer = NULL;
                inner = std::vector<std::unique_ptr<geos::geom::LinearRing>>();
            }
        }
    }
    delete coords;
    if (outer)
    {
       std::unique_ptr<geos::geom::Polygon> p = gfactory->createPolygon(move(outer), move(inner));
       polys.push_back(move(p));
    }
    return gfactory->createMultiPolygon(move(polys));
}

void writepoly(const geos::geom::Geometry* p)
{
    const geos::geom::Polygon *pp = dynamic_cast<const geos::geom::Polygon *>(p);
    int numir = pp->getNumInteriorRing(); 
    for (int i=-1; i<numir; i++)
    {
        printf("%s%d\n", (i==-1) ? "" : "!", poly_count++);
        const geos::geom::LineString *ls = (i== -1) ? pp->getExteriorRing() : pp->getInteriorRingN(i);
        std::unique_ptr<geos::geom::CoordinateSequence> cs = ls->getCoordinates();
        for (size_t j=0; j<cs->getSize(); j++)
        {
            printf("   %10E   %10E\n", cs->getAt(j).x, cs->getAt(j).y);
        }
        printf("END\n");
    }
}

int main(int argc, char **argv)
{
    gfactory = geos::geom::GeometryFactory::getDefaultInstance();
    std::unique_ptr<geos::geom::MultiPolygon> p1 = readpoly();
    std::unique_ptr<geos::geom::MultiPolygon> p2 = readpoly();
    std::unique_ptr<geos::geom::Geometry> intersect = p1->intersection(p2.get());
    printf("none\n");
    if (intersect->getGeometryTypeId() == geos::geom::GEOS_POLYGON)
    {
        writepoly(intersect.get());
    }
    else if (intersect->getGeometryTypeId() == geos::geom::GEOS_MULTIPOLYGON)
    {
        for (size_t i=0; i< intersect->getNumGeometries(); i++)
        {
            writepoly(intersect->getGeometryN(i));
        }
    }
    printf("END\n");
}

