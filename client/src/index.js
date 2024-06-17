// import React from 'react';
// import ReactDOM from 'react-dom/client';
// import './index.css';
// import App from './App';
// import reportWebVitals from './reportWebVitals';

// const root = ReactDOM.createRoot(document.getElementById('root'));
// root.render(
//   <React.StrictMode>
//     <App />
//   </React.StrictMode>
// );

// // If you want to start measuring performance in your app, pass a function
// // to log results (for example: reportWebVitals(console.log))
// // or send to an analytics endpoint. Learn more: https://bit.ly/CRA-vitals
// reportWebVitals();

import React from 'react';
import ReactDOM from 'react-dom/client';
import reportWebVitals from './reportWebVitals';
import { BrowserRouter as Router, Route, Routes } from "react-router-dom";
import App from './App';
import Map from './Map';
import InterestPoints from './InterestPoints';
import HeatMap from './HeatMap';
import Paths from './Paths';

const root = ReactDOM.createRoot(document.getElementById('root'));
root.render(
  <React.StrictMode>
    <Router>
      <Routes>
        <Route exact path='/' element={<App></App>}></Route>
        <Route exact path='/interest_points' element= { <InterestPoints /> }>
        </Route>
        <Route exact path='/heat_map' element={ <HeatMap /> }></Route>
        <Route exact path='/paths' element={ <Paths/> }></Route>
      </Routes>
    </Router>
  </React.StrictMode>
);
