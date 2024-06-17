import string
from ctypes import *


class Row(Structure):
    _fields_ = [('cols_count', c_int),
                ('points', c_double*2)]

    def __init__(self, cols):
        self.cols_count = cols
        pc = (c_char_p * cols)()
        self.cols = cast(pc, POINTER(c_char_p))


class Unit(Structure):
    _fields_ = [('rows_count', c_int),
                ('rows', POINTER(Row))]

    def __init__(self, rows, cols):
        self.rows_count = rows
        pr = (Row*rows)()
        for r in pr:
            r.__init__(cols)
        self.rows = cast(pr, POINTER(Row))


class Args(Structure):
    _fields_ = [('points', POINTER(Unit)),
                ('clusters', POINTER(Unit)),
                ('clustersBsCount', POINTER(c_int))]

    def __init__(self, points):
        self_points = Unit(len(points), 2)
        for i in range(points.rows_count):
            for j in range(points.rows[i].cols_count):
                self.points.rows[i].points[i] = points[i][j]
        self.points = cast(self_points, POINTER(Unit))