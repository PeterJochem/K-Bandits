#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <random>
#include <fstream>
#include <string>

// Globals
int maxMean = 10;
int maxVariance = 10;

int count = 0;

/* This class describes one of the K-Bandits 
 */
class Bandit {
 
        public:
		double variance;
        	double mean;
	        int timesSampled;
       		std::default_random_engine generator;
        	std::normal_distribution<double> myDistribution;

                // Constructor  
                Bandit(double mean, double variance) {
                        this->variance = variance;
                        this->mean = mean;
				
			this->timesSampled = 0;	

                        // Initialize the distribution
                        myDistribution = std::normal_distribution<double>(mean, variance);
                }

		 /* Ovrload the print of a Bandit object
		  */
		/* Describe this method here
		 */
		friend std::ostream& operator<< (std::ostream &out, const Bandit &myBandit);

                // Agent requests another data point
                double sample(void);
};

std::ostream& operator<< (std::ostream &out, const Bandit &myBandit) {
    // Since operator<< is a friend of the Point class, we can access Point's members directly.
    out << "(" << myBandit.mean << ", " << myBandit.variance << ")" << std::endl; // actual output done here
 
    return out; // return std::ostream so we can chain calls to operator<<
}

/* This takes the given bandit and generates
 * the next data point for it 
 * No arguments
 * Returns the chosen bandits next data point as specefied by
 * its probability distribution
 */
double Bandit::sample(void) {
        return myDistribution(generator);
}


/* This method describes our agent who will learn
 * how to navigate the K-Bandits problem
 */
class Agent { 
	
	// List of all our agent's bandits
	Bandit** allBandits;	

	double* valueEstimate;

	double exploreRate;
        double exploreRateDecay;
	
	int numSamples;
	
	double totalReward = 0.0;
	
	public:
		// How many times have we sampled from all bandits
		int currentIndex;
		int numBandits;	
		// These are the agent's estimates of how valuable each action is
        	double** allData;
		
		// This stores a list of average reward 
	        double* averageReward;

		// Constructor
		// numBandits is the number of bandits
		// numSamples is the length of the episode ie 
		// how many times the agent can choose an action
		Agent(int numBandits, int numSamples, double exploreRate) {
			
			this->currentIndex = 0;	

			this->numBandits = numBandits;
			allBandits = static_cast<Bandit**>(malloc(sizeof(Bandit*) * numBandits) );
			
			allData = static_cast<double**>(malloc(sizeof(double**) * numSamples) );

			averageReward = static_cast<double*>(malloc(sizeof(double) * numSamples) );

			valueEstimate = static_cast<double*>(malloc(sizeof(double) * numBandits) );

			this->exploreRate = exploreRate;
                        this->exploreRateDecay = 0.1;
			
			this->numSamples = numSamples;

			// Create a batch of bandits	
			for (int i = 0; i < numBandits; ++i) {
				
				// Create new bandit with a distribution
				// specefied by gloabls max variance etc.
				double nextVariance = 2.0; // (rand() % maxVariance) + 0.5;
				double nextMean = count + 1; // (double(rand() % maxMean) );
				
				count++;

				allBandits[i] = new Bandit(nextMean, nextVariance);
					
				// How to initialize this?
				valueEstimate[i] = 10.0;
			}

			count = 0;

			// Init the list of all observed data
			for (int i = 0; i < numSamples; ++i) {
				
				allData[i] = static_cast<double*>(malloc(sizeof(double*) * numBandits) );

                                for (int j = 0; j < numBandits; ++j) {
                                        // Zero out the list of data points
                                        allData[i][j] = 0.0;
                                }
			}
		}	
		
		// Describe
		void runEpisode(void);
		// Defined below - chooses agent's next bandit
		double chooseBandit(bool = true);		
		// Implements the bandit's greedy choice making	
		double chooseGreedyBandit(bool = true);
		// implements the bandit's exploring methods
		double explore(bool = true);
		void updateEstimate(int, double);
		void printAllEstimates(void);
		void writeData(void);
		void printData(void);
		double sampleAverageReward(void);

		
};

/* Describe this method here
 * Freeze the policy and compute the average reward
 */
double Agent::sampleAverageReward(void) {

	int timesToSample = 100;
	int currentSample = 0;
	
	double totalReward = 0.0;
	while ( currentSample < timesToSample ) {

                // Chooses next data point and writes value
                // to the correct data structure                
                totalReward = totalReward + chooseBandit(false);
                currentSample++;
                //currentAction++;
        }

	return totalReward / timesToSample;
}



/* Describe this method here
 */ 
void Agent::printData(void) {
	
	for (int i = 0; i < this->numBandits; ++i) {
		std::cout << *allBandits[i]; 
	}
}

/* Describe this method here
 */ 
void Agent::writeData(void) {
	
	std::ofstream myFile;	
	myFile.open ("data.txt");
	for (int i = 0; i < numBandits; ++i) {
		
		for (int j = 0; j < numSamples; ++j) {
			
			myFile << allData[j][i] << " ";	
		}
		myFile << "\n";
	}

        myFile.close();
}

/* Describe this method 
 */
