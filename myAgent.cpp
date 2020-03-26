#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <random>
#include <fstream>

// Globals
int maxMean = 10;
int maxVariance = 3;



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
	// Fields
	int numBandits;
	
	// List of all our agent's bandits
	Bandit** allBandits;	

	// These are the agent's estimates of how valuable each action is
	double** allData; 	
	
	double* valueEstimate;

	double exploreRate;
        double exploreRateDecay;
	
	int numSamples;
	
	double totalReward = 0.0;

	public:
		// How many times have we sampled from all bandits
		int currentIndex;

		// Constructor
		// numBandits is the number of bandits
		// numSamples is the length of the episode ie 
		// how many times the agent can choose an action
		Agent(int numBandits, int numSamples, double exploreRate) {
			
			this->currentIndex = 0;	

			this->numBandits = numBandits;
			allBandits = static_cast<Bandit**>(malloc(sizeof(Bandit*) * numBandits) );
			
			allData = static_cast<double**>(malloc(sizeof(double**) * numSamples) );
			
			valueEstimate = static_cast<double*>(malloc(sizeof(double) * numBandits) );

			this->exploreRate = exploreRate;
                        this->exploreRateDecay = 0.1;
			
			this->numSamples = numSamples;

			// Create a batch of bandits	
			for (int i = 0; i < numBandits; ++i) {
				
				// Create new bandit with a distribution
				// specefied by gloabls max variance etc.
				double nextVariance = (rand() % maxVariance) + 0.5;
				double nextMean = (double(rand() % maxMean) );

				allBandits[i] = new Bandit(nextMean, nextVariance);
					
				// How to initialize this?
				valueEstimate[i] = 1000.0;
			}

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
		void chooseBandit(void);		
		// Implements the bandit's greedy choice making	
		double chooseGreedyBandit(void);
		// implements the bandit's exploring methods
		double explore(void);
		void updateEstimate(int, double);
		void printAllEstimates(void);
		void writeData(void);
		void printData(void);
};

/* Describe this method here
 * Freeze the policy and compute the average reward
 */
double sampleAverageReward(void) {

	int timesToSample = 100;
	int currentSample = 0;
	while ( currentSample < timesToSample ) {

                // Chooses next data point and writes value
                // to the correct data structure                
                //chooseBandit();
		;
                //currentIndex++;
                //currentAction++;
        }
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

	// Re-set Agent's data

	// Sample
	int currentAction = 0;
	while ( currentAction < numSamples ) {

		// Chooses next data point and writes value
		// to the correct data structure		
		chooseBandit();
		currentIndex++;
		currentAction++;
		
		// Compute the average reward so far
		

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
double Agent::chooseGreedyBandit(void) {
	
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

	// Record the observed data point
        //allData[index][allBandits[index]->timesSampled] = nextValue;
	updateEstimate(index, nextValue);
	allBandits[index]->timesSampled++;

	return nextValue;
}

/* Describe this method here
 */
double Agent::explore(void) {

	// Return an index between 0 and numBandits
	int randomIndex = ( rand() % numBandits );	
		
	double nextValue = allBandits[randomIndex]->sample();
	
	// Record the observed data point
	//allData[randomIndex][allBandits[randomIndex]->timesSampled] = nextValue; 	
	
	updateEstimate(randomIndex, nextValue);	
	allBandits[randomIndex]->timesSampled++;
	
	return nextValue;
}	


/* This method lets the agent choose a move
 * No arguments
 * Returns the chosen bandits next data point as specefied by
 * its probability distribution
 */ 
void Agent::chooseBandit(void) {
	// Add a greedy method - ie choose only greedily
	
	double exploreNow = ( (double) rand() ) / RAND_MAX;
		
	if ( exploreRate > exploreNow ) {
		// Choose a bandit at random
		totalReward = totalReward + explore();
	}
	else {
		totalReward = totalReward + chooseGreedyBandit();
	}
	
	return;
}

/* This class holds a list of agents and varies
 * their search rates to see/compare diffrent approaches
 */
class varyParameters {

        public:
                int numberAgents;
		Agent** allAgents;

                varyParameters(int numberAgents) {
			this->numberAgents = numberAgents; 
                        //allAgents = new Agent(0.0, 0.0, 0.0);
                
			allAgents = static_cast<Agent**> ( malloc(sizeof(Agent) * numberAgents) );

			for (int i = 0; i < numberAgents; ++i) {
				int numberBandits = 5;
				int numSamples = 100;
				double exploreRate = 0.1;
				allAgents[i] = new Agent(numberBandits, numSamples, exploreRate);	
			}
		}

		/* Describe this method here
		 */ 
		void runAllEpisodes(void) {

			for (int i = 0; i < numAgents; ++i) {

				int currentAction = 0;
		        	while ( currentAction < numSamples ) {

                			// Chooses next data point and writes value
                			// to the correct data structure                
                			// allBandits[i].chooseBandit();
                			// allBandits[i].currentIndex++;
                			currentAction++;

                			// Compute the average reward so far
        			}

				currentAction = 0;

        			// Write data to list so we can plot it later
			}		
		}
};


int main(void) {

	std::cout << "K_Bandits!\n";
			
	// Create an agent
	Agent myAgent = Agent(5, 100, 0.1);	
				
	myAgent.runEpisode();	
	
	myAgent.printData();

	// Write data to file to be plotted
  	myAgent.writeData();

	return 0;
}

