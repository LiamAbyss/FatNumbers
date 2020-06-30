#ifndef FAT_NUMBER
#define FAT_NUMBER

#include <iostream>
#include <cstdlib>

/** 
 * A class for manipulating very large or very small numbers
 * Uses linked lists and powers to store the meaningful digits
 * Zeros are meaningless digits and therefore will not be stored
 */
class FatNumber
{
public:
	FatNumber(const long long& value);
	FatNumber(const FatNumber& value);

	/** 
	 * Prints the number in the console 
	 */
	void print() const;

	FatNumber& operator+=(const FatNumber& num);
	FatNumber& operator++();
	FatNumber operator++(int a);
	FatNumber& operator*=(const FatNumber& num);
	FatNumber& operator=(const FatNumber& num);

	friend FatNumber operator+(const FatNumber& a, const FatNumber& b);

	friend bool operator<(const FatNumber& left, const FatNumber& right);
	friend bool operator>(const FatNumber& left, const FatNumber& right);
	friend bool operator==(const FatNumber& left, const FatNumber& right);
	friend bool operator!=(const FatNumber& left, const FatNumber& right);
	friend bool operator<=(const FatNumber& left, const FatNumber& right);
	friend bool operator>=(const FatNumber& left, const FatNumber& right);

private:

	void print(const FatNumber& power) const;

	/**
	 * @private
	 * updates the number to keep only one digit
	 */
	void update();

	/**
	 * @static
	 * @private
	 * Contains the maximum of value
	 */
	static const long long MAX_VALUE = 9;

	/** 
	 * @private
	 * Contains the digit of the current link
	 */
	long long value;

	/** 
	 * @private
	 * Contains the power of the current digit 
	 */
	std::unique_ptr<FatNumber> power;

	/** 
	 * @private
	 * Contains the following meaningful digit
	 */
	std::unique_ptr<FatNumber> postValue;
};

#endif // !FAT_NUMBER

