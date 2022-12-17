import numpy as np
from matplotlib import pyplot as plt
	
if __name__ == '__main__':
		
	db = open("saida.txt", "r")
	data = []

	for line in db:
		line_s = line.split()
		data.append(float(line_s[0]))
		
	db.close()
	
	plt.plot([_ for _ in range(len(data))], data)
	plt.show()
	
	