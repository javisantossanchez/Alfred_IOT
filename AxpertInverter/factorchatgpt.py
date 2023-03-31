#!/usr/bin/env python3

"""Example to read JSON data and send to InfluxDB."""

from datetime import datetime, timedelta
from dateutil import parser
import json
from influxdb_client import InfluxDBClient, Point, WriteOptions, WritePrecision
from influxdb_client.client.write_api import ASYNCHRONOUS




def add_timestamp(json_data):
    # Add timestamp to json
    now = datetime.utcnow()
    json_data["time"] = now.strftime("%Y-%m-%dT%H:%M:%SZ")
    return json_data


# Datos de conexión a InfluxDB
bucket = "AxpertMANUEL"
org = "VestAssist"
token = "msVtNAhRI4MUzg_5t0yWJ9uHnPMdyoXcvoc_G_q25izxVkdY-0la9M_cVx98BsT04DIOiHHkyoJYRFY340NSeA=="
url = "http://localhost:8086"

# Lectura del archivo JSON
json_body = []
time = 0
with open("exito.json", "r") as json_file:
    for line in json_file: 
        lectura = json.loads(line)
        ahora = datetime.utcnow()
        hace_un_dia = ahora - timedelta(minutes=time)
        lectura["time"] = hace_un_dia.isoformat()
        time = time + 1
        json_body.append(lectura)

# Conexión a InfluxDB
client = InfluxDBClient(url=url, token=token, org=org, debug=True)

# Escritura de datos en InfluxDB
with client:
    write_api = client.write_api(write_options=ASYNCHRONOUS)
    for lectura in json_body:
        point = Point(bucket)
        point.time(lectura["time"])
        timestamp = datetime.fromisoformat(lectura["time"])
        fecha = datetime.strptime(lectura["time"], "%Y-%m-%dT%H:%M:%S.%f")
        hora = fecha.strftime("%H:%M")
        for key, value in lectura.items():
            if key == "charge_source":
                point.field(key, value[0])
            else:
                point.field(key, value)
        # Add tag with day of month
        day_of_month = timestamp.day
        point.tag("day_of_month", str(day_of_month))
        write_api.write(bucket=bucket, org=org, record=point, write_precision=WritePrecision.S)
    write_api.flush()
