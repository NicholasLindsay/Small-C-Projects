# FourBanger

FourBanger is a parser for a very simple language. The aim of the project is to get familiar with formal grammers and formal languges. FourBanger simply reads a string and decides whether or not the string is a valid sentance in the language. This project is not throughly tested or rigoriusly designed -in fact the code is often inconsistent and ugly, and so there will be ways to break it.

## Formal Language

S -> P -> ()

## Recursive Descent Parsing

The aim is to check whether a given input string is valid in the language. This is down through parsing. The idea in a recursive descent parser is that every non-terminal symbol gets its own function. Inside each function, every production rule for the current non-terminal is checked until a valid production is found or the end of the function is reached, in which case it returns a failure code. This type of parsing can be inefficient as it may have to try several derivation trees until a valid production is encountered.

# Formal Language

The formal language for FourBanger is of the following EBNF form:

start = term;
term = factor {["+"|"-"] factor};
factor = subfactor {["*"|"/"] subfactor};
subfactor = "(" term ")"
			| {"0"|"1"|"2"|"3"|"4"|"5"|"6"|"7"|"8"|"9"};

Here the following conventions are used:
{...} - one or more repetitions
[...] - optional
a|b	  -	choice
"t"	  - terminal
