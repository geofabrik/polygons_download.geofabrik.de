# Clipping polygons for download.geofabrik.de

This repository contains clipping polygon for download.geofabrik.de.
They are used to create the regional extracts of the planet dump using `osmium extract`.
The format is documented [on the OSM wiki](https://wiki.openstreetmap.org/wiki/Osmosis/Polygon_Filter_File_Format).

This repository is published in order to allow you to track the changes
to the clipping boundaries.

## How it works

### Directory structure

In the `polygons` directory, you will find the polygon hierarchicall ordered.
At the top level, there are continents (and Russia as intercontinental country, the US and Canada for historic reasons).
Below them you will find countries within continents, and states/provinces within their countries.
In addition, the top level directory contains some special clipping polygons which do not match a country or published extract
to split the world into few large subregions.

### How are clipping polygons derived?

Before clipping polygons are deployed on our production servers, they are
intersected with their parents. By doing so, we avoid to store the same
detailed boundary line of a country in its own polygon file and the polygon
files of its provinces.

Intersection is done using the [intersectpoly](tools/intersectpoly.cc) tool.

### What is included in the polygons, what is missing?

See [CONTRIBUTING.md](CONTRIBUTING.md) for buffer sizes, degree of simplification, handling of exclaves etc.

## License

Polygons are a produced work as in terms of Open Database License 1.0. By using them, you are required to attribute OpenStreetMap contributors.

Software is released into the public domain.
