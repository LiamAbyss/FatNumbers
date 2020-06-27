#include "FatNumber.h"

using namespace std;

FatNumber::FatNumber(const long long& value) : value(value), power(nullptr), postValue(nullptr)
{
	update();
}

FatNumber::FatNumber(const FatNumber& value) : value(value.value), power(nullptr), postValue(nullptr)
{
	if (value.power)
		power = make_unique<FatNumber>(*value.power);
	if (value.postValue)
		postValue = make_unique<FatNumber>(*value.postValue);
}

void FatNumber::print() const
{
	cout << value;// << ((power && power->value % 3 == 0) ? " " : "");
	if(postValue) postValue->print((power)?*power:0);
}

void FatNumber::print(const FatNumber& power) const
{
	FatNumber buffer = (this->power) ? *this->power : 0;
	buffer += 1;
	while(buffer != power)
	{
		buffer += 1;
		cout << 0;// << ((buffer.value - 1 % 3 == 0) ? " " : "");
	}
	cout << value;// << ((this->power && this->power->value % 3 == 0) ? " " : "");
	if (postValue) postValue->print((this->power) ? *this->power : 0);
}


FatNumber& FatNumber::operator+=(const FatNumber& num)
{
	int postpone = 0;
	long long newVal = 0;
	FatNumber newPostVal = 0;
	if(power && num.power)
	{
		if (*power > * num.power)
			postpone = 1;
		else if (*power < *num.power)
			postpone = 2;
		else
		{
			newVal = value + num.value;
			if (postValue && num.postValue)
			{
				newPostVal = *postValue;
				newPostVal += *num.postValue;
			}
			else if (postValue)
				newPostVal = *postValue;
			else if (num.postValue)
				newPostVal = *num.postValue;
		}
	}
	else if(power)
	{
		if (*power > 0)
			postpone = 1;
		else if (*power < 0)
			postpone = 2;
	}
	else if(num.power)
	{
		if (*num.power < 0)
			postpone = 1;
		else if (*num.power > 0)
			postpone = 2;
	}
	else
	{
		newVal = value + num.value;
		if (postValue && num.postValue)
		{
			newPostVal = *postValue;
			newPostVal += *num.postValue;
		}
		else if (postValue)
			newPostVal = *postValue;
		else if (num.postValue)
			newPostVal = *num.postValue;
	}

	FatNumber buffer = *this;
	switch(postpone)
	{
	case 1:
		if (postValue)
			*postValue += num;
		else
			postValue = make_unique<FatNumber>(num);
		break;
	case 2:
		*this = num;
		if (postValue)
			*postValue += buffer;
		else
			postValue = make_unique<FatNumber>(buffer);
		break;
	default:
		value = newVal;
		if(postValue)
			*postValue = newPostVal;
		else if(newPostVal != 0)
		{
			postValue = make_unique<FatNumber>(newPostVal);
		}
		break;
	}
	update();
	return *this;
}

FatNumber& FatNumber::operator=(const FatNumber& num)
{
	value = num.value;
	if (num.postValue)
		if (postValue)
			*postValue = *num.postValue;
		else
			postValue = make_unique<FatNumber>(*num.postValue);
	else if (postValue)
		postValue.reset();

	if (num.power)
		if (power)
			*power = *num.power;
		else
			power = make_unique<FatNumber>(*num.power);
	else if (power)
		power.reset();

	return *this;
}

void FatNumber::update()
{
	// Separate first digit from the rest
	long long shift = 1;
	int shiftCount = 0;
	while (value / shift > MAX_VALUE)
	{
		shift *= 10;
		shiftCount++;
	}
	long long post = value - (value / shift) * shift;
	long long newVal = value / shift;
	value = newVal;
	// TODO: Handle power shifts
	if(!power && shiftCount != 0)
	{
		power = make_unique<FatNumber>(shiftCount);
	}

	if(!postValue && post != 0)
	{
		postValue = make_unique<FatNumber>(post);
	}
}

bool operator<(const FatNumber& left, const FatNumber& right)
{
	if(left.power && right.power)
	{
		if (*left.power < *right.power)
			return true;
		else if (*left.power == *right.power)
		{
			if (left.value < right.value)
				return true;
			else if(left.value == right.value)
			{
				if (left.postValue && right.postValue && *left.postValue < *right.postValue)
					return true;
				else if (left.postValue && !right.postValue && *left.postValue < 0)
					return true;
				else if (right.postValue && !left.postValue && *right.postValue > 0)
					return true;
			}
		}
	}
	else if(left.power)
	{
		if (*left.power < 0)
			return true;
	}
	else if(right.power)
	{
		if (*right.power > 0)
			return true;		
	}
	else
	{
		if (left.value < right.value)
			return true;
		else if (left.value == right.value)
		{
			if (left.postValue && right.postValue && *left.postValue < *right.postValue)
				return true;
			else if (left.postValue && !right.postValue && *left.postValue < 0)
				return true;
			else if (right.postValue && !left.postValue && *right.postValue > 0)
				return true;
		}
	}
	return false;
}

bool operator>(const FatNumber& left, const FatNumber& right)
{
	if (left.power && right.power)
	{
		if (*left.power > *right.power)
			return true;
		else if (*left.power == *right.power)
		{
			if (left.value > right.value)
				return true;
			else if (left.value == right.value)
			{
				if (left.postValue && right.postValue && *left.postValue > *right.postValue)
					return true;
				else if (left.postValue && !right.postValue && *left.postValue > 0)
					return true;
				else if (right.postValue && !left.postValue && *right.postValue < 0)
					return true;
			}
		}
	}
	else if (left.power)
	{
		if (*left.power > 0)
			return true;
	}
	else if (right.power)
	{
		if (*right.power < 0)
			return true;
	}
	else
	{
		if (left.value > right.value)
			return true;
		else if (left.value == right.value)
		{
			if (left.postValue && right.postValue && *left.postValue > *right.postValue)
				return true;
			else if (left.postValue && !right.postValue && *left.postValue > 0)
				return true;
			else if (right.postValue && !left.postValue && *right.postValue < 0)
				return true;
		}
	}
	return false;
}

bool operator==(const FatNumber& left, const FatNumber& right)
{
	return !(left < right || left > right);
}

bool operator!=(const FatNumber& left, const FatNumber& right)
{
	return !(left == right);
}

bool operator<=(const FatNumber& left, const FatNumber& right)
{
	return (left < right || left == right);
}

bool operator>=(const FatNumber& left, const FatNumber& right)
{
	return (left > right || left == right);
}
