

#include "../headers/lookup/l_selectLeastBit.h"

#define BIT_HELPERS 1


#define countBits13(arg) (bit_helpers::t_CountBits13[arg])



#define clearLeastBit13(val) (val ^ l_selectLeastBit[val])
//#define clearLeastBit13(val) l_clearLeastBit[val]


#define clearLeast2Bits13(val) clearLeastBit13(clearLeastBit13(val))


namespace bit_helpers	{
	 
	// assuming only the rightmost 13 bits are set, t_CountBits13[i] gives the number of bits in integer i
	static const unsigned char t_CountBits13[8192] = {
		#define B2(n) n,     n+1,     n+1,     n+2
		#define B4(n) B2(n), B2(n+1), B2(n+1), B2(n+2)
		#define B6(n) B4(n), B4(n+1), B4(n+1), B4(n+2)
		#define B8(n) B6(n), B6(n+1), B6(n+1), B6(n+2)
		#define B10(n) B8(n), B8(n+1), B8(n+1), B8(n+2)
		#define B12(n) B10(n), B10(n+1), B10(n+1), B10(n+2)
		B12(0), B12(1)
		};

	//short unsigned int countBits14(unsigned long arg)
	//	{
	//	assert(arg < 16384); 
	//	return t_CountBits14[arg >> 7] + t_CountBits14[arg & 0x7f];
	//	}

	short unsigned int countBitsINF(unsigned long long int arg)
		{
		if (arg >= 8192)
			return countBits13(arg & 0x1fff) + countBitsINF(arg >> 13);
		else
			return countBits13(arg);
		}
	
	// t_SelectLeastBit[i] gives the argument needed to clear the last bit of i
	// the result is all 0s except a 1 at the position of the least bit
	// usage:  i ^ t_SelectLeastBit[i], or you can use i & ~t_SelectLeastBit[i] to actually clear the bit
	// 1st entry, all 0s, is the flag that no bit was found
	//static const short unsigned int l_SelectLeastBit7[128] = { 
	//	0,1,2,1,4,1,2,1,8,1,2,1,4,1,2,1,
	//	16,1,2,1,4,1,2,1,8,1,2,1,4,1,2,1,
	//	32,1,2,1,4,1,2,1,8,1,2,1,4,1,2,1,
	//	16,1,2,1,4,1,2,1,8,1,2,1,4,1,2,1,
	//	64,1,2,1,4,1,2,1,8,1,2,1,4,1,2,1,
	//	16,1,2,1,4,1,2,1,8,1,2,1,4,1,2,1,
	//	32,1,2,1,4,1,2,1,8,1,2,1,4,1,2,1,
	//	16,1,2,1,4,1,2,1,8,1,2,1,4,1,2,1
	//	};
	 
	 //// given a 14-bit integer, return a copy with the rightmost 1-bit set to 0
	 //short unsigned int clearLeastBit14(short unsigned int val)
		// {
		// // check the rightmost 7 bits first
		// short unsigned int l = t_SelectLeastBit7[val & 0x7f]; 
		// if (l != 0)
		//	 return val ^ l;
		// else
		//	 // no bit found on the right
		//	 return val ^ (t_SelectLeastBit7[val >> 7] << 7);
		// }



	 // given a 14-bit integer, return a copy with the rightmost 1-bit set to 0
	 //short unsigned int clearLeastBit14(short unsigned int val)
		// {
		// // check the rightmost 7 bits first
		// return val ^ l_selectLeastBit[val];
		// //short unsigned int l = bit_helpers::t_SelectLeastBit7[val & 0x7f]; 
		// //if (l != 0)
		//	// return val ^ l;
		// //else
		//	// // no bit found on the right
		//	// return val ^ (bit_helpers::t_SelectLeastBit7[val >> 7] << 7);
		// }


	//short unsigned int clearLeast2Bits14(short unsigned int val)
	//	{
	//	return clearLeastBit14(clearLeastBit14(val));
	//	}
	









		// };

	// return a copy of the input, with the rightmost bit cleared
	//short unsigned int clearLSB(short unsigned int val, unsigned int k)
	//	{

	//	}



	}

