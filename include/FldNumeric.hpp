#ifndef _FLDNUMERIC_HPP_
#define _FLDNUMERIC_HPP_

//  FldDate defined to wrap the date information and does the required validation with the formats specified
#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <string>
#include <iomanip>

using namespace std;


namespace FFC
{
	class FldNumeric : public FldAbstract
	{
		public:
			
			FldNumeric()
				:FldAbstract(),
				_value(0.0)
			{
			}
			
			FldNumeric(const FldNumeric& iObj)
				:FldAbstract(iObj)
			{
				_value = iObj.get();
			}
			
			FldNumeric(const string& iStr)
				:FldAbstract(),
				_value(0.0)
			{				
				set(iStr);				
			}

			~FldNumeric()
			{}
		
			bool set(string iData)
			{
				boost::trim(iData);
				if(!iData.empty())
				{
					_value = std::atof(iData.c_str());
					_status = VALID;
				}
				else
				{
					_status = INVALID;
				}
				
				return _status;
			}
			
			string get()
			{
				ostringstream out;
				out.precision(2);
				out << _value;
				return out.str();				
			}
			
			string getAsString()
			{
				ostringstream out;
				out << _value;
				return out.str();	
			}
			
			float get() const 
			{
				return _value;
			}

		private:
			float _value;
	};

}

#endif

