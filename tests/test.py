from pymbgl import Map

# m = Map(100, 200, 1, token='foo', style_url="file://tests/fixtures/empty_style.json")

# style = open('tests/fixtures/empty_style.json').read()
style = open('tests/fixtures/example-style-geojson.json').read()
m = Map(200, 200, 1, token=None, style_json=style)
data = m.render(zoom=3, longitude=-120, latitude=40)
with open('/tmp/test.png', 'wb') as out:
    out.write(data)


# segfaults because network requests not handled
# style = open('tests/fixtures/example-style-mapbox-source.json').read()
# m = Map(200, 200, 1, token=None, style_json=style)
# data = m.render(zoom=3, longitude=-120, latitude=40)