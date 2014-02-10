Fast-Poker-Odds
===============

Texas Hold'Em odds calculator.  It evaluates roughly 23 million hands per second on my 2.67 Ghz laptop.  This is right in the ballpark of the fastest versions I've heard about.  

Executables
-----------

Each of these executables tests (and demonstrates) a different aspect of the program.

**HandStrength/main.cpp**: Tests the fcns that evaluate the hand strength of a seven-card hand (straight, pair, etc.)  

**Enumerate/main.cpp**: Enumerates all possible boards, given the cards that are in play.  

**Evalute/main.cpp**: Given two starting hands, computes win probability by enumerating all possible boards.

**HandClass/main.cpp**: Given two starting hand classes (i.e. Ace-King suited), evalutes win probabilities by tallying results from all suit combinations and all boards.

Further Development
-------------------

Very easily, this program could be extended for more than two players, for starting from any stage of the hand, and for any possible "dead" cards.  

There is a huge area for improvement in speed.  As of now, each set of 7 card ranks is checked 16,384 times for pairs --- one for each suit permutation.  By iterating through the boards by rank and then by suit, rather than the reverse, it is possible to avoid that redundancy.


Acknowledgements
----------------

I owe a debt to Keith Rule's version at CodeProject: http://www.codeproject.com/Articles/12279/Fast-Texas-Holdem-Hand-Evaluation-and-Analysis.  The algorithm is my own, but a few of his ideas (counting bits, the hand representation as a 52-bit number) helped me get off the ground.

I used Chris Pergrossi's implementation of a yield macro.  Borrowed with thanks from http://www.flipcode.com/archives/Yield_in_C.shtml.

I used Hallvard Furuseth's macro-expanded lookup table for counting bits.  Via http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetTable.

From the same website, I used Juha Järvi's method for finding the index of the least bit.  Used in the function that computes (and outputs) the lookup table for selectLeastBit.




