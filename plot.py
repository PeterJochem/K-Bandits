import matplotlib.pyplot as plt

myFile = open("build/data.txt", "r") 

# Each line in the txt file is a history of a bandit's value estimates
for line in myFile:
    line = line.strip().split(' ')
    for i in range(0, len(line)): 
        line[i] = float(line[i]) 
    
    plt.plot(line)


myFile.close()

plt.title("K-Bandits Episode")
plt.ylabel('ValueEstimate (utils)')
plt.xlabel('time')

plt.ylim((-10, 10))

plt.show()

