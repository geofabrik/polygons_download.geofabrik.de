# Contributing to Geofabrik clipping polygon set for download.geofabrik.de

We welcome contributions to our clipping polygon set.

## What clipping polygons include and what not

### Simplification and size

Clipping polygons are buffered and simplified boundary polygons of the region they represent.
The buffer width depends on the density of OpenStreetMap data along the border. If there is few data,
a buffer width of 10 to 50 km can be fine. If there is much data, buffer sizes shrink to 100 m to 5 km. In towns, we avoid buffers wider than a few hundred meters.

Clipping polygons must include the complete are of the country. No point of its boundary line should be located beyond it.


### Shared boundaries

If a clipping polygon shares the boundary with its parent polygon (e.g. Maine and United States),
the larger polygon must follow the boundary line (between US and Canada) and the smaller polygon
should extend into the neighbouring country with a huge buffer along the boundary. During
deployment, both polygons will be intersected.


### Enclaves and exclaves

Enclaves should be included if the enclave is small. They should be excluded if the enclave is a complete country. Exclaves should be included if they belong to the same continent as the main country.

If exclaves are located closely to the border (e.g. Baarle-Nassau/Baarle-Hertog), the clipping polygon
should not contain a separate polygon for the exclave but the polygon should extend beyond the border to include the exclave.

### Sea

Clipping polygons include the sea up to a sensible distance from coast.
If an extract includes islands on sea, the distance will be larger.
If an extract is small, the buffer along coast will be smaller.


### Disputed territories

The inclusion of a disputed territory in our clipping polygons does
not constitute a political statement. Please be aware on the [opinion of the
OpenStreetMap Foundation on disputed territories](https://wiki.osmfoundation.org/w/images/d/d8/DisputedTerritoriesInformation.pdf) and how they are handled
within the OpenStreetMap database.

In general, clipping polygons follow the mapped boundary polygons (`type=boundary` +
`boundary=administrative`). If a land area belongs to multiple boundary polygons at the same level,
it should be included in both. If a land area belongs to no boundary polygon, it should be
covered by at least one polygon.


## Editing poly files

Poly files can be edited using the JOSM "poly" plugin. You can install it from the JOSM settings
at *Edit* → *Preferences* → *Plugins*.

See the [OSM wiki](https://wiki.openstreetmap.org/wiki/Osmosis/Polygon_Filter_File_Format#Converting_to/from_POLY_format) for further options to convert poly files into other file formats and vice versa.
