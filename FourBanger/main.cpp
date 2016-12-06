#include <iostream>
#include <sstream>
#include <stack>
#include <ctype.h>
#include <math.h>

// Complex number class
template <typename T>
class Complex
{
public:
	Complex()
		: real(0), imag(0) {};
	Complex(T r)
		: real(r), imag(0) {};
	Complex(T r, T i)
		: real(r), imag(i) {};

	// Global constants
	static Complex<T> j()
	{
		return Complex<T>(0, 1);
	}

	// Overload operators
	Complex<T> operator+(const Complex<T>& rhs) const
	{
		return Complex<T>(real + rhs.real, imag + rhs.imag);
	}
	Complex<T> operator-(const Complex<T> &rhs) const
	{
		return Complex<T>(real - rhs.real, imag - rhs.imag);
	}
	Complex<T> operator*(const Complex<T> &rhs) const
	{
		Complex<T> retval;
		retval.real = real * rhs.real - imag * rhs.imag;
		retval.imag = imag * rhs.real + real * rhs.imag;
		return retval;
	}
	Complex<T> operator/(const Complex<T> &rhs) const
	{
		Complex<T> retval;
		retval = (*this * rhs.congj()) / (rhs.magnitude());
		return retval;
	}
	
	// Compound assignment
	Complex<T>& operator+=(const Complex<T> &rhs)
	{
		real += rhs.real;
		imag += rhs.imag;
		return *this;
	}
	Complex<T>& operator-=(const Complex<T> &rhs)
	{
		real -= rhs.real;
		imag -= rhs.imag;
		return *this;
	}
	Complex<T>& operator *=(const Complex<T> &rhs)
	{
		real = real * rhs.real - imag * rhs.imag;
		imag = real * rhs.imag + imag * rhs.real;
		return *this;
	}
	Complex<T>& operator /=(const Complex<T> &rhs)
	{
		*this *= rhs.congj();
		this->real /= rhs.magnitude2();
		this->imag /= rhs.magnitude2();
		return *this;
	}

	// Negatation operator
	Complex<T> operator-()
	{
		return Complex<T>(-real, -imag);
	}

	// Complex-specific functions
	Complex<T> congj() const
	{
		return Complex<T>(real, -imag);
	}

	// Polar representation
	T magnitude() const
	{
		return (sqrt(real*real + imag*imag));
	}
	T angle() const
	{
		return atan2(real, imag);
	}
	// Magnitude squared:
	T magnitude2() const
	{
		return (*this * this->congj()).real;
	}

	// Internal representation
	T real, imag;
};

// Typedef the complex number type
typedef Complex<double> C_double;

// Symbol types used by symbol
enum SymbolType {
	NUMBER,
	ADD_OPERATOR,
	SUB_OPERATOR,
	MULTIPLY_OPERATOR,
	DIVIDE_OPERATOR,
	OPEN_BRACKET,
	CLOSE_BRACKET,
	EQUAL_OPERATOR,
	END_OF_STREAM,
	IDENTIFIER,
	ILLEGAL
};

// Struct to hold contents of symbol
struct Symbol
{
	enum SymbolType Type;

	int NumericData;
	std::string TextData;

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
			mCurrentSym = Symbol(END_OF_STREAM);
		}
		// Now start checking characters
		else {
			unsigned char cur_char;
			*mStream >> std::skipws >> cur_char;

			if (mStream->eof()) {
				mCurrentSym = Symbol(END_OF_STREAM);
			}
			// Number:
			else if (isdigit(cur_char)) {
				int numeric_value = cur_char - '0';
				while (isdigit(mStream->peek()) && !mStream->eof()) {
					*mStream >> cur_char;
					numeric_value = numeric_value * 10 + (cur_char - '0');
				}
				mCurrentSym = Symbol(NUMBER, numeric_value);
			}
			// Operators
			else if (cur_char == '+') {
				mCurrentSym = Symbol(ADD_OPERATOR);
			}
			else if (cur_char == '-'){
				mCurrentSym = Symbol(SUB_OPERATOR);
			}
			else if (cur_char == '*'){
				mCurrentSym = Symbol(MULTIPLY_OPERATOR);
			}
			else if (cur_char == '/'){
				mCurrentSym = Symbol(DIVIDE_OPERATOR);
			}
			// Brackets
			else if (cur_char == '('){
				mCurrentSym = Symbol(OPEN_BRACKET);
			}
			else if (cur_char == ')'){
				mCurrentSym = Symbol(CLOSE_BRACKET);
			}
			// Letter (identifier):
			else if (isalpha(cur_char)) {
				// Disable ws until we get full identifer
				*mStream >> std::noskipws;

				mCurrentSym = Symbol(IDENTIFIER);

				while (cur_char != ' ' && !mStream->eof()){
					mCurrentSym.TextData += cur_char;
					*mStream >> cur_char;
				}

				// Re-enable ws
				*mStream >> std::skipws;
			}
			// Unrecognised
			else {
				mCurrentSym = Symbol(ILLEGAL);
			}
		}
		
	}

