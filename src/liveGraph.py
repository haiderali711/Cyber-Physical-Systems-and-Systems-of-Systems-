import socket
import json
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation


localIP     = "127.0.0.1"
localPort   = 1234

bufferSize  = 1024
#msgFromServer       = "Hello UDP Client"
#bytesToSend         = str.encode(msgFromServer)

# Create a datagram socket

UDPServerSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
# Bind to address and ip
UDPServerSocket.bind((localIP, localPort))


plt.style.use('fivethirtyeight')

timestamp = []
original = []
angle = []


def animate(i):

	data,address = UDPServerSocket.recvfrom(bufferSize)
	jsonData = json.loads(data)

	timestamp.append(float(jsonData['Timestamp']))
	original.append(float(jsonData['Original']))
	angle.append(float(jsonData['OurSteering']))

	plt.cla()
	plt.plot(timestamp,original, label='Original angle over time',color="blue")
	plt.plot(timestamp,angle, label='Calculated angle over time',color="green")
	
	i = 0
	total = len(angle)
	n_acceptable = 0
	for i in range(total):
		if angle[i] <= original[i] + original[i]*0.5 and angle[i] >= original[i] - original[i]*0.5:
			n_acceptable+=1
	print("Acceptability percentage: " + str(n_acceptable/total*100));
	if n_acceptable/total*100>=20: 
		print("Acceptable")
	else:
		print("NOT Acceptable")



ani = FuncAnimation(plt.gcf(),animate,interval=1)



print("Waiting for the video to run ...")

plt.xlabel('timestamp')
plt.ylabel('angle')
plt.title('Steering Wheel Angle')
plt.legend()
plt.show()


    
