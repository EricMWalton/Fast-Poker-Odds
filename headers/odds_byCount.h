// evaluate the strength of a 7-card hand, using the bit-count method


#ifndef BIT_HELPERS
#include "bit_helpers.h"
#endif


#define USE_INLINE 0

#if USE_INLINE
#define INLINE inline
#else
#define INLINE 
#endif

#define formStrength(type, ranksA, ranksB) (((type) << 26) | ((ranksA) << 13) | (ranksB))


namespace odds_byCount {


	RANK_MASK straightRank(unsigned short int);


	// find what flush-type hand this is: 
	// choices = straight flush, flush, or neither
	// if neither, return 0; otherwise, return handStr integer
	HAND_STR flushRank(RANK_MASK * ranks13)
		{

		// unroll or not?  doesnt seem to make much difference

		#define UNROLL

		#ifndef UNROLL
		//unsigned char cb;
		for (unsigned char i = 0; i < 4; i++)
			{
			//cb = countBits13(ranks13[i]); 
			if (countBits13(ranks13[i]) > 4)
				{
				RANK_MASK sr = straightRank(ranks13[i]);
				if (sr == 0)
					// not a straight flush
					{
					unsigned char cb = countBits13(ranks13[i]);
					if (cb == 5)
						return formStrength(ID_FLUSH, ranks13[i], 0);
					else if (cb == 6)
						return formStrength(ID_FLUSH, clearLeastBit13(ranks13[i]), 0);	  // only 5 cards matter for hand ranking, remove 1
					else
						return formStrength(ID_FLUSH, clearLeast2Bits13(ranks13[i]), 0); // same reason, remove 2
					}
				else
					// straight flush
					{
					return formStrength(ID_STRAIGHTFLUSH, sr, 0);
					}
				}
			}
		#else

		if (countBits13(ranks13[0]) > 4)
			{
			RANK_MASK sr = straightRank(ranks13[0]);
			if (sr == 0)
				// not a straight flush
				{
				unsigned char cb = countBits13(ranks13[0]);
				if (cb == 5)
					return formStrength(ID_FLUSH, ranks13[0], 0);
				else if (cb == 6)
					return formStrength(ID_FLUSH, clearLeastBit13(ranks13[0]), 0);	  // only 5 cards matter for hand ranking, remove 1
				else
					return formStrength(ID_FLUSH, clearLeast2Bits13(ranks13[0]), 0); // same reason, remove 2
				}
			else
				// straight flush
				{
				return formStrength(ID_STRAIGHTFLUSH, sr, 0);
				}
			}

		if (countBits13(ranks13[1]) > 4)
			{
			RANK_MASK sr = straightRank(ranks13[1]);
			if (sr == 0)
				// not a straight flush
				{
				unsigned char cb = countBits13(ranks13[1]);
				if (cb == 5)
					return formStrength(ID_FLUSH, ranks13[1], 0);
				else if (cb == 6)
					return formStrength(ID_FLUSH, clearLeastBit13(ranks13[1]), 0);	  // only 5 cards matter for hand ranking, remove 1
				else
					return formStrength(ID_FLUSH, clearLeast2Bits13(ranks13[1]), 0); // same reason, remove 2
				}
			else
				// straight flush
				{
				return formStrength(ID_STRAIGHTFLUSH, sr, 0);
				}
			} 

		if (countBits13(ranks13[2]) > 4)
			{
			RANK_MASK sr = straightRank(ranks13[2]);
			if (sr == 0)
				// not a straight flush
				{
				unsigned char cb = countBits13(ranks13[2]);
				if (cb == 5)
					return formStrength(ID_FLUSH, ranks13[2], 0);
				else if (cb == 6)
					return formStrength(ID_FLUSH, clearLeastBit13(ranks13[2]), 0);	  // only 5 cards matter for hand ranking, remove 1
				else
					return formStrength(ID_FLUSH, clearLeast2Bits13(ranks13[2]), 0); // same reason, remove 2
				}
			else
				// straight flush
				{
				return formStrength(ID_STRAIGHTFLUSH, sr, 0);
				}
			} 

		if (countBits13(ranks13[3]) > 4)
			{
			RANK_MASK sr = straightRank(ranks13[3]);
			if (sr == 0)
				// not a straight flush
				{
				unsigned char cb = countBits13(ranks13[3]);
				if (cb == 5)
					return formStrength(ID_FLUSH, ranks13[3], 0);
				else if (cb == 6)
					return formStrength(ID_FLUSH, clearLeastBit13(ranks13[3]), 0);	  // only 5 cards matter for hand ranking, remove 1
				else
					return formStrength(ID_FLUSH, clearLeast2Bits13(ranks13[3]), 0); // same reason, remove 2
				}
			else
				// straight flush
				{
				return formStrength(ID_STRAIGHTFLUSH, sr, 0);
				}
			} 
		#endif
		return 0;
		}

// used in pairType() only.  throws an error when ORcount and XORcount take values that don't make sense
#define PAIR_ERROR(ORcount, XORcount) {char s[80];sprintf_s(s, 80,"ORcount = %d; XORcount = %d\0",ORcount, XORcount);throw s;} 

	
	// what pair type hand is it?  return the handstrength integer
	// suit13 is the array of 4 13-bit ranks masks, 1 for each suit
	// bigOR is the result of suit13[0] | suit13[1] | suit13[2] | suit13[3]
	INLINE HAND_STR pairType(RANK_MASK * suit13, RANK_MASK bigOR)
		{
		RANK_MASK bigXOR = suit13[0] ^ suit13[1] ^ suit13[2] ^ suit13[3];
		//unsigned short int ORcount = countBits13(bigOR);
		
		switch(countBits13(bigOR))
			{
		case 7:
			return formStrength(ID_HIGHCARD, clearLeast2Bits13(bigOR), 0); // no pair
			break;
		case 6:
			return formStrength(ID_PAIR, bigOR & ~bigXOR, clearLeast2Bits13(bigXOR));	// one pair
			break;
		case 5:
			{
			if (countBits13(bigXOR) == 3)
				return formStrength(ID_2PAIR, bigOR & ~bigXOR, clearLeast2Bits13(bigXOR)); // two pair
			else //if (XORcount == 5)
				{
				RANK_MASK ranksA = ((suit13[0] & suit13[1]) ^ (suit13[2] & suit13[3])) & bigXOR;
				return formStrength(ID_3OAK, ranksA, clearLeast2Bits13(bigOR & ~ranksA)); // 3oak
				}
			//else
			//	PAIR_ERROR(ORcount,XORcount);
			}
			break;
		case 4:
			{
			if (countBits13(bigXOR) == 1)
				{
				RANK_MASK ranksA = clearLeastBit13(bigOR & ~bigXOR); 
				return formStrength(ID_2PAIR, ranksA, clearLeastBit13(bigOR & ~ranksA)); // 2 pair (actually 3 pair)
				}
			else // if (XORcount == 3)
				{
				RANK_MASK bigAND = suit13[0] & suit13[1] & suit13[2] & suit13[3];
				if (bigAND == 0) 
					return formStrength(ID_FULLHOUSE, ((suit13[0] & suit13[1]) ^ (suit13[2] & suit13[3])) & bigXOR, ~bigXOR & bigOR); // full house
				else
					return formStrength(ID_4OAK, bigAND, clearLeast2Bits13(bigOR & ~bigAND));  // 4oak
				}
			//else
			//	PAIR_ERROR(ORcount,XORcount);
			}
			break;
		case 3:
			{
			unsigned short int XORcount = countBits13(bigXOR);
			RANK_MASK bigAND = suit13[0] & suit13[1] & suit13[2] & suit13[3];
			if (bigAND == 0 && XORcount == 1)
				// full house + extra pair
				return formStrength(ID_FULLHOUSE, ((suit13[0] & suit13[1]) ^ (suit13[2] & suit13[3])) & bigXOR, clearLeastBit13(~bigXOR & bigOR)); 
			else if (XORcount == 3)
				// 2 3oak's
				{
				RANK_MASK trips = ((suit13[0] & suit13[1]) ^ (suit13[2] & suit13[3])) & bigXOR;
				RANK_MASK ranksA = clearLeastBit13(trips); 
				return formStrength(ID_FULLHOUSE, ranksA, trips & ~ranksA); 
				}
			else //if (XORcount == 1)
				 // 4 of a kind + pair
				return formStrength(ID_4OAK, bigAND, clearLeastBit13(bigOR & ~bigAND));
			//else
			//	PAIR_ERROR(ORcount,XORcount);
			}
			break;
		case 2:
			{
			RANK_MASK bigAND = suit13[0] & suit13[1] & suit13[2] & suit13[3];
			return formStrength(ID_4OAK, bigAND, bigOR & ~bigAND); // 4 of a kind + 3oak
			}

		default:
			{
			unsigned short int XORcount = countBits13(bigXOR);
			unsigned short int ORcount = countBits13(bigOR);
			PAIR_ERROR(ORcount,XORcount);
			}
			}
		}

