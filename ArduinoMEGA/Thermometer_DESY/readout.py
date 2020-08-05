import serial
import time
import io

import argparse
parser = argparse.ArgumentParser()
parser.add_argument('--outputFilePath', type=str, help='path to the output file', default="test.txt", required=True)
parser.add_argument("--portName", type=str, help="port", default="/dev/cu.usbmodem1411")
args = parser.parse_args()

outputFilePath=args.outputFilePath

#http://pyserial.readthedocs.io/en/latest/shortintro.html
ser = serial.Serial(
    port=args.portName,
    baudrate=19200,
    parity=serial.PARITY_ODD,
    stopbits=serial.STOPBITS_TWO,
    bytesize=serial.SEVENBITS
)

with open(outputFilePath, "w") as outfile:
	outfile.write("Date,Temp_resistor[C],Temp_DHT11[C],Humidity_DHT11[%]\n")

while (True):
	if (ser.is_open):
		data = ser.read(13)
		if not "XX.XX,XX,XX" in data:
			print time.ctime()+":  "+data
			with open(outputFilePath, "a") as outfile:
				outfile.write(time.ctime()+","+data)
