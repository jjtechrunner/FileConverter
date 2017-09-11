#ifndef _FILECONVERTER_HPP_
#define _FILECONVERTER_HPP_

#include <iostream>
#include <FldUTFString.hpp>
#include <FldDate.hpp>
#include <string>
#include <fstream>
#include <vector>
#include <map>


using namespace std;

namespace FFC
{
	/*
	* strcuture to store the meta data
	*/
	struct FieldMetaData
	{
		string _name; // name of the field
		int _length; // length of the fiel
		string _flatFileDataType; // flat file data type
		string _fieldDataType; // applicaiton data type
	}; 
	
	// represents one flat file record in structured format
	// using map, can store any number of data
	class DataRecord
	{
	public:
		//constructor
		DataRecord()
		{}
		
		// copy constructor
		DataRecord(const DataRecord& iRHS)
		{
			for (std::map<int, FldAbstract*>::const_iterator it=iRHS.getMapOfData().begin(); it!=iRHS.getMapOfData().end(); ++it)
			{
				_mapOfData[(*it).first] = (*it).second;
			}
		}

		// destructor
		~DataRecord()
		{
			for (std::map<int, FldAbstract*>::iterator it=_mapOfData.begin(); it!=_mapOfData.end(); ++it)
			{
				delete (*it).second;
			}
		}
		
		const map<int, FldAbstract*>& getMapOfData() const
		{
			return _mapOfData;
		}
		
		void addDataField(pair<int, FldAbstract*> iDataField)
		{
			_mapOfData.insert(iDataField);
		}
		
		/*
		* returns the data record as a comma separated string with enclosed in double quotes if there is any special characters
		*/
		string getCSVString() const
		{
			string aDataRecordString;
			int i = 0;
			for (std::map<int, FldAbstract*>::const_iterator it=_mapOfData.begin(); it!=_mapOfData.end(); ++it)
			{
				if((*it).second != NULL )
				{
					string aTempStr = (*it).second->getAsString();
					if(std::find_if(aTempStr.begin(), aTempStr.end(), DataRecord::isSpecialCharacter) != aTempStr.end())
					{
						aTempStr.insert(0, "\"");
						aTempStr.append("\"");
					}
					aDataRecordString.append(aTempStr);
				}
				i++;
				if(i < _mapOfData.size())
				{
					aDataRecordString.append(",");
				}
			}
			// replace special characters with double quotes
			
			aDataRecordString += "\n";
			return aDataRecordString;
		}
		
		

	private:
	
		static bool isSpecialCharacter(char ch)
		{
			return !(isalnum(ch) || ch == '.' || ch == '/');
		}
		
	private:
		map<int, FldAbstract*> _mapOfData;	// store the data record elements in a map.	
	};
	

	/*
	* File Converts wraps the whole processing
	* Read nd store meta data
	* read input file and write to ouput file in chunks of 1000 records to be faster writing.
	*/	

	class FileConverter
	{
		public:		
			FileConverter()
			{
			}
			
			FileConverter(const string& iInputFileName,
			              const string& iMetaFileName,
						  const string& iOutputFile)
				: _inputFileName(iInputFileName),
				  _metaFileName(iMetaFileName),
				  _outputFileName(iOutputFile)
			{}
			
			~FileConverter();
			
			// check files are valid
			bool isFilesValid();
			
			/**
			* Parse the meta data file and store the information in the vector
			*/
			bool parseAndStoreMetaData();
			
			/** 
			* Read, parse and write the data to output file
			*/
			bool processInputFile();
			
			void  writeToOutputFile(ofstream& oOutputFile, const std::vector<DataRecord*>& iDataRecords, bool iWriteHeader=false);
		
		private:
			string _inputFileName;
			string _metaFileName;
			string _outputFileName;
			vector <FieldMetaData> _fieldinfo; // met data
	};
}

#endif