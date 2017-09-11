#ifndef _FLDUTFSTRING_HPP_
#define _FLDUTFSTRING_HPP_

#include <iostream>
#include <string>
#include <FldAbstract.hpp>
#include <boost/algorithm/string/trim.hpp>

using namespace std;

namespace FFC
{
	class FldUTFString: public FldAbstract
	{
		public:
		FldUTFString(int iMaxLength)
			: FldAbstract(),
			_maxLength(iMaxLength)
		{}
		
		FldUTFString(const FldUTFString& iObj)
			: FldAbstract(iObj)
		{
			_value = iObj.getValue();
			_maxLength =  iObj.getMaxLength();
		}
		
		FldUTFString(int iMaxLength, string iStr)
		{
			_maxLength = iMaxLength;
			_status = INVALID;
			set(iStr);
		}

		FldUTFString(string iStr)
		{
			_maxLength = iStr.length();
			_status = INVALID;
			set(iStr);
		}
		
		~FldUTFString()
		{}
		
		void set(string iStr)
		{
			boost::trim(iStr);
			if(!iStr.empty() && iStr.length() <= _maxLength)
			{
				_value = iStr;
				_status = VALID;
				return;
			}

			_status = INVALID;
		}
		
		string& get()
		{
			return _value;
		}
		
		const string& getValue() const
		{
			return _value;
		}
		
		string getAsString()
		{
			return _value;
		}
		
		int getMaxLength()
		{
			return _maxLength;
		}
		
		const int getMaxLength() const
		{
			return _maxLength;
		}
		
		protected:
			string _value;
			int _maxLength;
	};
	
} // end namespace

#endif
