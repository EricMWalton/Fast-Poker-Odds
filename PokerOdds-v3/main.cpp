
// for testing the functions in odds_byCount.h 
// evalutes hand strength of a 7-card hand
// main() is a full testing suite, testing every hand type multiple times, as well as random hands
// also fills various lookup arrays

#include "../headers/std_headers.h"


#include <time.h> // TIME()
#include "../headers/lookup/l_permsLookup.h"

#include "tests.h"






// used in systematic testing
// testHand is 7 items
// given a hand of 7 - n cards, add n random unique kickers
// each card is represented as an index 0 - 51
void addKickers(unsigned int * testHand, unsigned int n)
	{
	for (int j = 7 - n; j < 7; j++)
		{
		unsigned int r;
		do  {
			r = rand() % 52;
			} while(array_helpers::search<unsigned int>(r, testHand, testHand + j) != -1);
		testHand[j] = r;
		}
	}


// counting from the left, find the r-th set bit; r is 1 - 16
// return value is the 1-based index of it, or 16 if there aren't that many 1s
unsigned int naiveRank(short unsigned int val, unsigned int r)
	{
	
//	std::cout << sizeof(short unsigned int);  // 2 bytes, 16 bits
	for (int i = 1; i < 16; i++)
		{
		unsigned int x = val & (0x1 << (16 - i));
		if ((val & (0x1 << (16 - i))) != 0)
			r--;
		if (r == 0)
			return i;
		}
	return 16;
	}

// first this calculates the number of bits
// then finds the bit with rank rank (bits are ranked from left, returns a 1-indexed rank from left)
// if we called this with rank 3 and val = 01100100 it would return 6
// note the position starts with the last 16 bits of val...this is 16-bit code. add lines from web for larger integers
unsigned int bithackRank(const short unsigned int val, const unsigned int rank, bool printCount)

	{
	assert((val & 0xffff) == val); // only want val to have 16 bits.  
	unsigned int s;      // Output: Resulting position of bit with rank r [1-64]
	unsigned long a, b, c; // Intermediate temporaries for bit count.
	unsigned int t;      // Bit count temporary.
	unsigned int noul = 0xffff;
	unsigned int v = val;
	unsigned int r = rank;
	

	a =  v - ((v >> 1) & noul/3);
	b = (a & noul/5) + ((a >> 2) & noul/5);
	c = (b + (b >> 4)) & noul/0x11;
	t = (c + (c >> 8)) & noul/0x101;

	if (printCount)
		std::cout << t << '\t'; //print the count

	// Now do branchless select!    

	s = 16;

	t = (c >> (s - 8)) & 0xf;

	if (r > t)
		{
		s -= 8;
		r -= t;

		}

	t = (b >> (s - 4)) & 0x7;	

	if (r > t) 
		{
		s -= 4; 
		r -= t;
		}

	t = (a >> (s - 2)) & 0x3;
	if (r > t) 
		{
		s -= 2; 
		r -= t;
		}

	t = (v >> (s - 1)) & 0x1;
	if (r > t) 
		s--;
	s = 17 - s;

	assert(s == naiveRank(val, rank));
	return s;
	}

// outputs to console the text for the lookup table that clears the last bit
// via http://graphics.stanford.edu/~seander/bithacks.html#ParityNaive
void clearLeastBit_lookup()
	{
	std::cout << "binary\t\t" << "count\t" << "rank\t" << "index" << std::endl;

	// check 10 random integers
	for (int i = 1; i < 11; i++)
		{
		unsigned int testVal = rand() % 256;
		std::cout << std::bitset<8>((int)testVal).to_string() << '\t';
		unsigned int r = (i - 1) / 2 + 1;
		std::cout << r << "\t" << bithackRank(testVal,r, true) << std::endl;
		}

	std::cout << 0; // this is the flag that you didn't find a bit here, so need to look to the right
	for (short unsigned int i = 1; i < 8192; i++)
		{
		short unsigned int numBits = countBits13(i);
		short unsigned int lastBitInd = 16 - bithackRank(i, numBits, false); // 0-based counting from right 
		short unsigned int orArg = 0x1 << lastBitInd;	// all 0s except a 1 at lastBitInd
		short unsigned int afterLastBitCleared = i & ~(0x1 << lastBitInd);
		std::cout << "," << afterLastBitCleared;
		}

	}


