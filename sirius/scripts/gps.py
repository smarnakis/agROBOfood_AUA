#####
# There are several lat, lon formats. This script extract the raw data in a DMS format. All info are included in GGA sting of NMEA message
# Information about speed and heading are also included in the script. All info are included in VTG sting of NMEA message
#####

import pynmea2
import serial

ser = serial.Serial(
              
   port='/dev/ttyUSB0',        # interfacing port of USB with RTK
   baudrate = 115200,
   parity=serial.PARITY_NONE,
   stopbits=serial.STOPBITS_ONE,
   bytesize=serial.EIGHTBITS,
   timeout=0.5
   )

x=ser.readlines() #Reads all NMEA strings
GGA=x[0].decode() #Decodes GGA. Contains lat,lon data
VTG=x[4].decode() #Decodes VTG. Contains speed, heading data
vtgS= VTG.split(',') #Splits VTG string to separate values
msg = pynmea2.parse(GGA) #parses the GGA string

if len(msg.lat) > 2:  #Safety check for satelite signals
	speed=float(vtgS[7]) #Speed value from VTG string
	heading=float(vtgS[2]) #Heading value from VTG string
     if speed > 1: #Checks if sprayer is stationary, for accurate heading
     	latitude = '%02d°%02d′%07.4f″' % (msg.latitude, msg.latitude_minutes, msg.latitude_seconds) #calculates latitude in DMS
		longitude = '%02d°%02d′%07.4f″' % (msg.longitude, msg.longitude_minutes, msg.longitude_seconds) #calculates longitude in DMS
     else:
     	print('Sprayer stationary, please start moving')
     else:
         print('Satellite insufficiency, Retrying...')