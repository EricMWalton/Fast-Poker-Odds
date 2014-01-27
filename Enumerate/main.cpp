
// TESTS THE FUNCTIONS IN ENUMERATE.H
// the yield version is significantly faster, though both take only a small proportion of the overall program
#include "../headers/std_headers.h"







int main()
	{
	DECK_MASK boardCards[5];
	unsigned int cardsInPlay[4];

	// for timing tests
	const int trials = 1;

	if (trials > 1)
		getch();
		// for use with Very Sleepy

	for (int i = 0; i < trials; i++)  
		{
		srand(time(NULL));

		my_math::rand_unique<unsigned int>(cardsInPlay, 0, 52, 4);

		const DECK_MASK inPlay = hands_cards::hand2ULong(cardsInPlay, 4);

		DECK_MASK board = enumerate::initialBoard(boardCards, inPlay);

		if (trials == 1)
			{
			std::cout << "\nNOT USING YIELD" << std::endl;
			std::cout << "cards in play:\t" << std::bitset<52>(inPlay).to_string() << std::endl;
			std::cout << "initial board:\t" << std::bitset<52>(board).to_string() << std::endl;
			}

		// NOT USING YIELD

		unsigned int iters = 0;

		do	{
			// process board
			//		std::cout << "at " << i << ":\t\t" << std::bitset<52>(board).to_string() << std::endl;

			board = enumerate::nextBoard(boardCards, inPlay);
			iters++;
			}
		while (board != 0);

		std::cout << "iters:\t" << iters << std::endl;


		/// USING YIELD /// 

		iters = 0;


		board = enumerate::yieldBoard(boardCards, inPlay, true); 


		if (trials == 1)
			{
			std::cout << "\nUSING YIELD" << std::endl;
			std::cout << "cards in play:\t" << std::bitset<52>(inPlay).to_string() << std::endl;
			std::cout << "initial board:\t" << std::bitset<52>(board).to_string() << std::endl;
			}


		while (board != 0)
			{
			board = enumerate::yieldBoard(boardCards, inPlay, false);
			iters++;
			}
		std::cout << "iters:\t" << iters << std::endl;
		

		}
	pauseExit();
	}					  