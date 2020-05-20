import matplotlib.pyplot as graph
import csv

angle= []
timestamp= []
original= []

with open('myCSV/steering.csv', 'r') as csvfile:
	plots= csv.reader(csvfile, delimiter=',')
	next(plots)
	for row in plots:
		angle.append(float(row[0]))
		timestamp.append(float(row[1]))
		original.append(float(row[2]))

	i = len(timestamp) -1;
	while i > 0 :
		if timestamp[i] < timestamp[i-1]:
			del timestamp[i-1]
			del angle[i-1]
			del original[i-1]
			i+=1
		else:
			i-=1


graph.plot(timestamp, original, label='Original angle over time',color="blue")
graph.plot(timestamp, angle, label='Calculated angle over time',color="magenta")
graph.xlabel('timestamp')
graph.ylabel('angle')
graph.title('Steering Wheel Angle')
graph.legend()
graph.show()
