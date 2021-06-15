################################################################################################################################################################

# @project        Open Space Toolkit ▸ Mathematics
# @file           bindings/python/test/geometry/d3/objects/test_composite.py
# @author         Remy Derollez <remy@loftorbital.com>
# @license        Apache License 2.0

################################################################################################################################################################

import pytest

import numpy as np

import ostk.mathematics as mathematics

################################################################################################################################################################

Object = mathematics.geometry.d3.Object
Point = mathematics.geometry.d3.objects.Point
LineString = mathematics.geometry.d3.objects.LineString
Composite = mathematics.geometry.d3.objects.Composite
Transformation = mathematics.geometry.d3.Transformation

################################################################################################################################################################

def test_geometry_d3_objects_composite_constructor ():

    point_1: Point = Point(-1.0, 1.0, 1.0)
    point_2: Point = Point(1.0, 1.0, -1.0)
    point_3: Point = Point(1.0, -1.0, 1.0)
    point_4: Point = Point(-1.0, -1.0, 0.0)

    # Construction with Array of Points using python list
    linestring: LineString = LineString([point_1, point_2, point_3, point_4])

    assert linestring is not None
    assert isinstance(linestring, LineString)
    assert isinstance(linestring, Object)
    assert linestring.is_defined()

    # Construct Composite using the Line String
    composite: Composite = Composite(linestring)

    assert composite is not None
    assert isinstance(composite, Composite)
    assert isinstance(composite, Object)
    assert composite.is_defined()
    assert composite.is_empty() is False
    assert composite.is_point() is False
    assert composite.is_line() is False
    assert composite.is_line_string()

    # Access Object in the Composite
    linestring_accessed: LineString = composite.access_object_at(0)

    assert linestring_accessed is not None
    assert linestring_accessed.is_defined()
    assert isinstance(linestring_accessed, LineString)
    assert isinstance(linestring_accessed, Object)
    assert isinstance(linestring_accessed, Composite) is False

    # Get Object Count
    object_count: int = composite.get_object_count()

    assert object_count is not None
    assert object_count == 1

################################################################################################################################################################
