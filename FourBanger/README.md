# FourBanger

FourBanger is a parser for a very simple language. The aim is to get familiar with formal grammers and formal languges. FourBanger simply reads a string and decides whether or not the string is a valid sentance in the language.

## Formal Language

All legal sentances are of the form a<sup>n</sup>b<sup>n</sup>
, where n is an integer >= 1.

The production rules of the grammar are:

- S -> P
- S -> Q
- P -> 0P1
- P -> 01
- Q -> 1Q0
- Q -> 10

For example, the following is the derivation of 111000 for S:
S -> Q -> 1Q0 -> 11Q00 -> 111000

## Recursive Descent Parsing

The aim is to check whether a given input string is valid in the language. This is down through parsing.
