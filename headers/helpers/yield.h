// via Chris Pergossi at http://www.flipcode.com/archives/Yield_in_C.shtml //



#include <conio.h> 


//
// marks a location in the program for resume
// does not return control, exits function from inside macro
//
// yield( ret )
//    ret : the return value for when yield() exits the function;
//          must match function return type (leave blank for no return type)


#define yield(ret)                            \
    {                                           \
        /* store the resume location */         \
        __asm {                                 \
		mov _myStaticMkr,offset label_##g_yieldLoc   \
        }                                       \
                                                \
        /* return the supplied value */         \
        return ret;                             \
		g_yieldLoc++;							    \
    }                                           \
    /* our offset in the function */            \
	label_##g_yieldLoc:									


//
// resumes function from the stored offset, or
// continues without notice if there's not one
// stored
// if reset is true, resets resume location to here


#define resume(reset)                        \
    /* our stored offset */             \
    static int _myStaticMkr=0;              \
                                        \
    /* test for no offset */            \
    if( _myStaticMkr && !reset)         \
    {                                   \
        /* resume from offset */        \
        __asm                           \
        {                               \
            jmp _myStaticMkr            \
        }                               \
    }									\
	else if (reset)						\
		_myStaticMkr = 0;				\



	unsigned int g_yieldLoc = 0;  // to "name" the location of the yield statement in the code


