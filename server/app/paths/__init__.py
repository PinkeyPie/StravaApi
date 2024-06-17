from flask import Blueprint

bp = Blueprint('paths', __name__)

from app.paths import routes
