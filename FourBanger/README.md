# FourBanger

FourBanger is a simple calculator.

# Recursive Descent Parsing

The aim of parsing is to convert the inputted string into a meaningful series of actions and calculations. This program (and 99.9% of compilers) split's this process into two parts: lexical analysis and syntaxical analysis.

 In the lexical analysis phase, the text input is split into more meaningful chunks called tokens. For example, the input 1234 would be converted into a NUMBER token with value 1234; "(" would be converted into an OPEN_BRACKET token and "+" would be converted into an ADD_OPERATOR token. This form simplifies the processing in the next stage.

 In the syntaxical analysis phase, the token stream is converted into a more structured representation which greater represents the meaning of the inputted text. In the case of this calculator, for instance, the syntaxical analysis examines the tokens and generates and solves the mathematical expressions that the user has inputted, taking into account the orders of precendence.

 The language can be represented by a Formal Grammar. The parsers job is to try to match the inputted input string to a derivation tree (a graph which shows the structure of a particular sentance). There may be more that one grammar per formal language. The grammar below is structed in such a way that the derivation tree mimics the mathematical meaning of a sentance - operations which are performed first and lower down in the tree. As you move up the tree, every expression on each level gets computed until there is a final answer left on the root of the tree.

# Formal Language

The formal language for FourBanger is of the following EBNF form:

<St1>:	start = term;
<St2>:  start = ident "=" term;
<Te1>:	term = factor {["+"|"-"] factor};
<Fa1>:	factor = subfactor {["*"|"/"] subfactor};
<Sf1>:	subfactor = "(" term ")"
<Sf2>:  subfactor = number
		number = {0-9}+
		ident = (A-z) {(A-z)|(0-9)}+
Here the following conventions are used:
{...} - zero or more repetitions
{...}+- one or more repetiitions
[...] - optional
a|b	  -	choice
"t"	  - terminal



That production rules are labelled in the LHS of each line.

The grammar is LL(1). This means that the next production rule can be determined by looking solely at the next input to the parser. This avoids the need for backtracking. It also means that if an invalid symbol occurs within a production it is definately an error and not an alternative production.

|           | number |   "("   |   ")"  | ident |
|:---------:|:------:|:-------:|:------:|:-----:|
|     start |   St1  |   St1   |    -   |  St2  |
|      term |   Te1  |   Te1   |    -   |   -   |
|    factor |   Fa1  |   Fa1   |    -   |   -   |
| subfactor |   Sf1  |   Sf1   |    -   |   -   |

The above table shows how the transitions used by the predictive parser. Each row represents a non-terminal which is on the LHS of an expression, and each column represents the first symbol on the RHS of the expression. The content of each cell is the production rule that corresponds to each pair. A dash indicates that there are no rules with the column as the first symbol on the RHS and therefor that there is a syntax error.

Note that this table is not optimal but is unlikely to be optimised until the langauge gets into any final state (which is unlikely).