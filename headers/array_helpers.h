
#define ARRAY_HELPERS 1

namespace array_helpers {
	template<typename T>T sum(T* arr, size_t numElems)
		{
		T total = 0;
		for (unsigned int i = 0; i < numElems; i++)
			total += arr[i];
		return total;
		}

	template<typename T>void print(T* arr, size_t numElems)
		{
		std::cout << std::endl << '[' << arr[0];

		for (unsigned int i = 1; i < numElems; i++)
			{
			std::cout << ", " << arr[i];
			}
		std::cout << ']' << std::endl;
		}

	// return index if found
	// -1 if no match
	template<typename T> int search(T item, T* start, T* stop)
		{
		for (T* ptr = start; ptr != stop; ptr++)
			{
			if ((*ptr) == item)
				{
				return ptr - start; 

				}
			}
		return -1;
		}

	// returns true if every item in the array is equal
	template<typename T> bool allEqual(T * arr, size_t numElems)
		{
		for (unsigned int i = 1; i < numElems; i++)
			{
			if (arr[i] != arr[i - 1])
				return false;
			}
		return true;
		}

	// add corresponding elements (like vector addition)
	template<typename T> void plus(T * dest, T * A, T * B, size_t numElems)
		{
		for (unsigned int i = 0; i < numElems; i++)
			{
			dest[i] = A[i] + B[i];
			}
		}

	// as above; checkNeg is true if for all i, we expect A[i] >= B[i]
	template<typename T> void minus(T* dest, T* A, T* B, size_t numElems, bool checkNeg = false)
		{
		for (unsigned int i = 0; i < numElems; i++)
			{
			if (checkNeg)
				assert(A[i] >= B[i]);
			dest[i] = A[i] - B[i];
			}
		}
	template<typename T> bool areAllEqual(T* arr, size_t numElems)
		{
		const T firstVal = arr[0];
		for (unsigned int i = 1; i < numElems; i++)
			{
			if (arr[i] != firstVal)
				return false;

			}

		return true;
		}

	
	}