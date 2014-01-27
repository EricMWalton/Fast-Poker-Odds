

// check that the number of set bits in ranksA and ranksB makes sense with the hand type
bool checkBitsInRanks(unsigned int hs)
	{
 	short unsigned int type = GET_TYPE(hs);
	short unsigned int numRanks = countBits13(GET_RANKS_A(hs)) + countBits13(GET_RANKS_B(hs));
	
	switch(type)
		{
	case 0:
		return(numRanks == 5);
		break;
	case 1:
		return(numRanks == 4);
		break;
	case 3:
		return(numRanks == 3);
		break;
	case 2:
		return(numRanks == 3);
		break;
	case 4:
		return(numRanks == 1);
		break;
	case 5:
		return(numRanks == 5);
		break;
	case 6:
		return(numRanks == 2);
		break;
	case 8: 
		return(numRanks == 1);
		break;
	case 7:
		return(numRanks <= 4);
		break;
	default:
		return false;
		}
	}

// test the hand strength of testHand, with expectedType as given
// expectedType is a lower bound on the type we expect to see...at least two pair, for instance.
unsigned int test(DECK_MASK hand52, unsigned int expectedType = 0)
	{

	std::cout << hands_cards::cardsToString(hand52) << '\t';
	unsigned int hs;

	try
		{
		hs = odds_byCount::handStr(hand52);
		}

	catch (const char *e)
		{
		std::cerr << "Exception: " << e << std::endl;
		pauseExit(1);
		}
	std::cout << hands_cards::str2String(hs) << std::endl;

	assert(GET_TYPE(hs) >= expectedType);
	assert(checkBitsInRanks(hs));
	assert(hs < (0x1 << 30)); // hand strength is suppose to use only the 30 rightmost bits

	return hs;
	}

// test the hand strength of testHand, with expectedType as given
unsigned int test(unsigned int * testInds, unsigned int expectedType = 0)
	{
	return test(hands_cards::hand2ULong(testInds), expectedType);
	}