# Contributing to Geofabrik clipping polygon set for download.geofabrik.de

We welcome contributions to our clipping polygon set and we endeavour to 
deploy corrections on our download server in a timely fashion.

We cannot, however, honor requests for arbitrary additional extracts.
We will create additional extracts once a country or region file has
reached a certain size limit; other than that, special extracts - whether
they be smaller extracts or combinations of regions - can only be offered
as a commercial service.

## What clipping polygons include and what not

### Simplification and size

Clipping polygons are buffered and simplified boundary polygons of the region they represent.
The buffer width depends on the density of OpenStreetMap data along the border. If there is little data,
a buffer width of 10 to 50 km can be fine. If there is a lot of data (for example when a region boundary cuts through a city), buffer sizes shrink down to about 100 metres.

Because of this complicated rule we don't (yet?) create the boundaries automatically.

Clipping polygons must include the complete are of the country. No point of its boundary line should be outside the clipping polygon.


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

Clipping polygons include the sea up to a sensible distance from the coast.
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
