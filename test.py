#!/Users/bcward/.local/share/virtualenvs/pymbgl-QWxOyrD1/bin/python

from pymbgl.lib import PyLatLng, PyCameraOptions

l = PyLatLng(10, 100)

print(l.latitude(), l.longitude())

c = PyCameraOptions([10, 100], 10, bearing=500)

print("created camera", c.zoom, c.center, c.bearing, c.pitch)
