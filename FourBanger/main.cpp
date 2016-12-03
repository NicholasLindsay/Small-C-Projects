#include <iostream>
#include <sstream>
#include <stack>
#include <ctype.h>

// Struct to hold contents of symbol
struct Symbol
{
	enum SymbolType {
		NUMBER,
		ADD_OPERATOR,
		SUB_OPERATOR,
		MULTIPLY_OPERATOR,
		DIVIDE_OPERATOR,
		OPEN_BRACKET,
		CLOSE_BRACKET,
		END_OF_STREAM,
		ILLEGAL
	} Type;

	int NumericData;

	Symbol(SymbolType type, int number = 0)
		: Type(type), NumericData(number) {};

	Symbol()
		: Type(END_OF_STREAM) {};
};

// Class to get symbols from a std::istream
class TokenStream
{
public:
	TokenStream(std::iostream *str)
	{
		mStream = str;
		GetNextSym();
	}

	// Returns the current symbol held in the internal buffer
	Symbol CurrentSym() const {
		return mCurrentSym;
	}

	// Gets the next symbol from the input stream
	void GetNextSym() {
		// Check for end of file
		if (mStream->eof()) {
			mCurrentSym = Symbol(Symbol::END_OF_STREAM);
		}
		// Now start checking characters
		else {
			unsigned char cur_char;
			*mStream >> cur_char;

			// Number:
			if (isdigit(cur_char)) {
				int numeric_value = cur_char - '0';
				while (isdigit(mStream->peek()) && !mStream->eof()) {
					*mStream >> cur_char;
					numeric_value = numeric_value * 10 + (cur_char - '0');
				}
				mCurrentSym = Symbol(Symbol::NUMBER, numeric_value);
			}
			// Operators
			else if (cur_char == '+') {
				mCurrentSym = Symbol(Symbol::ADD_OPERATOR);
			}
			else if (cur_char == '-'){
				mCurrentSym = Symbol(Symbol::SUB_OPERATOR);
			}
			else if (cur_char == '*'){
				mCurrentSym = Symbol(Symbol::MULTIPLY_OPERATOR);
			}
			else if (cur_char == '/'){
				mCurrentSym = Symbol(Symbol::DIVIDE_OPERATOR);
			}
			// Brackets
			else if (cur_char == '('){
				mCurrentSym = Symbol(Symbol::OPEN_BRACKET);
			}
			else if (cur_char == ')'){
				mCurrentSym = Symbol(Symbol::CLOSE_BRACKET);
			}
			// Unrecognised
			else {
				mCurrentSym = Symbol(Symbol::ILLEGAL);
			}
		}
		
	}

private:
	std::iostream *mStream;
	Symbol mCurrentSym;
};

// Returns 1 if symbol matches, zero otherwise
int accept(TokenStream* str, Symbol s)
{
	if (str->CurrentSym().Type == s.Type) {
		str->GetNextSym();
		return 1;
	}
	else return 0;
}

// Similar to above but gives error message
int expect(TokenStream* str, Symbol s)
{
	if (accept(str, s))
		return 1;
	else {
		std::cout << "Error, unexpected symbol.\n";
		return 0;
	}
}

// Function prototypes
int term(TokenStream* str, int* number);

// Parse a subfactor and evaluate expression. Returns 1 if successful or 0 if
// input not accepted
int subfactor(TokenStream* str, int* number)
{
	Symbol currentSym = str->CurrentSym();

	// subfactor -> number
	if (accept(str, Symbol(Symbol::NUMBER))) {
		*number = currentSym.NumericData;
		return 1;
	}
	// subfactor -> "(" term ")"
	if (accept(str, Symbol(Symbol::OPEN_BRACKET))) {
		if (term(str, number)) {
			return (expect(str, Symbol(Symbol::CLOSE_BRACKET)));
		}
		else {
			std::cout << "Syntax error. Expected term.\n";
			return 0;
		}
	}

	// no match
	else return 0;
}

// Parse a factor and evaluate result. Returns 1 if successful or 0 if error
int factor(TokenStream* str, int* number)
{
	Symbol currentSym = str->CurrentSym();
	int eval_val = 0;

	// factor -> subfactor {[*]|[/] subfactor}
	if (subfactor(str, &eval_val)) {
		while (1) {
			if (accept(str, Symbol(Symbol::MULTIPLY_OPERATOR))) {
				int sub_val;
				if (subfactor(str, &sub_val)) {
					eval_val *= sub_val;
				}
				else {
					std::cout << "Unexpected symbol!\n";
					return 0;
				}
			}
			else if (accept(str, Symbol(Symbol::DIVIDE_OPERATOR))) {
				int sub_val;
				if (subfactor(str, &sub_val)) {
					eval_val /= sub_val;
				}
				else {
					std::cout << "Unexpected symbol!\n";
					return 0;
				}
			}
			else {
				*number = eval_val;
				return 1;
			}
		}
		
	}
	// not a valid production:
	else return 0;
}

// Parse a term and evaluate return. Returns 1 if successful or 0 if error
int term(TokenStream* str, int* number)
{
	int term_value = 0;

	if (factor(str, &term_value)){
		while (1){
			if (accept(str, Symbol::ADD_OPERATOR)){
				int factor_val = 0;
				if (factor(str, &factor_val)) {
					term_value += factor_val;
				}
				else {
					std::cout << "Syntax error. Expected a factor!\n";
					return 0;
				}
			}
			else if (accept(str, Symbol::SUB_OPERATOR)){
				int factor_val = 0;
				if (factor(str, &factor_val)) {
					term_value -= factor_val;
				}
				else {
					std::cout << "Syntax error. Expected a factor!\n";
					return 0;
				}
			}
			else {
				*number = term_value;
				return 1;
			}
		}
	}
	else {
		std::cout << "Syntax error. Expected a factor!\n";
		return 0;
	}
}

int main()
{
	std::cout << "Please enter an expression: ";
	std::string expr;
	std::cin >> expr;

	std::stringstream ss(expr);
	TokenStream token_str(&ss);
	int value = 0;
	
	if (term(&token_str, &value)) {
		std::cout << "Accepted! Value: " << value << ".\n";
	} else {
		std::cout << "Not accepted.\n";
	}
	
	std::getchar();
	std::getchar();
}
