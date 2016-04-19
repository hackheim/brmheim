from flask import Flask, render_template, Response
from flask.ext.mysqldb import MySQL
from werkzeug.routing import BaseConverter, ValidationError
import ssl

from config import get_db_config
import json
from datetime import datetime, timedelta

conf = get_db_config()

# SSL
if 'certificate' in conf['app'] and 'privkey' in conf['app']:
  ssl_context = ssl.SSLContext(ssl.PROTOCOL_TLSv1_2)
  ssl_context.load_cert_chain(conf['app']['certificate'], conf['app']['privkey'])
else:
  ssl_context = None
# Reads config.ini from the directory you execute the application from.

app = Flask(__name__)
app.config['DEBUG'] = conf['app']['debug']
app.config['MYSQL_HOST'] = conf['mysqld']['host']
app.config['MYSQL_USER'] = conf['mysqld']['user']
app.config['MYSQL_PASSWORD'] = conf['mysqld']['password']
app.config['MYSQL_DB'] = conf['mysqld']['db']
app.config['MYSQL_PORT'] = int(conf['mysqld']['port'])
#app.config['MYSQL_UNIX_SOCKET', None)
app.config['MYSQL_CONNECT_TIMEOUT'] = 10
#app.config['MYSQL_READ_DEFAULT_FILE', None)
app.config['MYSQL_USE_UNICODE'] =  True
app.config['MYSQL_CHARSET'] = 'utf8'
#app.config['MYSQL_SQL_MODE', None)
#app.config['MYSQL_CURSORCLASS', None)

mysql = MySQL(app)

class DateTimeEncoder(json.JSONEncoder):
    def default(self, obj):
        if isinstance(obj, datetime):
            encoded_object = list(obj.timetuple())[0:6]
        else:
            encoded_object = json.JSONEncoder.default(self, obj)
        return encoded_object

class DateConverter(BaseConverter):
    """Extracts a ISO8601 date from the path and validates it."""

    regex = r'\d{4}-\d{2}-\d{2}'

    def to_python(self, value):
        try:
            return datetime.strptime(value, '%Y-%m-%d').date()
        except ValueError:
            raise ValidationError()

    def to_url(self, value):
        return value.strftime('%Y-%m-%d')


app.url_map.converters['date'] = DateConverter

class DateTimeConverter(BaseConverter):
    """Extracts a ISO8601 date from the path and validates it."""

    regex = r'\d{4}-\d{2}-\d{2}-\d{2}-\d{2}-\d{2}'

    def to_python(self, value):
        try:
            return datetime.strptime(value, '%Y-%m-%d-%H-%M-%S')
        except ValueError:
            raise ValidationError()

    def to_url(self, value):
        return value.strftime('%Y-%m-%d-%H-%M-%S')


app.url_map.converters['datetime'] = DateTimeConverter

@app.route("/")
def hello():
  return render_template('index.html')

@app.route("/volume")
@app.route("/volume/")
@app.route("/volume/<int:client_id>")
@app.route("/volume/<int:client_id>/")
@app.route("/volume/<int:client_id>/<datetime:time_from>/<datetime:time_to>")
@app.route("/volume/<int:client_id>/<date:time_from>/<date:time_to>")
@app.route("/volume/<int:client_id>/<datetime:time_from>/<date:time_to>")
@app.route("/volume/<int:client_id>/<date:time_from>/<datetime:time_to>")
@app.route("/volume/<datetime:time_from>/<datetime:time_to>")
@app.route("/volume/<date:time_from>/<date:time_to>")
@app.route("/volume/<datetime:time_from>/<date:time_to>")
@app.route("/volume/<date:time_from>/<datetime:time_to>")
def volume(client_id=False, time_from=False, time_to=False):
  cur = mysql.connection.cursor()
  if not client_id and not time_from:
     cur.execute('''SELECT * FROM clients LIMIT 1000''')
     clients = cur.fetchall()
     cur.execute('''SELECT * FROM volumes LIMIT 1000''')
     volume = cur.fetchall()
  elif not time_from and client_id:
     cur.execute('''SELECT * FROM clients WHERE clients.id = %s''', (client_id,))
     clients = cur.fetchall()
     cur.execute('''SELECT * FROM volumes WHERE volumes.client_id = %s LIMIT 1000''', (client_id,))
     volume = cur.fetchall()
  elif time_from and not client_id:
     clients = []
     cur.execute('''SELECT * FROM volumes WHERE volumes.created >= %s AND volumes.created <= %s LIMIT 1000''', (time_from, time_to,))
     volume = cur.fetchall()
  else:
     cur.execute('''SELECT * FROM clients WHERE clients.id = %s LIMIT 1000''', (client_id,))
     clients = cur.fetchall()
     cur.execute('''SELECT * FROM volumes WHERE volumes.client_id = %s AND volumes.created >= %s AND volumes.created <= %s LIMIT 1000''', (client_id,time_from,time_to))
     volume = cur.fetchall()

  data = {
    'clients': clients,
    'volume': volume
  }
  return Response(json.dumps(data, cls=DateTimeEncoder), status=200, mimetype='application/json')

@app.route("/test")
def test():
  cur = mysql.connection.cursor()
  cur.execute('''SELECT * FROM clients''')
  rv = cur.fetchall()
  js = json.dumps(rv, cls=DateTimeEncoder)
  resp = Response(js, status=200, mimetype='application/json')
  return resp


if __name__ == "__main__":
    app.run(host=conf['app']['host'], ssl_context=ssl_context)
