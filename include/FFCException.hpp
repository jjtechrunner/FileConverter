#ifndef _FFCEXCEPTION_HPP_
#define _FFCEXCEPTION_HPP_

#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>

using namespace std;

namespace FFC
{
	class FFCException
	{	
	public:
		FFCException(int errorCode, const string& iErrorText)
	         : _errorCode(errorCode),
			   _errorText(iErrorText)
		{}
		
	   FFCException(const FFCException& iRHS)
	   {
		   _errorCode = iRHS.getErrorCode();
		   _errorText = iRHS.getErrorText();
	   }
		
		const int getErrorCode() const
		{
			return _errorCode;
		}
		
		const string& getErrorText() const
		{
			return _errorText;
		}
	   
	   const string getErrorString() const
	   {
		   return boost::lexical_cast<string>(_errorCode) + "-" + _errorText;
	   }

	private:
	   int _errorCode;
	   string _errorText;
	   FFCException()
	   {}
		
   };
}

#endif