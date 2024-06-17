FROM tiangolo/uwsgi-nginx-flask:python3.11
LABEL authors="leon"
RUN apk --update add bash nano
COPY ./requirements.txt /var/www/requirements.txt
RUN pip install -r /var/www/requirements.txt
ENTRYPOINT ["top", "-b"]