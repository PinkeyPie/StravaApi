from flask import Blueprint

bp = Blueprint('interest_points', __name__)

from app.interest_points import routes
