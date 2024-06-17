import React, { useEffect, useState } from 'react';
import './Paths.css';
import Map from './Map';
import { slide as Menu } from 'react-burger-menu';
import MultilevelSidebar from 'react-multilevel-sidebar';
import { TripsLayer, HeatmapLayer } from 'deck.gl';
import axios from 'axios';

import 'react-multilevel-sidebar/src/Sidebar.css';

function Paths(props) {

    const [pathsData, setPathsData] = useState([]);
    const [pathsLayers, setPathsLayers] = useState([]);

    useEffect(() => {
        const loadPaths = async () => {
            const response = await axios.get('http://localhost:5000/paths/get_path_gps_data');
            setPathsData([response.data]);
        };

        loadPaths();
    }, []);

    let [isOpen, setIsOpen] = useState(false);

    const handleSidebarToggle = isActive => {
        isOpen = setIsOpen(!isOpen);
    }

    const handleClick = async (props) => {
        if (props.isTime !== undefined) {
            setPAthLayers([
                new TripsLayer({
                    id: 'TripsLayer',
                    data: `http://localhost:5000/paths/get_path_path?pathId=${props.pathId}&date=${props.date}&rowNumber=${props.rowNumber}`,
                    getPath: (d) => d.waypoints.map(p => p.coordinates),
                    // Timestamp is stored as float32, do not return a long int as it will cause precision loss
                    getTimestamps: (d) => d.waypoints.map(p => p.timestamp - 1554772579000),
                    getColor: [0, 0, 0],
                    currentTime: 500,
                    trailLength: 600,
                    capRounded: true,
                    jointRounded: true,
                    widthMinPixels: 15,
                }),
                new HeatmapLayer({
                    id: 'HeatmapLayer',
                    data: 'https://raw.githubusercontent.com/visgl/deck.gl-data/master/website/sf-bike-parking.json',
                    aggregation: 'SUM',
                    getPosition: (d) => d.COORDINATES,
                    getWeight: (d) => d.SPACES,
                    radiusPixels: 25
                }),
            ]);
        }
    }

    return (
        <>
            <Menu
                isOpen={false}
                width={700}
                className='path-burger'
            >
                <MultilevelSidebar
                    open={true}
                    onToggle={handleSidebarToggle}
                    options={pathsData}
                    header="Маршруты постов"
                    onItemClick={handleClick}
                />
            </Menu >
            <Map
                className="paths-map"
                layers={pathLayers}
            />
        </>
    );
}

export default Paths;