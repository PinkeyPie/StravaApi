import datetime
import json
from struct import Struct

from flask import request
from sqlalchemy import cast

from app import db
from app.base_stations import bp
from .models import InterestPoints, Region
import geoalchemy2.functions as geo_functions
from flask_cors import CORS, cross_origin
import ctypes
import pathlib


def get_float_array(str_array: str):
    str_array = str_array.replace('[', '')
    str_array = str_array.replace(']', '')
    str_array = str_array.replace(' ', '')
    string_array = str_array.split(',')
    ret_array = [float(ch) for ch in string_array]
    return ret_array


def build_quad_tree():
    array = ctypes.c_double()
    count = ctypes.c_int()
    clusters = ctypes.c_int()
    nodes = ctypes.c_int()

    geocluster = pathlib.Path().absolute() / 'geocluster.so'
    c_lib = ctypes.CDLL(str(geocluster))
    c_lib.GetClusters(array, ctypes.byref(clusters), ctypes.byref(nodes))

@bp.route('/get_interest_points', methods=['GET'])
@cross_origin()
def get_interest_points():
    left_up = request.args.get('left_up')
    right_down = request.args.get('right_down')
    zoom_level = request.args.get('zoom_level')
    result = {
        'status': 'success',
        'message': ''
    }
    if left_up is None or right_down is None:
        result['status'] = 'fail'
        result['message'] = 'you must provide both left_up and right_down'
        return result

    libname = pathlib.Path().absolute() / 'libgeoclassifier.so'
    c_lib = ctypes.cdll.LoadLibrary(str(libname))
    json_args = {
        'upLeft': [get_float_array(left_up)[0], get_float_array(left_up)[1]],
        'downRight': [get_float_array(right_down)[0], get_float_array(right_down)[1]],
        'zoomLevel': int(zoom_level),
    }
    c_lib.GetClassifiedPointsCh.argtypes = (ctypes.c_char_p,)
    c_lib.GetClassifiedPointsCh.restype = ctypes.c_void_p
    c_lib.FreeMemory.argtypes = (ctypes.c_void_p,)
    c_lib.FreeMemory.restype = None
    c_string = ctypes.c_char_p(json.dumps(json_args).encode('utf-8'))
    _ret_c_string = c_lib.GetClassifiedPointsCh(c_string)
    ret_c_string = ctypes.cast(_ret_c_string, ctypes.c_char_p).value
    c_lib.FreeMemory(_ret_c_string)

    return ret_c_string

