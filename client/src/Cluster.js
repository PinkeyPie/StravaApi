import React, { useState, useEffect } from 'react';
import { Marker, useMap, Map } from 'react-map-gl/maplibre';
import './Cluster.css';

function Cluster(props) {
    
    const {map: map} = useMap();

    const handleClusterClick = (e) => {
        map.flyTo({ center: [props.data.coords[0], props.data.coords[1]], duration: 1500});
    };

    return (
        <Marker longitude={props.data.coords[0]} latitude={props.data.coords[1]} onClick={handleClusterClick}>
            <div className='cluster-circle'>
                <div className='cluster-cnt'>
                    <p>
                        {props.data.pointCount}
                    </p>
                </div>
            </div>
        </Marker>
    )
}

export default Cluster;