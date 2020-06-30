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
	else if(power)
	{
		for(FatNumber i = 0; i < *power; i = i++)
		{
			cout << 0;
		}
	}
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
	else if (power > 0)
	{
		for (FatNumber i = 1; i < power; i = i++)
		{
			cout << 0;
		}
	}
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

FatNumber& FatNumber::operator++()
{
	*this += 1;
	return *this;
}

FatNumber FatNumber::operator++(int)
{
	FatNumber tmp = *this;
	*this += 1;
	return tmp;
}

FatNumber& FatNumber::operator*=(const FatNumber& num)
{
	// TODO : SET POWER RECURSIVELY

	FatNumber buffer1 = value * num.value;
	FatNumber* buffer1Post = buffer1.postValue.get();
	if (power)
		if (!buffer1.power)
			buffer1.power = make_unique<FatNumber>(*power);
		else
		{
			*buffer1.power += *power;
			while (buffer1Post)
			{
				if (!buffer1Post->power)
					buffer1Post->power = make_unique<FatNumber>(*power);
				else
					*buffer1Post->power += *power;
				buffer1Post = buffer1Post->postValue.get();
			}
			buffer1Post = buffer1.postValue.get();
		}
	if (power && num.power)
	{
		*buffer1.power += *num.power;
		while (buffer1Post)
		{
			if (!buffer1Post->power)
				buffer1Post->power = make_unique<FatNumber>(*num.power);
			else
				*buffer1Post->power += *num.power;
			buffer1Post = buffer1Post->postValue.get();
		}
		buffer1Post = buffer1.postValue.get();
	}
	else if (num.power)
		if (!buffer1.power)
			buffer1.power = make_unique<FatNumber>(*num.power);
		else
		{
			*buffer1.power += *num.power;
			while (buffer1Post)
			{
				if (!buffer1Post->power)
					buffer1Post->power = make_unique<FatNumber>(*num.power);
				else
					*buffer1Post->power += *num.power;
				buffer1Post = buffer1Post->postValue.get();
			}
		}

	FatNumber buffer2 = 0;
	if (postValue && num.postValue)
	{
		buffer2 = *postValue;
		buffer2 *= *num.postValue;
	}

	// Value and num.value <= 9, so this method is okayish
	FatNumber buffer3 = 0;
	if (num.postValue)
	{
		for (long long i = 0; i < value; i++)
			buffer3 += *num.postValue;

		FatNumber* buffer3Post = buffer3.postValue.get();
		if (power)
			if(!buffer3.power)
				buffer3.power = make_unique<FatNumber>(*power);
			else
			{
				*buffer3.power += *power;
				while (buffer3Post)
				{
					if (!buffer3Post->power)
						buffer3Post->power = make_unique<FatNumber>(*power);
					else
						*buffer3Post->power += *power;
					buffer3Post = buffer3Post->postValue.get();
				}
				buffer3Post = buffer3.postValue.get();
			}
		/*if (power && num.postValue->power)
		{
			*buffer3.power += *num.postValue->power;
			while (buffer3Post)
			{
				if (!buffer3Post->power)
					buffer3Post->power = make_unique<FatNumber>(*num.postValue->power);
				else
					*buffer3Post->power += *num.postValue->power;
				buffer3Post = buffer3Post->postValue.get();
			}
			buffer3Post = buffer3.postValue.get();
		}*/
		else if (num.postValue->power)
			if(!buffer3.power)
				buffer3.power = make_unique<FatNumber>(*num.postValue->power);
			else
			{
				*buffer3.power += *num.postValue->power;
				while (buffer3Post)
				{
					if (!buffer3Post->power)
						buffer3Post->power = make_unique<FatNumber>(*num.postValue->power);
					else
						*buffer3Post->power += *num.postValue->power;
					buffer3Post = buffer3Post->postValue.get();
				}
			}
	}

	FatNumber buffer4 = 0;
	if (postValue)
	{
		for (long long i = 0; i < num.value; i++)
			buffer4 += *postValue;

		FatNumber* buffer4Post = buffer4.postValue.get();
		if (num.power)
			if (!buffer4.power)
				buffer4.power = make_unique<FatNumber>(*num.power);
			else
			{
				*buffer4.power += *num.power;
				while (buffer4Post)
				{
					if (!buffer4Post->power)
						buffer4Post->power = make_unique<FatNumber>(*num.power);
					else
						*buffer4Post->power += *num.power;
					buffer4Post = buffer4Post->postValue.get();
				}
				buffer4Post = buffer4.postValue.get();
			}
		/*if (postValue->power && num.power)
		{
			*buffer4.power += *num.power;
			while (buffer4Post)
			{
				if (!buffer4Post->power)
					buffer4Post->power = make_unique<FatNumber>(*num.power);
				else
					*buffer4Post->power += *num.power;
				buffer4Post = buffer4Post->postValue.get();
			}
			buffer4Post = buffer4.postValue.get();
		}*/
		else if (postValue->power)
			if (!buffer4.power)
				buffer4.power = make_unique<FatNumber>(*postValue->power);
			else
			{
				*buffer4.power += *postValue->power;
				while (buffer4Post)
				{
					if (!buffer4Post->power)
						buffer4Post->power = make_unique<FatNumber>(*postValue->power);
					else
						*buffer4Post->power += *postValue->power;
					buffer4Post = buffer4Post->postValue.get();
				}
			}
	}

	*this = buffer1 + buffer3 + buffer4 + buffer2;
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
	update();
	return *this;
}

void FatNumber::update()
{
	if (postValue)
	{
		if (power && postValue->power)
		{
			if (*power == *postValue->power)
			{
				value += postValue->value;
				postValue.reset(postValue->postValue.release());
			}
		}
	}

	// Separate first digit from the rest
	long long shift = 1;
	int shiftCount = 0;
	while (value / shift > MAX_VALUE)
	{
		shift *= 10;
		shiftCount++;
	}
	if(shiftCount)
	{
		int tmpCount = shiftCount;
		long long tmpShift = shift;
		long long tmpVal = value;
		FatNumber tmp = 0;
		while(tmpCount >= 0)
		{
			FatNumber tmp2 = tmpVal / tmpShift;
			tmpVal -= (tmpVal / tmpShift) * tmpShift;
			tmp2.power = make_unique<FatNumber>(tmpCount+(power?*power:0));
			tmpShift /= 10;
			tmpCount--;
			tmp += tmp2;
		}
		if(this->postValue)
			tmp += *this->postValue;
		*this = tmp;
	}

	FatNumber* previous = this;
	FatNumber* tmpPost = postValue.get();
	while(tmpPost)
	{
		if (tmpPost->power && !tmpPost->power->value)
		{
			tmpPost->power.reset();
		}
		if(!tmpPost->value)
		{
			previous->postValue.reset(tmpPost->postValue.release());
			tmpPost = NULL;
		}
		if(tmpPost == NULL)
		{
			tmpPost = previous->postValue.get();
		}
		else
		{
			previous = tmpPost;
			tmpPost = tmpPost->postValue.get();
		}
	}
	
}

FatNumber operator+(const FatNumber& a, const FatNumber& b)
{
	FatNumber result = a;
	result += b;
	return result;
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