// for use with below
// for error checking, display the card string
void hml_readCard(DECK_MASK cards, bool comma = true)
	{
	//std::cout << hands_cards::cardsToString(cards, 2);
	if (comma)
		std::cout << ",";
	std::cout << cards;
	}

// outputs to console the lookup table that gives the handmasks for each hand class
// eg: hand class = AKs, there would be four handmasks, AsKs, AdKd, AhKh, and AcKc
void handMasks_lookup()
	{
	DECK_MASK cards;

	// pairs
	for (unsigned int i = 0; i < 13; i++)
		{
		std::cout << "{";
		cards = (FIRST_CARD >> i) | (FIRST_CARD >> (i + 13));
		hml_readCard(cards, false);

		cards = (FIRST_CARD >> i) | (FIRST_CARD >> (i + 26));
		hml_readCard(cards);

		cards = (FIRST_CARD >> i) | (FIRST_CARD >> (i + 39));
		hml_readCard(cards);

		cards = (FIRST_CARD >> (i + 13)) | (FIRST_CARD >> (i + 26));
		hml_readCard(cards);

		cards = (FIRST_CARD >> (i + 13)) | (FIRST_CARD >> (i + 39));
		hml_readCard(cards);

		cards = (FIRST_CARD >> (i + 26)) | (FIRST_CARD >> (i + 39));
		hml_readCard(cards);
		std::cout << "},";
		}


	for (unsigned int r1 = 0; r1 < 12; r1++)
		{
		// suited
		for (unsigned int r2 = r1 + 1; r2 < 13; r2++)
			{
			std::cout << "{";
			cards = (FIRST_CARD >> r1) | (FIRST_CARD >> r2);
			hml_readCard(cards, false);
			cards >>= 13;
			hml_readCard(cards);
			cards >>= 13;
			hml_readCard(cards);
			cards >>= 13;
			hml_readCard(cards);
			std::cout << "},";
			}

		// offsuit
		for (unsigned int r2 = r1 + 1; r2 < 13; r2++)
			{
			std::cout << "{";

			// first card is a "lower" suit than 2nd
			cards = (FIRST_CARD >> r1) | (FIRST_CARD >> (r2 + 13));
			hml_readCard(cards, false);

			cards >>= 13;
			hml_readCard(cards);

			cards >>= 13;
			hml_readCard(cards);

			cards = (FIRST_CARD >> r1) | (FIRST_CARD >> (r2 + 26));
			hml_readCard(cards);

			cards >>= 13;
			hml_readCard(cards);

			cards = (FIRST_CARD >> r1) | (FIRST_CARD >> (r2 + 39));
			hml_readCard(cards);

			// 2nd card is a "lower" suit than 1st

			cards = (FIRST_CARD >> r2) | (FIRST_CARD >> (r1 + 13));
			hml_readCard(cards);

			cards >>= 13;
			hml_readCard(cards);

			cards >>= 13;
			hml_readCard(cards);

			cards = (FIRST_CARD >> r2) | (FIRST_CARD >> (r1 + 26));
			hml_readCard(cards);

			cards >>= 13;
			hml_readCard(cards);


			cards = (FIRST_CARD >> r2) | (FIRST_CARD >> (r1 + 39));
			hml_readCard(cards);

			std::cout << "},";

			}
		}
	}

