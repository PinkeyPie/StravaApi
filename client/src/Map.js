import React, { useEffect, useRef, useState } from 'react';
import './Map.css';
import HomeButton from './HomeButton';
import { MapboxOverlay } from '@deck.gl/mapbox';
import { Map, useControl, NavigationControl, Source, Layer, MapProvider } from 'react-map-gl/maplibre';
import { Rings } from 'react-loader-spinner';
import InterestPoints from './InterestPoints';
import { clusterLayer, clusterCountLayer, unclusteredPointLayer } from './Layers';
import Cluster from './Cluster.js';

function DeckGLOverlay(props) {
  const overlay = useControl(() => new MapboxOverlay(props));
  overlay.setProps(props);
  return null;
};

function Map(props) {
  const [loading, setLoading] = useState(false);

  useEffect(() => {
    setLoading(true);
  }, []);

  // Moscow
  const [lng] = useState(37.5976833);
  const [lat] = useState(55.75977903498331);
  const [zoom] = useState(10);

  // Deck gl initial state
  const INITIAL_VIEW_STATE = {
    longitude: lng,
    latitude: lat,
    zoom: zoom,
  };

  return (
    <>
      {!loading ? (
        <Rings
          className="map-loader"
          visible={true}
          height="80"
          width="80"
          color="#4fa94d"
          ariaLabel="rings-loading"
          wrapperStyle={{}}
          wrapperClass=""
        />
      ) : (
        <div className="map-wrap">
          <MapProvider>
            <Map
              id="map"
              initialViewState={INITIAL_VIEW_STATE}
              mapStyle="http://localhost:8080/styles/maptiler-3d-gl-style/style.json"
            > 
            {/* <Source
              id="earthquakes"
              type="geojson"
              data="https://docs.mapbox.com/mapbox-gl-js/assets/earthquakes.geojson"
              cluster={true}
              clusterMaxZoom={14}
              clusterRadius={50}
            >
              <Layer {...clusterLayer} />
              <Layer {...clusterCountLayer} />
              <Layer {...unclusteredPointLayer} />
            </Source> */}
              <DeckGLOverlay
                layers={props.layers}
              // interleaved
              />
              {/*<InterestPoints />*/}
              <NavigationControl position="bottom-right" />
            </Map>
          </MapProvider>
        </div >
      )
      }

    </>

  );
}

export default Map;