void Agent::printAllEstimates(void) {
		
	for (int i = 0; i < numBandits; ++i) {
		std::cout << valueEstimate[i] << std::endl; 
	}
}


/* Describe this method here
 */
void Agent::runEpisode(void) {

	int currentAction = 0;
	while ( currentAction < numSamples ) {

		// Chooses next data point and writes value
		// to the correct data structure		
		chooseBandit();
		currentIndex++;
		currentAction++;
		
		// Record the average reward under this policy	
		averageReward[currentAction] = sampleAverageReward();	
	}	

	// Write data to list so we can plot it later
}


/* Describe this method here
 */
void Agent::updateEstimate(int agentNumber, double nextEstimate) {

	// Iterative update rule: Sample_N + ( (1/N) * (Old_Avg - Sample_N) )
	
	int numSamples = allBandits[agentNumber]->timesSampled;
	double newAverage = 0; 

	if ( numSamples == 0 ) {
		newAverage = nextEstimate;
	}
	else {
		newAverage = valueEstimate[agentNumber] + ( (1.0/numSamples) * (nextEstimate - valueEstimate[agentNumber]) );   	
	}


	//std::cout << newAverage << "\n";
	valueEstimate[agentNumber] = newAverage;	
		
	// Record the agent's estimates at this point in time
	if ( currentIndex != 0 ) {
		// Copy the prior times estimates
		for (int i = 0; i < numBandits; ++i) {
			allData[currentIndex][i] = allData[currentIndex - 1][i];  
		}

		// Only this estimate changes
		allData[currentIndex][agentNumber] = newAverage; 
	}
	else {
		allData[currentIndex][agentNumber] = newAverage;
	}
		
	return;
}

/* Describe this method here
 */
double Agent::chooseGreedyBandit(bool update) {
	
	double maxValue = -10000000.0;
	int index = 0;

	// Traverse the list of value estimates to find the max
	for (int i = 0; i < numBandits; ++i) {
		
		double nextValue = valueEstimate[i]; 

		if ( nextValue > maxValue ) { 
			maxValue = nextValue; 
			index = i;
		}
	}
	
	double nextValue = (allBandits[index])->sample();

	
	if ( update ) {
		updateEstimate(index, nextValue);
		allBandits[index]->timesSampled++;
	}

	return nextValue;
}

/* Describe this method here
 */
double Agent::explore(bool update) {

	// Return an index between 0 and numBandits
	int randomIndex = ( rand() % numBandits );	
		
	double nextValue = allBandits[randomIndex]->sample();
	
	if ( update ) {	
		updateEstimate(randomIndex, nextValue);	
		allBandits[randomIndex]->timesSampled++;
	}

	return nextValue;
}	


/* This method lets the agent choose a move
 * No arguments
 * Returns the chosen bandits next data point as specefied by
 * its probability distribution
 */ 
double Agent::chooseBandit(bool update) {
	// Add a greedy method - ie choose only greedily
	
	double exploreNow = ( (double) rand() ) / RAND_MAX;
	
	double newReward = 0.0;
		
	if ( exploreRate > exploreNow ) {
		// Choose a bandit at random
		//totalReward = totalReward + explore(update);
		newReward = explore(update);
	}
	else {
		// totalReward = totalReward + chooseGreedyBandit(update);
		newReward = chooseGreedyBandit(update);
	}
	
	totalReward = totalReward + newReward;
	return newReward;
}

/* This class holds a list of agents and varies
 * their search rates to see/compare diffrent approaches
 */
class varyParameters {

        public:
                int numberAgents;
		int numSamples;
		Agent** allAgents;

		// Constructor
                varyParameters(int numberAgents) {
			this->numberAgents = numberAgents; 
                
			allAgents = static_cast<Agent**> ( malloc(sizeof(Agent) * numberAgents) );
			
			this->numSamples = 1000;
			
			// Create the agents
			for (int i = 0; i < numberAgents; ++i) {
				
				// VARY THIS!
				double exploreRate = 0.1 * (i + 1);
				
				int numberBandits = 10;
				allAgents[i] = new Agent(numberBandits, numSamples, exploreRate);	
			}
		}

		
		/* Describe this method here
		 */
		void writeData(int agentNumber) {
			std::ofstream myFile;
		        myFile.open("agent" + std::to_string(agentNumber) + ".txt");
        		
			for (int i = 0; i < numSamples; ++i) {	
				myFile << allAgents[agentNumber]->averageReward[i] << " ";
			}

			myFile << "\n";
       			myFile.close();
		}

		/* Describe this method here
                 */
                void runAllEpisodes(void) {

                        // Run an episode for each agent
                        for (int i = 0; i < numberAgents; ++i) {

                                allAgents[i]->runEpisode();
                                // Write data to list so we can plot it later
                                writeData(i);
			}
                }
};


int main(void) {

	std::cout << "K_Bandits!\n";
			
	// Create an agent
	//Agent myAgent = Agent(5, 100, 0.1);					
	//myAgent.runEpisode();	
	//myAgent.printData();
	// Write data to file to be plotted
  	//myAgent.writeData();

	varyParameters myExperiment(10);
	myExperiment.runAllEpisodes();


	return 0;
}

