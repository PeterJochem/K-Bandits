#include <stdlib.h>
#include <iostream>
#include <cmath>

/* This method describes our agent who will learn
 * how to navigate the K-Bandits problem
 */
class Agent { 
	// Fields
	public:
		int chooseBandit(void);
	};


/* This method lets the agent choose a move
 * No arguments
 * Returns the chosen bandits next data point as specefied by
 * its probability distribution
 */ 
int Agent::chooseBandit(void) {
	return 1;
}

/* This class describes one of the K-Bandits 
 */
class Bandit {
  public:
    int sample(void);
};

/* This takes the given bandit and generates
 * the next data point for it 
 * No arguments
 * Returns the chosen bandits next data point as specefied by
 * its probability distribution
 */
int Bandit::sample (void) {
  return 1;
}


int main(void) {

	std::cout << "Hello\n";
	std::cout << "Updates\n";
		
	int i = rand();

	return 0;
}

