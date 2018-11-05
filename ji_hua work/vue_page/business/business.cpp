#include "business.h"

State* Business::Transition(char c)
{
	switch(c)
	{
		case 'A':
			return &s1;
		case 'B':
			return &s2;
		case 'C':
			return &s3;
		case 'D':
			return &s4;
		case '\0':
			return NULL;
		DEFAULT:
			return NULL;
	}
}