import React, { useEffect, useState } from 'react';
import { Marker, Popup, Layer, Source } from 'react-map-gl/maplibre';
import * as turf from '@turf/turf';
import { Rings } from 'react-loader-spinner';
import './InterestPointsMarker.css';
import Cluster from './Cluster';

// Fill style spec https://docs.mapbox.com/style-spec/reference/layers/
// layers.point.type: expected one of [fill, line, symbol, circle, heatmap, fill-extrusion, raster, hillshade, background]

function InterestPointsMarker(props) {

    const [show, setShow] = useState(false);
    const [isCluster, setIsCluster] = useState(false);
    // const { map: map } = useMap();
    // console.log(map)

    useEffect(() => {
        if (props.data.type === 'cluster') {
            setIsCluster(true);
        } else {
            setIsCluster(false);
        }
    }, []);

    // const [baseStationsData, setInterestPointsData] = useState({ ...props.baseStationsData });

    // Square example
    // let line = turf.lineString([[-74, 40], [-78, 42], [-82, 35]]);
    // let bbox = turf.bbox(line);
    // let bboxPolygon = turf.bboxPolygon(bbox);
    // console.log(bboxPolygon.geometry.coordinates);

    const handleMarkerClick = (e) => {
        e.originalEvent.stopPropagation();
        // map.flyTo({ center: [props.data.coords[0], props.data.coords[1]], duration: 1500 });
        setShow(!show);
    };

    const handlePopupCloseClick = () => {
        setShow(false);
    };

    return (
        <>
            {isCluster ? (
                <Cluster data={props.data}></Cluster>
            ) : (
                <>
                    <Marker longitude={props.data.coords[0]} latitude={props.data.coords[1]} anchor={"bottom"}
                        onClick={handleMarkerClick}
                    >
                        <img className="marker-icon" src="/pin.png" width={50} height={50} />
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
                       
                    </Marker>
                    {show && (
                        <Popup
                            longitude={props.data.coords[0]}
                            latitude={props.data.coords[1]}
                            onClose={handlePopupCloseClick}
                            width={150}
                        >
                        </Popup>
                    )}
                </>
            )
            }
        </>
    );
}

export default InterestPointsMarker;