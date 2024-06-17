import datetime
import uuid
from typing import Optional, List, Dict

from sqlalchemy import cast, DATE, and_
from flask import url_for, request, flash, session, jsonify
from app import db
from app.paths import bp
from .models import Path, GpsData
import os
import json
from geoalchemy2 import Geometry
import time
import geoalchemy2.functions as functions
from flask_cors import CORS, cross_origin
from shared_misc.json_node_detour import JsonNodeDetour

@bp.route('/all_paths')
@cross_origin()
def all_paths():
    paths = Path.query.all()
    ret_data = []
    for path in paths:
        coordinates = (db.session.query(functions.ST_X(GpsData.coordinates), functions.ST_Y(GpsData.coordinates), GpsData.time)
                 .filter_by(path_id=path.id).order_by(GpsData.time.desc()).first())
        if coordinates is None:
            continue
        coordinates = [coordinates[0], coordinates[1]]
        ret_data.append({
            'path_name': path.name,
            'path_id': str(path.id),
            'coord': coordinates,
        })
    return jsonify(ret_data)


def get_path_days(path_id: uuid.UUID) -> Optional["List[dict]"]:
    path = Path.query.get(path_id)
    if path is None:
        return None
    timestamps = (db.session.query(cast(GpsData.time, DATE)).filter_by(path_id=path_id).group_by(cast(GpsData.time, DATE))
                  .order_by(cast(GpsData.time, DATE)).first())

    result = []
    for timestamp in timestamps:
        path_date: datetime.date = timestamp[0]
        result.append({
            'name': path_date.strftime('%d %m'),
            'date': path_date.strftime('%Y-%m-%d')
        })
    return result


def get_path_time(path_id: uuid.UUID) -> Optional[dict]:
    path = Path.query.get(path_id)
    if path is None:
        return None
    timestamps_query = db.session.query(GpsData.time).filter_by(path_id=path_id).order_by(GpsData.time.asc()).all()
    timestamps: List[datetime.datetime] = [dt[0] for dt in timestamps_query]
    dates: List[datetime.date] = [dt.date() for dt in timestamps]
    dates = list(set(dates))
    result = {}
    for date in dates:
        result[date] = []
        date_times = list(filter(lambda dt: dt.date() == date, timestamps))
        if len(date_times) > 0:
            cur_time: datetime.time = date_times[0]
            first_time = cur_time
            iterator = 1
            for path_time in date_times:
                delta: datetime.timedelta = path_time - cur_time
                if delta.total_seconds() > 120:
                    result[date].append({
                        'time': first_time.strftime('%H:%M'),
                        'rowNumber': iterator
                    })
                    first_time = path_time
                    iterator = iterator + 1
                cur_time = path_time
            result[date].append({
                'time': first_time.strftime('%H:%M'),
                'rowNumber': iterator
            })
    return result


@bp.route('/path_path_days')
@cross_origin()
def path_path_days():
    name = request.args.get('name')
    path_id = request.args.get('path_id')
    result = {
        'status': 'success',
        'message': ''
    }
    if name is None and path_id is None:
        result['status'] = 'fail'
        result['message'] = 'name or path id is required'
        return json.dumps(result)
    if path_id is None:
        path = Path.query.filter_by(name=name).first()
    else:
        path = Path.query.get(path_id)
    if path is None:
        result['status'] = 'fail'
        result['message'] = 'path does not exist'
        return json.dumps(result)
    timestamps = (db.session.query(cast(GpsData.time, DATE)).filter(GpsData.path_id == path.id)
                  .group_by(cast(GpsData.time, DATE)).all())
    result['data'] = []
    for timestamp in timestamps:
        path_date: datetime.date = timestamp[0]
        if path_date.day == datetime.date.today().day:
            result['data'].append("today")
        else:
            result['data'].append(path_date.strftime("%Y-%m-%d"))
    return jsonify(result)


