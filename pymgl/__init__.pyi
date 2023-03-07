import numpy as np
import numpy.typing as npt

class Map:
    def __init__(
        self,
        style: str,
        width: int = 256,
        height: int = 256,
        ratio: float = 1,
        longitude: float = 0,
        latitude: float = 0,
        zoom: float = 0,
        token: str = None,
        provider: str = None,
    ) -> Map:
        """Create Maplibre GL map instance.

        Parameters
        ----------
        style : str
            Mapbox GL style object as json-encoded string.
        width : int, optional (default: 256)
            Width of output map.
        height : int, optional (default: 256)
            Height of output map.
        ratio : float, optional (default: 1)
            Pixel ratio of output map.
        longitude : float, optional (default: 0)
            Map center longitude.
        latitude : float, optional (default: 0)
            Map center latitude.
        zoom : float, optional (default 0)
            Map zoom level, between 0 and 24.
        token : str, optional
            Token, if required for provider.
        provider : str, one of {'mapbox', 'maptiler', 'maplibre', None}
            Map resource provider, if required for sources listed in the style.
        """
    def __enter__(self): ...
    def __exit__(self, exc_type: object, exc_value: str, traceback: str): ...
    def addImage(
        self,
        name: str,
        image: bytes,
        width: int,
        height: int,
        ratio: float = 1,
        make_sdf: bool = False,
    ):
        """Add an image to the map to be used as an icon or pattern.

        Parameters
        ----------
        name : str
            Name or ID of the image.  Used when referencing
            this image in the style.
        image : bytes
            Raw pixels of image data.  PNG and other images must
            be decoded to raw pixel values.
        width : int
            Width of image.
        height : int
            Height of image.
        ratio : float
            Pixel ratio of image to use when rendering the image.
        make_sdf : bool
            If True, image will be converted to an SDF format.  See
            the Mapbox Style Specification for more information about
            SDF images.
        """
    @property
    def bearing(self) -> float:
        """map bearing, in degrees"""
    @property
    def center(self) -> tuple[float, float]:
        """map center (longitude, latitude)"""
    @property
    def pitch(self) -> float:
        """map pitch, in degrees"""
    @property
    def size(self) -> tuple[float, float]:
        """map size (width, height)"""
    @property
    def zoom(self) -> float:
        """map zoom"""
    def getLayerFilter(self, id: str) -> str:
        """Get the filter of a layer in the map

        Parameters
        ----------
        id : str
            id of layer in map

        Returns
        -------
        str or None
        """
    def getLayerJSON(self, id: str) -> str:
        """Get JSON that describes a layer

        Parameters
        ----------
        id : str
            id of layer in map

        Returns
        -------
        str
        """
    def getLayerVisibility(self, id: str) -> bool:
        """Get the visibility of a layer in the map

        Parameters
        ----------
        id : str
            id of layer in map

        Returns
        -------
        bool
        """
    def listLayers(self) -> list:
        """List layer ids in the map's style

        Returns
        -------
        list
        """
    def listSources(self) -> list:
        """List source ids in the map's style

        Returns
        -------
        list
        """
    def renderPNG(self) -> bytes:
        """Render the map to PNG bytes."""
    def renderBuffer(self) -> np.ndarray[np.uint8]:
        """Render the map to a numpy array of uint8 pixel values."""
    def setBearing(self, bearing: float) -> None:
        """Set the bearing of the map.

        Parameters
        ----------
        bearing : float
            Map bearing in degrees, between 0 and 360.
        """
    def setBounds(
        self, xmin: float, ymin: float, xmax: float, ymax: float, padding: int = 0
    ) -> None:
        """Fit the map to the bounds, given an optional inset padding in pixels.

        Parameters
        ----------
        xmin : float
        ymin : float
        xmax : float
        ymax : float
        padding : int, optional (default: 0)
        """
    def setLayerFilter(self, id: str, filter: str = None) -> None:
        """Set the filter of a layer in the map

        Parameters
        ----------
        id : str
            id of layer in map
        visible : str, optional (default None)
            JSON string or None / empty string to clear filter
        """
    def setLayerVisibility(self, id: str, visible: bool) -> None:
        """Set the visibility of a layer in the map

        Parameters
        ----------
        id : str
            id of layer in map
        visible : bool
            set to True to make layer visible
        """
    def setPitch(self, pitch: float) -> None:
        """Set the pitch of the map.

        Parameters
        ----------
        pitch : float
            Map pitch in degrees, between 0 and 85.
        """
    def setZoom(self, zoom: float) -> None:
        """Set the zoom level of the map.

        Parameters
        ----------
        zoom : float
            Map zoom between 0 and 24
        """
    def setSize(self, width: int, height: int) -> None:
        """Set the width and height of the map.

        Parameters
        ----------
        width : int
        height : int
        """
