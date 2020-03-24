#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <random>

// Globals
int maxMean = 15;
int maxVariance = 5;

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
				
			timesSampled = 0;	

                        // Initialize the distribution
                        myDistribution = std::normal_distribution<double>(mean, variance);
                }

                // Agent requests another data point
                double sample(void);
};

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

	public:
		// Constructor
		// numBandits is the number of bandits
		// numSamples is the length of the episode ie 
		// how many times the agent can choose an action
		Agent(int numBandits, int numSamples) {
			
			this->numBandits = numBandits;
			allBandits = static_cast<Bandit**>(malloc(sizeof(Bandit*) * numBandits) );
			
			allData = static_cast<double**>(malloc(sizeof(double*) * numBandits) );
			
			valueEstimate = static_cast<double*>(malloc(sizeof(double) * numBandits) );

			this->exploreRate = 0.1;
                        this->exploreRateDecay = 0.1;
			
			this->numSamples = numSamples;

			// Create a batch of bandits	
			for (int i = 0; i < numBandits; ++i) {
				
				// Create new bandit with a distribution
				// specefied by gloabls max variance etc.
				int nextVariance = rand() % maxVariance;
				int nextMean = rand() % maxMean;

				allBandits[i] = new Bandit(nextMean, nextVariance);
					
				valueEstimate[i] = 0.0;

				allData[i] = static_cast<double*>(malloc(sizeof(double*) * numSamples) ); 

				for (int j = 0; j < numSamples; ++j) {
					// Zero out the list of data points
					allData[i][j] = 0.0;
				}
			}

					
			/* Test to make sure I set this up right	
			for (int i = 0; i < numBandits; ++i) {
				std::cout << "\n";
			        std::cout << ( **(allBandits + i) ).sample();
        			std::cout << "\n";
			}
			*/	

		}
		
		// Describe
		void learn(void);

		// Defined below - chooses agent's next bandit
		void chooseBandit(void);		
		// Implements the bandit's greedy choice making	
		double chooseGreedyBandit(void);
		// implements the bandit's exploring methods
		double explore(void);
	};


/* Describe this method here
 */
void Agent::learn(void) {

	// Sample
	int currentAction = 0;
	while ( currentAction < numSamples ) {

		// Chooses next data point and writes value
		// to the correct data structure		
		chooseBandit();
		
		// std::cout << "more data " << std::endl; 

		currentAction++;
	}	

	// Write data to list so we can plot it later
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
        allData[index][allBandits[index]->timesSampled] = nextValue;
	allBandits[index]->timesSampled++;

        // Update the agent's value estimate 
        //valueEstimate[index] =  


	return nextValue;
}

/* Describe this method here
 */
double Agent::explore(void) {

	// Return an index between 0 and numBandits
	int randomIndex = ( rand() % numBandits );	
		
	double nextValue = allBandits[randomIndex]->sample();
	
	// Record the observed data point
	allData[randomIndex][allBandits[randomIndex]->timesSampled] = nextValue; 	
	allBandits[randomIndex]->timesSampled++;

	// Update the agent's value estimate 
	//valueEstimate[randomIndex] =  
	
	return nextValue;
}	


/* This method lets the agent choose a move
 * No arguments
 * Returns the chosen bandits next data point as specefied by
 * its probability distribution
 */ 
void Agent::chooseBandit(void) {
	// Add a greedy method - ie choose only greedily
	
	int nextVariance = rand() % maxVariance;
	double exploreNow = ( (double) rand() ) / RAND_MAX;
	
		
	if ( exploreRate > exploreNow ) {
		// Choose a bandit at random
		explore();
	}
	else {
		chooseGreedyBandit();
	}
	
	return;
}


int main(void) {

	std::cout << "K_Bandits!\n";
			
	// Create an agent
	Agent myAgent = Agent(10, 1000);	

	myAgent.learn();	
		
	// Learn
	// Plot data


	return 0;
}