@bp.route('/get_paths_at_date')
@cross_origin()
def get_paths_at_date():
    name = request.args.get('name')
    path_id = request.args.get('path_id')
    date = request.args.get('date')
    result = {
        'status': 'success',
        'message': ''
    }
    if name is None and path_id is None:
        result['status'] = 'fail'
        result['message'] = 'name or path id is required'
        return json.dumps(result)
    if date is None:
        result['status'] = 'fail'
        result['message'] = 'date is required'
        return json.dumps(result)
    if path_id is None:
        path = Path.query.filter_by(name=name).first()
    else:
        path = Path.query.get(path_id)
    if path is None:
        result['status'] = 'fail'
        result['message'] = 'path does not exist'
        return json.dumps(result)
    coordinates = db.session.query(GpsData.time).filter_by(path_id=path.id).order_by(GpsData.time.asc()).all()
    if coordinates is None:
        result['status'] = 'fail'
        result['message'] = 'something is wrong'
        return json.dumps(result)
    prev_coord: datetime.datetime = coordinates[0][0]
    last_record_time = prev_coord.strftime('%H:%m:%S')
    result['data'] = []
    iterator = 1
    for i in range(1, len(coordinates)):
        curr_coord = coordinates[i][0]
        time_diff: datetime.timedelta = curr_coord - prev_coord
        if time_diff.seconds <= 120:
            last_record_time = curr_coord.strftime('%H:%m:%S')
        else:
            result['data'].append({
                'measurement': iterator,
                'time': last_record_time
            })
            iterator = iterator + 1
        prev_coord = curr_coord
    result['data'].append({
        'measurement': iterator,
        'time': last_record_time
    })
    return jsonify(result)


@bp.route('/get_path_path')
@cross_origin()
def get_path_path():
    path_id = request.args.get('pathId')
    date = request.args.get('date')
    measurement = request.args.get('rowNumber')
    result = {
        'status': 'success',
        'message': ''
    }
    if path_id is None or date is None or measurement is None:
        result['status'] = 'fail'
        result['message'] = 'you forgot to pass path id or date or measurement or date'
        return json.dumps(result)
    path = Path.query.get(path_id)
    if path is None:
        result['status'] = 'fail'
        result['message'] = 'path does not exist'
        return json.dumps(result)
    date = datetime.datetime.strptime(date, '%d-%m-%Y')
    measurement = int(measurement)
    coordinates = (db.session.query(GpsData.time, functions.ST_X(GpsData.coordinates), functions.ST_Y(GpsData.coordinates))
                   .filter(and_(GpsData.path_id == path.id, cast(GpsData.time, DATE) == date)).
                   order_by(GpsData.time.asc()).all())
    if coordinates is None:
        result['status'] = 'fail'
        result['message'] = 'something went wrong'
        return json.dumps(result)
    prev_coord = coordinates[0]
    iterator = 1
    result = {
        'waypoints': []
    }
    timestamp_start = 1554772579000
    if iterator == measurement:
        result['waypoints'].append(
            {
                "coordinates": [prev_coord[1], prev_coord[2]],
                "timestamp": timestamp_start
            }
        )
    for i in range(1, len(coordinates)):
        curr_coord = coordinates[i]
        coord_datetime: datetime.datetime = curr_coord[0]
        time_diff: datetime.timedelta = curr_coord[0] - prev_coord[0]
        if time_diff.seconds > 120:
            iterator = iterator + 1
            if iterator > measurement:
                break
        if iterator == measurement:
            result['waypoints'].append(
                {
                    "coordinates": [curr_coord[1], curr_coord[2]],
                    "timestamp": timestamp_start + i * 10
                }
            )
        prev_coord = curr_coord
    return jsonify([result])


@bp.route('/get_path_gps_data')
@cross_origin()
def get_path_gps_data():
    paths = Path.query.join(GpsData, (GpsData.path_id == Path.id)).all()
    ret_data = []
    path_data = {
        'title': 'Paths',
        'content': []
    }
    for path in paths:
        child_node = {
            'title': 'Scan Dates',
            'content': []
        }
        path_data['content'].append(
            {
                'name': path.name,
                'pathId': str(path.id),
                'children': [child_node]
            }
        )
        path_dates: Dict[datetime.date, List[dict]] = get_path_time(path.id)
        for path_date, path_times in path_dates.items():
            date_child_node = {
                'title': 'Scans time',
                'content': []
            }
            date_node = {
                'name': path_date.strftime('%d %B'),
                'children': [date_child_node]
            }
            for path_time in path_times:
                time_node = {
                    'name': path_time['time'],
                    'rowNumber': path_time['rowNumber'],
                    'date': path_date.strftime('%d-%m-%Y'),
                    'pathId': str(path.id),
                    'isTime': True
                }
                date_child_node['content'].append(time_node)
            child_node['content'].append(date_node)
    detour = JsonNodeDetour(path_data, "name")
    detour.json_detour()
    ret_data = detour.graph
    return jsonify(ret_data)
