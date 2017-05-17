Author: Jimmy Wong
Date: May 8, 2017

Story:
User can call a function with some given numeric strings and generate the lottery ticket numbers.

Requirements:
1. the generated numbers must consists of 7 unique numbers
2. the generated numbers must be between 1 and 59
3. the generated numbers must be generated in the same order as shown in the numeric strings (algorithm)
4. every digit must be consumed to generate the numbers

Use Case 1:
Given the positive* input strings, the function will generate the valid lotto numbers and return true.

*Note: Positive input strings means it can be used to generate one or more numbers that meet the requirements

Use Case 2:
Given the negative input strings, the tool will return false.

*Note: Negative input strings means there is no way it can generate the valid lotto numbers that meet the requirements

Challenges:
We need an algorithm to construct the valid numbers.

Logic (translated from Requirements):
1. scan the numeric string in order
2. construct the numbers (either single or double digits) one by one
3. able to detect duplication
4. trace back and regenerate previous number in different value (as in 2) if it cannot generate next number to satisfy the conditions

(extra check):
- filter out those strings mathematically cannot yeild any result (for example, 100-digits numeric strings) to skip the processing the numeric string

Design:
1. Data Structure - assists us throughout the algorithm to generate the lottery numbers

The construction of the valid numbers from the numeric string are basically an exploration of finding the solution from a set of possible combinations.  Tree-based data structure will be a good choice.

The parent node of the tree is basically pointing to all possible choice for the first number. There are always only two possible choice you can construct the next number - either a single digit or double digits. Given this, we could use a binary tree.

Once we got the first number, we will construct the second number from the remaining of the numeric string.  So each level is a number generated for the lottery ticket.  We should have a total of 7 level excluding the root node of the tree.  How the number is being constructed and how to identify we have successfully generated the numbers, please read Algorithm below.

There is also another data structure - hash table also mentioned in Algorithm below.

2. Algorithm - logic and efficiency:

The algorithm is built based upon on the data structure we selected which is a binary tree. For the each number it has to follow the rule of requirement #2 at least.  There is the requirement #1 for the uniqueness of the generated numbers, we need to keep track all the number we have generated. To easily check for data collision (duplication), we could use hash table to store the generated numbers (key is the generated number, value is a boolean and true means the number has been generated). The numbers in the hash table will be removed as we trace back the tree.

As we travel down the tree, we will check to see if we have consumed the whole string once we got 7 numbers for requirement #4.  If not, we will trace back and continue to explore other combination until we exhaust all possiblities in which no valid numbers can be found. If we have found 7 numbers and meet requirement #4, then we have found a valid set of numbers.

The algorithm mentioned above has already covered requirement #3.

The efficiency for travelling the tree to reach the solution:
For best case is O(n) since you could have a string of 1234567 and generates the string the first attempt
For the worst case is O(2^n) because each level will multiple the number of different combinations by two
Note: The efficiency for accessing the hash table is O(1).

3. Design Pattern - Aim for interchangable algorithm and loosely coupled

Strategy design pattern makes it easy to switch the caller to a different implementation of the algorithm and on unit testing (both are not provided/shown in the source code)

Programming language used: C++
