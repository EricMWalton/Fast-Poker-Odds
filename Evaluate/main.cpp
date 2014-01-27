
// EVALUATES two random starting hands, enumerating all possible boards and computing the win probability of each
// tests both enumerate.h and odds_byCount.h

#include "../headers/std_headers.h"

#include <time.h> // TIME(), timing tests



void printOutcomes(DECK_MASK * hands, unsigned int aWins, unsigned int bWins, unsigned int ties, clock_t start)
	{

	 
	const unsigned int total = aWins + bWins + ties;

	float leftEV =  1.f * aWins / total + 0.5f * ties / total;
	float rightEV = 1.f * bWins / total + 0.5f * ties / total;

	printf("total boards: %d\n", aWins + bWins + ties);

	printf("\n");

	printf("cards\twin\twin%%\tlose\tlose%%\ttie\ttie%%\tEV\n");

	std::cout << hands_cards::cardsToString(hands[0], 2) << '\t'; 
	printf("%d\t", aWins);
	printf("%.2f\t", 100.f*aWins / total);
	printf("%d\t", bWins);
	printf("%.2f\t", 100.f*bWins / total);
	printf("%d\t", ties);
	printf("%.2f\t", 100.f * ties / total);
	printf("%.3f\t", leftEV);

	printf("\n");

	std::cout << hands_cards::cardsToString(hands[1], 2) << '\t'; 
	printf("%d\t", bWins);
	printf("%.2f\t", 100.f*bWins / total);
	printf("%d\t", aWins);
	printf("%.2f\t", 100.f*aWins / total);
	printf("%d\t", ties);
	printf("%.2f\t", 100.f * ties / total);	
	printf("%.3f\t", rightEV);

	printf("\n");

	
	}

int main()
	{
	DECK_MASK boardCards[5];
	unsigned int cardInds[4];

	const unsigned int nTrials = 1;
	unsigned int totalTime = 0;

	for (unsigned int t = 0; t < nTrials; t++)
		{
		srand(time(NULL));

		// random starting cards
		my_math::rand_unique<unsigned int>(cardInds, 0, 52, 4);
		
		// choose the cards	: AKs vs. 77
		//cardInds[0] = 0;
		//cardInds[1] = 1;
		//cardInds[2] =  7;
		//cardInds[3] = 20;

		DECK_MASK hands[2] = {hands_cards::hand2ULong(cardInds, 2),hands_cards::hand2ULong(cardInds + 2, 2)};

		DECK_MASK inPlay = hands_cards::hand2ULong(cardInds, 4);

		if (nTrials < 5)
			{
			//std::cout << hands_cards::cardsToString(hands[0],2) << "\t\t\t\t\t\t\t";
			//std::cout << hands_cards::cardsToString(hands[1],2) << std::endl;
			}
		clock_t start = clock();

		DECK_MASK board = enumerate::yieldBoard(boardCards, inPlay, true);

		unsigned int iters = 0;


		unsigned int aWins = 0;
		unsigned int bWins = 0;
		unsigned int ties = 0;

		while (board != 0)
			{
			HAND_STR aStr = odds_byCount::handStr(hands[0] | board);
			HAND_STR bStr = odds_byCount::handStr(hands[1] | board);


			/// with or without branching; doesn't seem to make a difference
			#define BRANCH

			#ifndef BRANCH
			aWins += (aStr > bStr);
			bWins += (bStr > aStr);
			ties += (aStr == bStr);

			#else
			if (aStr > bStr)
				++aWins;
			else if (bStr > aStr)
				++bWins;
			else
				++ties;

			#endif

			//if (rand() % 85000 == 0)  // displays info on approx. 20 random boards
			//	{

			//	std::cout << hands_cards::type2str[GET_TYPE(left)] << "\t";
			//	if (left > right)
			//		std::cout << "WIN";
			//	std::cout << "\t";
			//	std::cout << hands_cards::cardsToString(board, 5) << "\t";
			//	if (right > left)
			//		std::cout << "WIN";
			//	std::cout << "\t";
			//	std::cout << hands_cards::type2str[GET_TYPE(right)]  << std::endl;
			//	}
			board = enumerate::yieldBoard(boardCards, inPlay, false);
			iters++;
			}

 
		clock_t diff = clock() - start;

		int elapsed = diff * 1000 / CLOCKS_PER_SEC;
		printf("%.2f seconds\n", (float)elapsed / 1000);
		totalTime += elapsed;

		if (nTrials < 10)
			printOutcomes(hands, aWins, bWins, ties, start);
		}
	if (nTrials > 5)
		printf("Average time: %.4f", (float)totalTime / 1000 / nTrials);
	pauseExit();
	}