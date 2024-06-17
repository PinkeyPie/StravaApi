import logging
from logging.handlers import RotatingFileHandler
import os
from datetime import timedelta
from flask import Flask, session, request, current_app
from flask_sqlalchemy import SQLAlchemy
from flask_cors import CORS
from config import Config
from pathlib import Path
import shutil

db = SQLAlchemy()
cors = CORS()

def create_app(config_name=Config):
    app = Flask(__name__)
    app.config.from_object(config_name)
    db.init_app(app)
    cors.init_app(app)
    path = Path(os.path.abspath(__file__))
    lib_dir = path.parent.parent.parent.joinpath('geoclassifier/cmake-build-debug/libgeoclassifier.so')
    shutil.copyfile(str(lib_dir), str(path.parent.parent.joinpath('libgeoclassifier.so')))

    from app.paths import bp as paths_bp
    app.register_blueprint(paths_bp, url_prefix='/paths')

    from app.interest_points import ip as interest_points_bp
    app.register_blueprint(interest_points_bp, url_prefix='/interest_points')

    return app
