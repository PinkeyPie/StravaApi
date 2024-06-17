import os
from datetime import timedelta

basedir = os.path.abspath(os.path.dirname(__file__))


class Config(object):
    SECRET_KEY = os.environ.get('SECRET_KEY') or 'you-will-never-guess'
    REMEMBER_COOKIE_DURATION = timedelta(hours=72)
    PERMANENT_SESSION_LIFETIME = timedelta(minutes=120)
    UPLOAD_FOLDER = ''
    SQLALCHEMY_DATABASE_URI = os.environ.get("DATABASE_URL") or \
        "postgresql://postgres:postgres@localhost/strava"
    SQLALCHEMY_TRACK_MODIFICATIONS = False