// tests evaluation of hand class vs. hand class, by iterating through possible suits
// todo: externalize parts of this to handClassOdds.h

#include "../headers/std_headers.h"

#include "../headers/my_string.h"
#include "../headers/basic_helpers.h"
#include "../headers/string_number.h"
#include "../headers/lookup/l_handMasks.h"
#include "../headers/lookup/l_handPerms.h"

#include <time.h> // TIME(), timing tests, random values


// handInds[] is the number 0-169 of each hand class
void printOutcomes(unsigned int * handInds, unsigned int aWins, unsigned int bWins, unsigned int ties, bool flipped)
	{
	const unsigned int total = aWins + bWins + ties;

	float leftEV =  1.f * aWins / total + 0.5f * ties / total;
	float rightEV = 1.f * bWins / total + 0.5f * ties / total;

	printf("total boards: %d\n", aWins + bWins + ties);

	printf("\n");

	if (flipped)
		{
		swap<unsigned int>(handInds, handInds + 1);

		}

	printf("%8s%12s%8s%12s%8s%12s%8s%8s\n", "cards","win", "win%", "lose", "lose%", "tie", "tie%", "EV");

	//std::cout << hands_cards::l_handInd2Str[handInds[0]] << '\t'; 
	printf("%8s", hands_cards::l_handInd2Str[handInds[0]].c_str());
	printf("%12d", aWins);
	printf("%8.4f", 100.f*aWins / total);
	printf("%12d", bWins);
	printf("%8.4f", 100.f*bWins / total);
	printf("%12d", ties);
	printf("%8.4f", 100.f * ties / total);
	printf("%8.5f", leftEV);

	printf("\n");

	//std::cout << hands_cards::l_handInd2Str[handInds[0]] << '\t'; 
	printf("%8s", hands_cards::l_handInd2Str[handInds[1]].c_str());
	printf("%12d", bWins);
	printf("%8.4f", 100.f*bWins / total);
	printf("%12d", aWins);
	printf("%8.4f", 100.f*aWins / total);
	printf("%12d", ties);
	printf("%8.4f", 100.f * ties / total);	
	printf("%8.5f", rightEV);

	printf("\n");

	
	}


int main()
	{
	DECK_MASK boardCards[5];
	DECK_MASK handMasks[2];
	unsigned int handInds[2];

	const unsigned int trials = 1;

	srand(time(NULL));

	for (unsigned int t = 0; t < trials; t++)
		{
		// random starting hands
		handInds[0] = rand() % 169;
		handInds[1] = rand() % 169;
		
		// choose the hands: (AA vs. KK)
		//handInds[0] = 0;
		//handInds[1] = 1;

		const DECK_MASK * movingHand;
		DECK_MASK fixedHand;
		unsigned int totalPerms;
		bool flipped; // were the hands flipped?

		// fix the suits of the hand with more permutations
		if (l_handPerms[handInds[0]] >= l_handPerms[handInds[1]])
			{
			fixedHand = l_handMasks[handInds[0]][0];
			movingHand = l_handMasks[handInds[1]];
		    totalPerms = l_handPerms[handInds[1]];
			flipped = false;
			}
		else
			{
			fixedHand = l_handMasks[handInds[1]][0];
			movingHand = l_handMasks[handInds[0]];
		    totalPerms = l_handPerms[handInds[0]];
			flipped = true;
			}
		

		unsigned int aWins = 0;
		unsigned int bWins = 0;
		unsigned int ties = 0;

		clock_t start = clock();

		//for(int i = 0; i < l_handPerms[handInds[1]]; i++)
		for(int i = 0; i < totalPerms; i++)
			{
			//handMasks[1] = l_handMasks[handInds[1]][i];

			if ((fixedHand & movingHand[i]) != 0)
				// cards in common? impossible suits
				continue;

			DECK_MASK inPlay = fixedHand | movingHand[i];

			if (trials < 5)
				{
				std::cout << hands_cards::cardsToString(fixedHand,2) << "\t\t\t\t\t\t\t";
				std::cout << hands_cards::cardsToString(movingHand[i],2) << std::endl;
				}
			
			DECK_MASK board = enumerate::yieldBoard(boardCards, inPlay, true);

			while (board != 0)
				{
				HAND_STR aStr = odds_byCount::handStr(fixedHand | board);
				HAND_STR bStr = odds_byCount::handStr(movingHand[i] | board);

				if (aStr > bStr)
					++aWins;
				else if (bStr > aStr)
					++bWins;
				else
					++ties;
				board = enumerate::yieldBoard(boardCards, inPlay, false);
				}
			}
		clock_t diff = clock() - start;

		int elapsed = diff * 1000 / CLOCKS_PER_SEC;
		printf("%.2f seconds\n", (float)elapsed / 1000);

		if (trials < 20)
			printOutcomes(handInds, aWins, bWins, ties, flipped);		
		}
	pauseExit();
	}