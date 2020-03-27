import matplotlib.pyplot as plt

# get number of agents from the command line

numAgents = 5

for j in range(numAgents):

    myFile = open("build/agent" + str(j) + ".txt", "r") 

    # Each line in the txt file is a history of a bandit's value estimates
    for line in myFile:
        line = line.strip().split(' ')
   
        for i in range(0, len(line) ): 
            line[i] = float(line[i]) 
    
    plt.plot(line, label = "Explore Rate = " + str( round( (j + 1) * 0.1, 3 ) ) )
    plt.legend()

    myFile.close()


plt.title("K-Bandits Varying Explore Rate")
plt.ylabel('AverageReward (utils)')
plt.xlabel('time')

plt.ylim((0, 30))

plt.show()

