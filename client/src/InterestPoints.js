import React, { useState } from 'react';
import './InterestPoints.css';
import { Marker } from 'react-map-gl/maplibre'
import InterestPointsMarker from './InterestPointsMarker';

function InterestPoints(props) {

    const mockInterestPointsFetch = [
        {
            type: 'cluster',
            pointCount: 55,
            coords: [37.618423, 55.751244],
        },
        {
            type: 'point',
            coords: [43.936058, 56.296505],
            info: {
                operator: 'megafone',
                mcc: 100,
                mnc: 13,
                radius: 0.5,
            }
        },
        {
            type: 'cluster',
            pointCount: 4,
            coords: [39.700001, 47.233334],
        },
        {
            type: 'point',
            coords: [37.618492, 54.204838],
            info: {
                operator: 'beeline',
                mcc: 100,
                mnc: 13,
                radius: 0.5,
            },
        }
    ];
    return (
        <>
            {/*{*/}
            {/*        return (*/}
            {/*            <InterestPointsMarker key={index} data={baseStationMarkerData} />*/}
            {/*        );*/}
            {/*    })*/}
            {/*}*/}
        </>

    );
}

export default InterestPoints;