
// HELPER FCNS, MACROS, AND LOOKUP ARRAYS FOR MANAGING HAND AND CARD DATA
// todo: use a namespace, make API and fcn names clearer
// include a fcn for going hand strength (type only) -> string


#define HANDS_CARDS 1

//#include<bitset>

// ********* HAND STRENGTH  ************* //

	#define GET_TYPE(handStr) handStr >> 26
	#define GET_RANKS_A(handStr) (handStr >> 13) & 0x1fff
	#define GET_RANKS_B(handStr) handStr & 0x1fff

	#define ID_HIGHCARD 0
	#define ID_PAIR 1
	#define ID_2PAIR 2
	#define ID_3OAK 3
	#define ID_STRAIGHT 4
	#define ID_FLUSH 5
	#define ID_FULLHOUSE 6
	#define ID_4OAK 7
	#define ID_STRAIGHTFLUSH 8

// ****** CARDS ************** //

	// indexing in suit-major order
	#define CARD_INDEX(rank,suit) suit*13 + rank

	// all 0s, but 1 at the location of this card
	#define CARD_MASK(index) FIRST_CARD >> index

	// all 0s, but 1 at the location of the first card (in the 52-bit mask)
	#define FIRST_CARD 0x8000000000000


	#define RAND_SUIT rand() % 4

	// inclusive of both lowRank and highRank
	// lowRank must be a higher ranked card than highRank...but with lower index in RANKS enum
	#define RAND_RANK(highRank,lowRank) rand() % (1 + lowRank - highRank) + highRank

	enum e_ranks {ACE, KING, QUEEN, JACK, TEN, NINE, EIGHT, SEVEN, SIX, FIVE, FOUR, THREE, TWO, nRANKS};
	enum e_suits {SPADES, HEARTS, DIAMONDS, CLUBS, nSUITS};

	// higher cards have lower indices, and vice versa
	#define INCR_RANK(r) r--
	#define DECR_RANK(r) r++


	// take a handmask, create a 13-bit field with the ranks of one suit
	#define GET_SPADES(hand) (hand >> 39)
	#define GET_HEARTS(hand) ((hand >> 26) & 0x1fff)
	#define GET_DIAMONDS(hand) ((hand >> 13) & 0x1fff)
	#define GET_CLUBS(hand) (hand & 0x1fff)


// ****** DATA TYPES ************** //

	#define DECK_MASK unsigned long long
	#define RANK_MASK unsigned short	 // just the 13 bits to display ranks (cards of one suit, or the ranks of RANKSA or RANKSB
	#define HAND_STR unsigned int // total of 30 active bits

namespace hands_cards
	{
	std::string type2str[9] = {
		"No pair\t",
		"One pair",
		"Two pair",
		"Three of a kind",
		"Straight",
		"Flush\t",
		"Full house",
		"4 of a kind",
		"Straight flush"
		};

	std::string rank2str[13] = {
		"A",
		"K",
		"Q",
		"J",
		"T",
		"9",
		"8",
		"7",
		"6",
		"5",
		"4",
		"3",
		"2"
		};

	std::string l_suit2str[4] = {
		"s",
		"h",
		"d",
		"c"};

	const std::string l_handInd2Str[169] = {
		"AA","KK","QQ","JJ","TT","99","88","77","66","55","44","33","22",
		"AKs", "AQs","AJs","ATs","A9s","A8s","A7s","A6s","A5s","A4s","A3s","A2s",
		"AKo", "AQo","AJo","ATo","A9o","A8o","A7o","A6o","A5o","A4o","A3o","A2o",
		"KQs","KJs","KTs","K9s","K8s","K7s","K6s","K5s","K4s","K3s","K2s",
		"KQo","KJo","KTo","K9o","K8o","K7o","K6o","K5o","K4o","K3o","K2o",
		"QJs","QTs","Q9s","Q8s","Q7s","Q6s","Q5s","Q4s","Q3s","Q2s",
		"QJo","QTo","Q9o","Q8o","Q7o","Q6o","Q5o","Q4o","Q3o","Q2o",
		"JTs","J9s","J8s","J7s","J6s","J5s","J4s","J3s","J2s",
		"JTo","J9o","J8o","J7o","J6o","J5o","J4o","J3o","J2o",
		"T9s","T8s","T7s","T6s","T5s","T4s","T3s","T2s",
		"T9o","T8o","T7o","T6o","T5o","T4o","T3o","T2o",
		"98s","97s","96s","95s","94s","93s","92s",
		"98o","97o","96o","95o","94o","93o","92o",
		"87s","86s","85s","84s","83s","82s",
		"87o","86o","85o","84o","83o","82o",
		"76s","75s","74s","73s","72s",
		"76o","75o","74o","73o","72o",
		"65s","64s","63s","62s",
		"65o","64o","63o","62o",
		"54s","53s","52s",
		"54o","53o","52o",
		"43s","42s",
		"43o","42o",
		"32s",
		"32o"
		};

	std::string ranks2string(RANK_MASK ranks13)
		{
			 std::string retStr = "";
			 for (int i = 12; i >= 0; i--)
				 {
				 if ((ranks13 & (1 << i)) != 0)
					 retStr += rank2str[12 - i];
				 }
			 return retStr;
		}


	// given a string for a card ("As", for instance)
	// output the index needed by hand2ULong() below
	//[Qh,8h,5h,Kd,8d,5d,8c]
	unsigned int str2cardInd(std::string handStr)
		{
		assert(handStr.length() == 2);
		int rank = array_helpers::search<std::string>(handStr.substr(0, 1), rank2str, rank2str + 13);
		int suit = array_helpers::search<std::string>(handStr.substr(1, 1), l_suit2str, l_suit2str + 4);
		assert(rank >= 0);
		assert(suit >= 0);
		return 13 * suit + rank;

		}

// given indices in suit-major order (As Ks Qs Js ... 2s Ad Kd Qd Jd ... 2d Ah ....)
// return a 52-bit mask of the hand
DECK_MASK hand2ULong(unsigned int * cardInds, size_t nCards = 7)
	{
	DECK_MASK cards52 = 0;

	for (int j = 0; j < nCards; j++)
		cards52 |= CARD_MASK(cardInds[j]);

	return cards52;
	}


	std::string str2String(HAND_STR handStr)
		{
		RANK_MASK type =  GET_TYPE(handStr);
		RANK_MASK ranksA = (handStr >> 13) & 0x1fff;
		RANK_MASK ranksB = handStr & 0x1fff;

		return type2str[type] + "\t" + ranks2string(ranksA) + "\t" + ranks2string(ranksB);
		}


	// print to console, a list of handSize cards
	// given a 52-bit mask of the cards
	std::string cardsToString(DECK_MASK cards52, unsigned int nCards = 7)
		{
		std::string retStr("[");

		unsigned int sIndex = 0;

		for (int i = 0; i < 52; i++)
			{
			if ((cards52 & (FIRST_CARD >> i)) != 0)
				{
				unsigned int suit = i / 13;
				unsigned int rank = i % 13;
				retStr += rank2str[rank] + l_suit2str[suit];
				if (sIndex < nCards - 1)  // not the last card
					retStr += ",";
				else 
					break;
				sIndex++;
				}
			}
		retStr += "]";
		return retStr;
		}

	}