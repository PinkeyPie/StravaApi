import { imageOverlay } from 'leaflet';
import React, { useEffect, useState } from 'react';
import './App.css';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome'
import { faMapLocation } from "@fortawesome/free-solid-svg-icons";
import { Rings } from 'react-loader-spinner';

function App() {
    const [loading, setLoading] = useState(false);

    useEffect(() => {
        setLoading(true);
    }, []);

    return (
        <>
            {!loading ? (
                <Rings
                    visible={true}
                    height="80"
                    width="80"
                    color="#4fa94d"
                    ariaLabel="rings-loading"
                    wrapperStyle={{
                        width: "100vw",
                        height: "100vh",
                        textAlign: "center",
                        verticalAlign: "middle",
                    }}
                    wrapperClass=""
                />

            ) : (
                <>
                    <div className='img-container'>
                        <div className='header-container'>
                            <div className='map-header-container'>
                                <h1 className='map-header'>Карты</h1>
                            </div>
                            <div className='map-logo-container'>
                                <FontAwesomeIcon className='map-icon' icon={faMapLocation} />
                            </div>
                        </div>
                    </div>
                    <div className='link-list-container'>
                        <div className='link-list-header'>
                            <h3>Режимы</h3>
                        </div>
                        <div className='link-list-element'>
                            <a href='/interest_points'>Базовые станции</a>
                        </div>
                        <div className='link-list-element'>
                            <img src='/heatmap.jpg' className='list-element-img'></img>
                            <a href='/heat_map'>Тепловые карты</a>
                        </div>
                        <div className='link-list-element'>
                            <img src='/sampled_routes.png' className='list-element-img'></img>
                            <a href='/client/src/Paths'>Маршруты постов</a>
                        </div>
                    </div>
                </>
            )
            }
        </>
    );
}

export default App;