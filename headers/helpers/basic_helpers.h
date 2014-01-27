
template<typename T> void swap(T* A, T* B)
	{
	T temp = *A;
	(*A) = *B;
	(*B) = temp;
	}
