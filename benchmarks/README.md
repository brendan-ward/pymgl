# Benchmarks

To run all benchmarks:

```bash
pytest benchmarks
```

If you want to run benchmarks against Mapbox sources, uncomment those benchmarks.

The benchmarks optionally compare against the performance of
[mbgl-renderer](https://github.com/consbio/mbgl-renderer). To run these tests,
`mbgl-render` must be running on port 8002 and have mounted the `tests/fixtures`
directories for local tiles.
