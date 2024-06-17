import React from 'react';
import './HomeButton.css';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome'
import { faHome } from "@fortawesome/free-solid-svg-icons";

function HomeButton() {
    return (
        <a href='/' className='home-button'>
            <FontAwesomeIcon className='home-button-icon' icon={ faHome } />
        </a>
    );
}

export default HomeButton;