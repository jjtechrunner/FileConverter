#ifndef _FLDABSTRACT_HPP_
#define _FLDABSTRACT_HPP_

#include <iostream>
#include <string>

using namespace std;

namespace FFC
{
	// enumeration for the field values
	enum FldStatus {UNDEFINED=-1, INVALID=0, VALID=1};

	class FldAbstract
	{
	public:
	
		FldAbstract(const FldAbstract& iObj)
		{
			_status = iObj.getStatus();
		}

		virtual ~FldAbstract() = 0;
		
		bool isValid()
		{
			_status == VALID;
		}
			
		bool isUndefined()
		{
			_status == UNDEFINED;
		}
			
		bool isInvalid()
		{
			_status == INVALID;
		}
		
		virtual string getAsString() = 0;
		
		const int getStatus() const
		{
			return _status;
		}
			
	protected:
		
		FldAbstract(): _status(UNDEFINED)
		{}
		
		int _status;
	};

}

#endif