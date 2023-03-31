#!/usr/bin/env python3
"""Example to read JSON data and send to InfluxDB."""
from datetime import datetime, timedelta
from dateutil import parser
import json

from influxdb_client import InfluxDBClient, Point, WriteOptions,WritePrecision
from influxdb_client.client.write_api import ASYNCHRONOUS
#false = False
#true = True





'''
class LecturaInversor(lectura_json):
    data = {
    "grid_volt": lectura_json["grid_volt"],
    "grid_freq": lectura_json["grid_freq"],
    "ac_volt": lectura_json["ac_volt"],
    "ac_freq": lectura_json["ac_freq"],
    "ac_va": lectura_json["ac_va"],
    "ac_watt": lectura_json["acc_watt"],
    "load_percent": lectura_json["load_percent"],
    "bus_volt": lectura_json["bus_volt"],
    "batt_volt": lectura_json["batt_volt"],
    "batt_charge_amps": 29,
    "batt_capacity": 75,
    "temp": 36,
    "pv_amps": 2.4,
    "pv_volts": 368.9,
    "batt_volt_scc": 0,
    "batt_discharge_amps": 0,
    "raw_status": "00010110",
    "mask_b": "00",
    "mask_c": "00",
    "pv_watts": 898,
    "mask_d": "010",
    "charge_source": [
        "solar_charging"
    ],
    "batt_volt_to_steady": false,
    "load_status": true,
    "ssc_firmware_updated": false,
    "configuration_changed": false,
    "sbu_priority_version": false
    }
'''

#client = InfluxDBClient('localhost',8086,'santos','')
json_body=[]
time = 0
with open("exito.json", "r") as json_file:
    for line in json_file: 
        lectura = json.loads(line)
        ahora = datetime.utcnow()
        hace_un_dia = ahora - timedelta(minutes=time)
        lectura["time"] = hace_un_dia.isoformat()
        time = time + 1
        json_body.append(lectura)
#print(json_body)
bucket = "AxpertLATEST"
org = "VestAssist"
token = "msVtNAhRI4MUzg_5t0yWJ9uHnPMdyoXcvoc_G_q25izxVkdY-0la9M_cVx98BsT04DIOiHHkyoJYRFY340NSeA=="
url = "http://localhost:8086"


client = InfluxDBClient(
   url=url,
   token=token,
   org=org
)

allpoints=[]
count=1
with InfluxDBClient(url=url, token=token, org=org, debug=True) as client:
    point = Point("AxpertLATEST")
    for lectura in json_body:
        write_api = client.write_api(write_options=ASYNCHRONOUS)
        datetime_of_test = datetime
        point.time(lectura["time"])
        fecha = datetime.strptime(lectura["time"], "%Y-%m-%dT%H:%M:%S.%f")
        # Obtener la hora en formato hh:mm:ss
        hora = fecha.strftime("%H:%M")
        count = count + 1
        #point.tag("FLAGMANU",hora)
        for key,value in lectura.items():
            if key == "charge_source":
                point.field(key,value[0])
            else:
                point.field(key,value)
        #print(point)
        write_api.write(bucket=bucket, org=org, record=[point], write_precision=WritePrecision.S)
        
        write_api.flush()
        write_api.close()
    #write_api.write(bucket=bucket, org=org, record=[allpoints])


print(count)
           