void handPerms_lookup()
	{
	std::cout << "6";

	// the pairs
	for (unsigned int i = 1; i < 13; i++)
		std::cout << ",6";

	for (unsigned int r1 = 14; r1 >= 3; r1--)
		{
		for (unsigned int r2 = r1 - 1; r2 >= 2; r2--)
			{
			std::cout << ",4";
			}
		for (unsigned int r2 = r1 - 1; r2 >= 2; r2--)
			{
			std::cout << ",12";
			}
		}
	}






int main()
	{
	srand(time(NULL));

	const unsigned int testN = 10;

	unsigned int testInds[7];
	unsigned int keyRanks[2];
	unsigned int keySuits[4];

	//std::cout << "**** PROBLEM  ****" << std::endl << std::endl;

	//std::string testStrs[7] = {"Jd", "Td", "6d", "Ac", "Kc", "Qc", "5c"};

	//for (unsigned int i = 0; i < 7; i++)
	//	{
	//	testInds[i] = hands_cards::str2cardInd(testStrs[i]);
	//	}
	//test(testInds, ID_STRAIGHT);
	//std::cout << std::endl;

	std::cout << "**** Straight flush - no wheels ****" << std::endl << std::endl;

	for (int i = 0; i < testN; i++)
		{
		int highRank = RAND_RANK(ACE,SIX);
		int suit = rand() % 4;

		unsigned int testInds[7];

		for (int j = 0; j < 5; j++)
			{
			testInds[j] = CARD_INDEX(highRank + j, suit);
			}
		addKickers(testInds, 2);
		test(hands_cards::hand2ULong(testInds),ID_STRAIGHTFLUSH);
		}
	std::cout << std::endl;

	std::cout << "**** Straight flush - wheel ****" << std::endl << std::endl;

	for (int i = 0; i < testN; i++)
		{
		int suit = RAND_SUIT;
		unsigned int testInds[7];

		testInds[0] = CARD_INDEX(ACE, suit); // the ace

		int iInds = 1;

		for (int r = FIVE; r != nRANKS; DECR_RANK(r))
			{
			testInds[iInds] = CARD_INDEX(r, suit);
			iInds++;
			}

		addKickers(testInds, 2);
		test(hands_cards::hand2ULong(testInds),ID_STRAIGHTFLUSH);
		}
	std::cout << std::endl;

	
	std::cout << "**** Four of a kind ****" << std::endl << std::endl;

	for (int i = 0; i < testN; i++)
		{
		int rank = RAND_RANK(ACE,TWO);
		unsigned int testInds[7];

		for (int s = 0; s < 4; s++)
			testInds[s] = CARD_INDEX(rank, s);
		addKickers(testInds, 3);
		test(hands_cards::hand2ULong(testInds),ID_4OAK);
		}
	std::cout << std::endl;


	std::cout << "**** Full house ****" << std::endl << std::endl;

	for (int i = 0; i < testN; i++)
		{
		unsigned iInds = 0;
		
		my_math::rand_unique<unsigned int>(keyRanks, 0, 13, 2);

		my_math::rand_unique<unsigned int>(keySuits, 0, 4, 3);

		for (int sIndex = 0; sIndex < 3; sIndex++)
			{
			testInds[iInds] = CARD_INDEX(keyRanks[0], keySuits[sIndex]);
			iInds++;
			}

		my_math::rand_unique<unsigned int>(keySuits, 0, 4, 2);

		for (int sIndex = 0; sIndex < 2; sIndex++)
			{
			testInds[iInds] = CARD_INDEX(keyRanks[1], keySuits[sIndex]);
			iInds++;
			}

		addKickers(testInds, 2);
		test(testInds,ID_FULLHOUSE);
		}
	std::cout << std::endl;


	std::cout << "**** FLUSHES ****" << std::endl << std::endl;

	for (int i = 0; i < testN; i++)
		{
		int suit = RAND_SUIT; // which suit?
	
		unsigned int testInds[7];
		
		my_math::rand_unique<unsigned int>(testInds, 0, 13, 5); // which ranks?

		for (int j = 0; j < 5; j++)
			testInds[j] = CARD_INDEX(testInds[j], suit);

		addKickers(testInds, 2);

		test(testInds, ID_FLUSH);
		}
	std::cout << std::endl;

	std::cout << "**** Straight - no wheels ****" << std::endl << std::endl;

	for (int i = 0; i < testN; i++)
		{
		int highRank = RAND_RANK(ACE, SIX);

		unsigned int testInds[7];
		unsigned int iInds = 0;

		for (int r = highRank; iInds < 5; DECR_RANK(r))
			{
			testInds[iInds] = CARD_INDEX(r, RAND_SUIT);
			iInds++;
			}
		addKickers(testInds, 2);
		test(testInds, ID_STRAIGHT);

		}
	std::cout << std::endl;

	std::cout << "**** Straight - wheel ****" << std::endl << std::endl;

	for (int i = 0; i < testN; i++)
		{
		testInds[0] = CARD_INDEX(ACE, RAND_SUIT); // the ace

		unsigned int iInds = 1;

		for (int r = FIVE; r != nRANKS; DECR_RANK(r))
			{
			testInds[iInds] = CARD_INDEX(r, RAND_SUIT);
			iInds++;
			}
		addKickers(testInds, 2);
		test(testInds,ID_STRAIGHT);
		}
	std::cout << std::endl;

	std::cout << "**** 3 of a kind ****" << std::endl << std::endl;

	for (int i = 0; i < testN; i++)
		{
		unsigned int rank = RAND_RANK(ACE,TWO);
		
		for (int s = 0; s < 3; s++)
			testInds[s] = CARD_INDEX(rank, s);
		addKickers(testInds, 4);
		test(testInds,ID_3OAK);
		}
	std::cout << std::endl;



	std::cout << "**** Two pair ****" << std::endl << std::endl;

	for (int i = 0; i < testN; i++)
		{
		my_math::rand_unique<unsigned int>(keyRanks, 0, 13, 2);
		my_math::rand_unique<unsigned int>(keySuits, 0, 4, 2);

		for (int j = 0; j < 2; j++)
			testInds[j] = CARD_INDEX(keyRanks[0], keySuits[j]);

		my_math::rand_unique<unsigned int>(keySuits + 2, 0, 4, 2);

		for (int j = 2; j < 4; j++)
			testInds[j] = CARD_INDEX(keyRanks[1], keySuits[j]);

		addKickers(testInds, 3);
		test(testInds, ID_2PAIR);
		}
	std::cout << std::endl;

	std::cout << "**** Two pair - (actually 3 pair) ****" << std::endl << std::endl;

	for (int i = 0; i < testN; i++)
		{
		unsigned iInds = 0;
		
		my_math::rand_unique<unsigned int>(keyRanks, 0, 13, 3);

		for (int j = 0; j < 3; j++)
			{
			my_math::rand_unique<unsigned int>(keySuits, 0, 4, 2);
			for (int k = 0; k < 2; k++)
				{
				testInds[iInds] = CARD_INDEX(keyRanks[j], keySuits[k]);
				iInds++;
				}
			}

		addKickers(testInds, 1);
		test(testInds,ID_2PAIR);
		}
	std::cout << std::endl;	

	std::cout << "**** One pair ****" << std::endl << std::endl;

	for (int i = 0; i < testN; i++)
		{
		unsigned int rank = RAND_RANK(ACE, TWO);
		
		for (int s = 0; s < 2; s++)
			testInds[s] = CARD_INDEX(rank, s);
		addKickers(testInds, 5);
		test(testInds, ID_PAIR);
		}
	std::cout << std::endl;


	std::cout << "**** Random hand *****" << std::endl << std::endl;

	for (int i = 0; i < testN; i++)
		{
		my_math::rand_unique<unsigned int>(testInds, 0, 52, 7);
		test(testInds);
		}
	std::cout << std::endl;

	pauseExit();
	}