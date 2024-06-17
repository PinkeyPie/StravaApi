import uuid

from app import db
from sqlalchemy.dialects.postgresql import UUID
from geoalchemy2 import Geometry
from sqlalchemy import Table, Column, Index, String, PrimaryKeyConstraint, ForeignKeyConstraint
from ctypes import Structure


class InterestPoints(db.Model):
    __tablename__ = 'InterestPoints'
    id = db.Column(UUID(as_uuid=True), default=uuid.uuid4)
    address = db.Column(db.Text, nullable=False)
    coordinates = db.Column(Geometry('POINT', srid=4326), nullable=False)
    region = db.Column(UUID(as_uuid=True), nullable=False)
    comment = db.Column(db.Text)
    __table_args__ = (
        PrimaryKeyConstraint('id', name='bs_id'),
        ForeignKeyConstraint(['region'], ['Region.id'])
    )


class Region(db.Model):
    __tablename__ = 'Region'
    id = db.Column(UUID(as_uuid=True), primary_key=True, default=uuid.uuid4)
    name = db.Column(db.Text, nullable=False)