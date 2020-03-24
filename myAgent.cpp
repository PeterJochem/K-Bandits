#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <random>



/* This class describes one of the K-Bandits 
 */
class Bandit {

        double variance;
        double mean;
        std::default_random_engine generator;
        std::normal_distribution<double> myDistribution;

        public:

                // Constructor  
                Bandit(double newMean, double newVariance) {
                        variance = newVariance;
                        mean = newMean;

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
	
	Bandit* myArray;
	
	public:
		// Constructor
		Agent(int numBandits1) {
			
			// numBandits = numBandits1;
			this->numBandits = numBandits1;
			
			std::cout << this->numBandits;

			myArray = (Bandit*)malloc(sizeof(Bandit) * numBandits);
		}
		
		int chooseBandit(void);			
	};



/* This method lets the agent choose a move
 * No arguments
 * Returns the chosen bandits next data point as specefied by
 * its probability distribution
 */ 
int Agent::chooseBandit(void) {
	// Fix me
	return 1;
}



int main(void) {

	std::cout << "Hello\n";
	std::cout << "Updates\n";
			
	// Create an agent
	Agent myAgent = Agent(10);
	
	// Create K-bandits
	Bandit myBandit = Bandit(1.0, 0.0);
		
	
	std::cout << "\n";		
	std::cout << myBandit.sample();
	std::cout << "\n";

	// Learn
	// Plot data


	return 0;
}

