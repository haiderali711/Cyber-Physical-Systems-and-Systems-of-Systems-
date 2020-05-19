import matplotlib.pyplot as graph
import csv

angle= []
timestamp= []

with open('myCSV/steering.csv', 'r') as csvfile:
	plots= csv.reader(csvfile, delimiter=',')

	for row in plots:
		angle.append(float(row[0]))
		timestamp.append(float(row[1]))

	i = len(timestamp) -1;
	while i > 0 :
		if timestamp[i] < timestamp[i-1]:
			del timestamp[i-1]
			del angle[i-1]
			i+=1
		else:
			i-=1


graph.plot(timestamp, angle, label='Angle over time')
graph.xlabel('timestamp')
graph.ylabel('angle')
graph.title('Steering Wheel Angle')
graph.legend()
graph.show()
