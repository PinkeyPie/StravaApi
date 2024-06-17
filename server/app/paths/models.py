from app import db
from sqlalchemy.dialects.postgresql import UUID
from geoalchemy2 import Geometry


class Path(db.Model):
    __tablename__ = 'Path'

    id = db.Column(UUID(as_uuid=True), primary_key=True)
    name = db.Column(db.String, nullable=False)


class GpsData(db.Model):
    __tablename__ = 'GpsData'

    id = db.Column(UUID(as_uuid=True), primary_key=True)
    coordinates = db.Column(Geometry('POINT'), nullable=False)
    time = db.Column(db.DateTime, nullable=False)
    altitude = db.Column(db.Float, nullable=False)
    speed = db.Column(db.Float, nullable=False)
    heading = db.Column(db.Float, nullable=False)
    path_id = db.Column(UUID(as_uuid=True), db.ForeignKey('path.id'), nullable=False)
