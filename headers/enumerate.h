
// ENUMERATES ALL POSSIBLE BOARDS, GIVEN A SET OF CARDS ALREADY IN PLAY
// the version using yield() is significantly faster

#include "../headers/helpers/yield.h"

#ifndef HANDS_CARDS
#include "hands_cards.h"
#endif


namespace enumerate
	{


	// uses yield() very much like Python; 
	// reset = true if it's a first board (resets an internal variable in yield.h)
	// cards[] = the array of all 5 individual cards in the previous board, each represented with a 52-bit array, 1 bit set
	// inPlay = 52-bit bitmask of all cards in player's hands or otherwise out of the deck
	// return 0 when there are no more boards
	// within each for statement, tests if the card is passed its scope; i.e., the first card can't be the 4th to last card in the deck
	// the if statements test if the card is already in play
	DECK_MASK yieldBoard(DECK_MASK * cards, const DECK_MASK inPlay, bool initial)
		{
		resume(initial);

		for (cards[0] = FIRST_CARD; cards[0] != 0x8; cards[0] >>= 1)
			{
			if ((cards[0] & inPlay) == 0)
				{
				for (cards[1] = cards[0] >> 1; cards[1] != 0x4; cards[1] >>= 1)
					{
					if ((cards[1] & inPlay) == 0)
						{
						for (cards[2] = cards[1] >> 1; cards[2] != 0x2; cards[2] >>= 1)
							{
							if ((cards[2] & inPlay) == 0)
								{
								for (cards[3] = cards[2] >> 1; cards[3] != 0x1; cards[3] >>= 1)
									{
									if ((cards[3] & inPlay) == 0)
										{
										for (cards[4] = cards[3] >> 1; cards[4] != 0x0; cards[4] >>= 1)
											{
											if ((cards[4] & inPlay) == 0)
												{
												yield(cards[0] | cards[1] | cards[2] | cards[3] | cards[4]);
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		return 0;
		}


	// cards[] is the array of all 5 individual cards in the previous board, each represented with a 52-bit array, 1 bit set
	// inPlay = 52-bit bitmask of all cards in player's hands or otherwise out of the deck
	// return 0 if there are no more boards
	DECK_MASK nextBoard(DECK_MASK * cards, const DECK_MASK inPlay)
		{

		cards[4] >>= 1;  // advance the 5th card

		while (cards[0] != 0x8) // while the 1st card isn't the 4th to last bit
			{
			if ((cards[0] & inPlay) == 0) // ..and it's not in play
				{
				while (cards[1] != 0x4) // while the 2nd card isn't the 3rd to last bit
					{
					if ((cards[1] & inPlay) == 0)  // ..and it's not in play
						{
						while (cards[2] != 0x2) // while the 3rd card isn't the 2nd to last bit
							{
							if ((cards[2] & inPlay) == 0) // ..and it's not in play
								{
								while (cards[3] != 0x1) // while the 4th card  isn't the last bit
									{
									if ((cards[3] & inPlay) == 0)	 // and it's not in play
										{
										while (cards[4] != 0x0) // while the 5th card isn't off the edge of the bitfield
											{
											if ((cards[4] & inPlay) == 0)	// .. and it's not in play
												return cards[0] | cards[1] | cards[2] | cards[3] | cards[4];
											cards[4] >>= 1;
											}
										}
									cards[3] >>= 1;
									cards[4] = cards[3] >> 1;
									}
								}
							cards[2] >>= 1;
							cards[3] = cards[2] >> 1;
							cards[4] = cards[3] >> 1;
							}
						}
					cards[1] >>= 1;
					cards[2] = cards[1] >> 1;
					cards[3] = cards[2] >> 1;
					cards[4] = cards[3] >> 1;
					}
				}
			cards[0] >>= 1;
			cards[1] = cards[0] >> 1;
			cards[2] = cards[1] >> 1;
			cards[3] = cards[2] >> 1;
			cards[4] = cards[3] >> 1;
			}
		return 0;
		}

	// initialize the first board.
	// uses nextBoard to ensure the first board doesn't collide with any of the cards in play
	// don't use with yieldBoard()
	DECK_MASK initialBoard(DECK_MASK * boardCards, DECK_MASK inPlay)
		{
		for (int i = 0; i < 4; i++)
			boardCards[i] = FIRST_CARD >> i;
		boardCards[4] = boardCards[3]; // because nextBoard will advance the 5th card
		return nextBoard(boardCards, inPlay);
		//return yieldBoard(boardCards, inPlay, true);
		}

	}