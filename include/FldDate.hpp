#ifndef _FLDDATE_HPP_
#define _FLDDATE_HPP_

//  FldDate defined to wrap the date information and does the required validation with the formats specified
#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <boost/algorithm/string/trim.hpp>
#include <FldAbstract.hpp>

using namespace std;


namespace FFC
{
	class FldDate : public FldAbstract
	{
		public:
			enum FldDateFormat 
			{
				YYYY_MM_DD, 
				DD_MM_YYYY,
				DDMMYYYY
			};
			
			FldDate()
				:FldAbstract(),
				_format("%4d-%2d-%2d")		
			{
			}
		
			FldDate(const FldDate& iObj)
				: FldAbstract(iObj)
			{
				_day = iObj.getDay();
				_month = iObj.getMonth();
				_year = iObj.getYear();
				_format = iObj.getFormat();	
			}
		
			FldDate(const string& iDate, FldDateFormat iFormat = YYYY_MM_DD)
				: FldAbstract()
			{
				_status = INVALID;
				switch(iFormat)
				{
					case YYYY_MM_DD:
						_format= "%4d-%2d-%2d";
						break;
					case DD_MM_YYYY:
						_format = "%2d-%2d-%4d";
						break;
					case DDMMYYYY:
						_format = "%2d/%2d/%4d";
						break;
					default:
						_format = "";
						break;
				}
				set(iDate);
			}

			~FldDate()
			{
			}
			
			bool set(const string& iDate)
			{
				if(_format.empty())
				{
					cout << "Invalid format return \n";
					_status = INVALID;
					return false;
				}
				string aDateStr(iDate);
				boost::trim(aDateStr);			
				_status = (sscanf( aDateStr.c_str(), _format.c_str(), &_year, &_month, &_day) !=  EOF)? VALID : INVALID;
				if(!isInvalid())
				{
					return true;
				}
				return false;
			}
			
			string get(FldDateFormat iDateFormat= DDMMYYYY)
			{
				stringstream dateStream;
				switch(iDateFormat)
				{
					case DDMMYYYY:
						dateStream << _day  <<  "/"  <<  _month <<  "/"  <<  _year;
						break;
					case YYYY_MM_DD:
						dateStream << _year << "-" << _month << " -" << _day;
						break;
					case DD_MM_YYYY:
						dateStream << _day << "-" << _month << "-" << _year;
						break;
					default:
						dateStream << _day << "-" << _month << "-" << _year;
						break;			
				}

				return dateStream.str();
			}
			
			string getAsString()
			{
				stringstream dateStream;
				dateStream << _day  <<  "/"  <<  _month <<  "/"  <<  _year;
				return dateStream.str();
			}

			const string& getFormat() const
			{
				return _format;
			}
			
			const int getDay() const
			{
				return _day;
			}
			
			const int getMonth() const
			{
				return _month;
			}
			
			const int getYear() const
			{
				return _year;
			}
		private:
			int _day, _month, _year;
			string _format;
	};

}

#endif

