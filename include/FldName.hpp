#ifndef _FLDNAME_HPP_
#define _FLDNAME_HPP_

#include <iostream>
#include <string>
#include <FldUTFString.hpp>

using namespace std;

namespace FFC
{
	const int kNAME_MAX_LENGTH = 15;
	
	class FldName: public FldUTFString
	{
		public:
		FldName()
			: FldUTFString(kNAME_MAX_LENGTH)
		{}
		
		FldName(const FldName& iObj)
			: FldUTFString(iObj)
		{
		}

		FldName(string iStr)
			: FldUTFString(kNAME_MAX_LENGTH)
		{
			_status = INVALID;
			set(iStr);
		}
		
		~FldName()
		{}
	};
	
} // end namespace

#endif
