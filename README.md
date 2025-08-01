# Clipping polygons for download.geofabrik.de

This repository contains clipping polygon for download.geofabrik.de.
They are used to create the regional extracts of the planet dump using `osmium extract`.
The format is documented [on the OSM wiki](https://wiki.openstreetmap.org/wiki/Osmosis/Polygon_Filter_File_Format).

This repository is published in order to allow you to track the changes
to the clipping boundaries, and submit issues if you should find a bug.

## How it works

### Directory structure

In the `polygons` directory, you will find the polygons in (mostly) hierarchical order.
At the top level, there are continents (and Russia as intercontinental country, the US and Canada for historic reasons).
Below them you will find countries within continents, and states/provinces within their countries.
In addition, the top level directory contains some special clipping polygons which do not match a country or published extract
to split the world into few large subregions.

### How are clipping polygons used?

When the Geofabrik download server computes nightly OSM extracts, it does
so in a hierarchical fashion. First the contients are created from the planet
file, then the countries are cut from the continents, and so on.

Therefore, the clipping process does not have to know about boundaries where
they coincide with a higher level boundary. For example, because the
Southern border of Mexico coincides with the border of what we call 
"North America", and because Mexico is cut from the "North America" file,
this particular boundary only requires precise modelling in the 
"North America" polygon. In the "Mexico" polygon you will find that this
border coarsely extends into the US landmass, but this does not have an 
effect on the acutal Mexico extract which is, along this border, linited
by the "North America" polygon.

There is a tool in this repository called [intersectpoly](tools/intersectpoly.cc) and we use this to create the smallest common area of several polygons, 
so that the polygons (or GeoJSON index files) we offer on the download
server show the actual polygon contained in the extract. They will differ
from the polygons in this repository.

### What is included in the polygons, what is missing?

See [CONTRIBUTING.md](CONTRIBUTING.md) for buffer sizes, degree of simplification, handling of exclaves etc.

## License

Polygons are a produced work as in terms of Open Database License 1.0. By using them, you are required to attribute OpenStreetMap contributors.

Software is released into the public domain.