private:
	std::iostream *mStream;
	Symbol mCurrentSym;
};

// Returns 1 if symbol matches and consumes symbol, zero otherwise
int accept(TokenStream* str, SymbolType s)
{
	if (str->CurrentSym().Type == s) {
		str->GetNextSym();
		return 1;
	}
	else return 0;
}

// Similar to above but gives error message
int expect(TokenStream* str, SymbolType s)
{
	if (accept(str, s))
		return 1;
	else {
		std::cout << "Error, unexpected symbol.\n";
		return 0;
	}
}

// Function prototypes
int term(TokenStream* str, C_double* number);

// Parse the start symbol and evaluates expression. 1 if succesful, 0 otherwise
int start(TokenStream* str, C_double* number)
{
	switch (str->CurrentSym().Type)
	{
	case NUMBER:
	case OPEN_BRACKET:
	{
		if (term(str, number)) {
			if (str->CurrentSym().Type == END_OF_STREAM) {
				return 1;
			}
			else {
				return 0;
			}
		}
		break;
	}
	case IDENTIFIER:
	{
		// Insert code here to remember identifier name
		if (expect(str, EQUAL_OPERATOR)) {
			C_double temp_no;
			if (term(str, &temp_no)) {
				// Insert code here to update symbol table
			}
			else {
				return 0;
			}
		}
		else
		{
			return 0;
		}
		break;
	}
	default:
	{
		std::cout << "Syntax error. Expected a term.\n";
		return 0;
	}
	}
}

// Parse a subfactor and evaluate expression. Returns 1 if successful or 0 if
// input not accepted
int subfactor(TokenStream* str, C_double* number)
{
	C_double local_no(0, 0);

	switch (str->CurrentSym().Type)
	{
	case NUMBER: {
		*number = str->CurrentSym().NumericData;
		str->GetNextSym();
		return 1;
		break;
	}
	case OPEN_BRACKET: {
		str->GetNextSym();
		if (term(str, &local_no)) {
			if (expect(str, CLOSE_BRACKET)) {
				*number = local_no;
				return 1;
			}
			else {
				return 0;
			}
		}
		else {
			std::cout << "Error. Expected a term.\n";
			return 0;
		}
		break;
	}
	default: {
		std::cout << "Unexpected symbol.\n";
		return 0;
	}
	}
}

// Parse a factor and evaluate result. Returns 1 if successful or 0 if error
int factor(TokenStream* str, C_double* number)
{
	C_double local_no(0, 0);

	switch (str->CurrentSym().Type)
	{
	case NUMBER:
	case OPEN_BRACKET:
	{
		if (subfactor(str, &local_no))
		{
			C_double temp_no;

			while (1) {
				if (accept(str, MULTIPLY_OPERATOR)) {
					if (subfactor(str, &temp_no)) {
						local_no *= temp_no;
					}
					else {
						return 0;
					}
				}
				else if (accept(str, DIVIDE_OPERATOR)) {
					if (subfactor(str, &temp_no)) {
						local_no /= temp_no;
					}
					else {
						return 0;
					}
				}
				else {
					*number = local_no;
					return 1;
				}
			}
			
		}
		else {
			return 0;
		}
		break;
	}
	default:
	{
		std::cout << "Unexpected symbol.\n";
		return 0;
	}
	}
}

// Parse a term and evaluate result. Returns 1 if successful or 0 if error
int term(TokenStream* str, C_double* number)
{
	C_double local_no(0, 0);

	switch (str->CurrentSym().Type)
	{
	case NUMBER:
	case OPEN_BRACKET:
	{
		if (factor(str, &local_no)) {
			while (1) {
				C_double temp_no;
				if (accept(str, ADD_OPERATOR)) {
					if (factor(str, &temp_no)) {
						local_no += temp_no;
					}
					else {
						return 0;
					}
				}
				else if (accept(str, SUB_OPERATOR)) {
					if (factor(str, &temp_no)) {
						local_no -= temp_no;
					}
					else {
						return 0;
					}
				}
				else {
					*number = local_no;
					return 1;
				}
			}
		}
		else {
			return 0;
		}
		break;
	}
	default:
	{
		std::cout << "Unexpected symbol.\n";
		return 0;
	}
	}
}

int main()
{
	std::cout << "Please enter an expression: ";
	std::string expr;
	std::getline(std::cin, expr);

	std::stringstream ss(expr);
	TokenStream token_str(&ss);
	C_double value = 0;
	
	if (start(&token_str, &value)) {
		std::cout << "Accepted! Value: " << value.real << " + " << value.imag << "j.\n";
	} else {
		std::cout << "Not accepted.\n";
	}
	
	Complex<double> c1(1, 3);
	Complex<double> c2(2, -1);
	auto c3 = c1 * c2 *  0;

	std::getchar();
	std::getchar();
}