	// returns 0 if not a straight
	// otherwise returns ranksA for straight and straight-flush hands: a bitfield with all zeros except a 1 where the high card of the straight is.
	INLINE RANK_MASK straightRank(unsigned short int bigOR)
		{
		bigOR <<= 1;

		if ((bigOR & 0x2000) != 0)
		{	
			// you have an Ace in your hand; add a phantom card to the right of the 13 to help recognize the wheel.
			bigOR |= 0x1;
		}
		HAND_STR test = bigOR & (bigOR << 1) & (bigOR << 2) & (bigOR << 3) & (bigOR << 4);

		test -= (test & (test >> 1)); 	// to clear extra bits from 6- and 7- card straights.  		
		return test >> 1;
		}

	//find hand strength of a 7-card hand, via count bits method
	INLINE HAND_STR handStr(DECK_MASK hand52)
		{
		unsigned short int suit13[4] = 
			{
			GET_SPADES(hand52),
			GET_HEARTS(hand52),
			GET_DIAMONDS(hand52),
			GET_CLUBS(hand52)
			};
		HAND_STR fr = flushRank(suit13);

		if (fr == 0)
			{
			unsigned short int bigOR = suit13[0] | suit13[1] | suit13[2] | suit13[3];
			unsigned short int sr = straightRank(bigOR);
			if (sr == 0)
				return pairType(suit13, bigOR);
			return formStrength(ID_STRAIGHT, sr, 0);
			}
		else 
			return fr;
		}

	}