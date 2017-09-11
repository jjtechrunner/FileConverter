#include <FileConverter.hpp>

#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include <stdlib.h>
#include <FFCException.hpp>
#include <FldNumeric.hpp>
#include <FldName.hpp>
#include <FldUTFString.hpp>
#include <FldDate.hpp>

namespace FFC
{
	// defining error codes
	const int kECInvalidFileName =  1000;
	const int kECInvalidMetaFile = 1001;
	const int kECInvalidInputFile = 1002;
	const int kECInvalidFormat = 1003;
	const int kECInvalidOutputFile = 1004;
	const int kECOutputFileWritingError = 1005;
	
	const string strCR = "\r", strLF="\n";
	
	
	template<typename T> FldAbstract* createDataField(const string& iDataStr)
	{

		FldAbstract* pDataField = new T(iDataStr);
		return pDataField;
	}
	
	
	
	bool createDataRecord(const string& iDataStr, 
		                  vector<FieldMetaData>& iMetaData,
						  DataRecord* oDataRecord)
	{
		int i = 0;
		size_t beginPos = 0, endPos=0;
		BOOST_FOREACH(FieldMetaData& iMetaField, iMetaData)
		{
			if(beginPos >= iDataStr.length())
			{
				break;
			}
				
			endPos = beginPos + iMetaField._length;	
			string aFieldStr = iDataStr.substr(beginPos, (endPos-beginPos));
			if(iMetaField._fieldDataType == "FldDate")
			{
				oDataRecord->addDataField(make_pair(i,createDataField<FldDate>(aFieldStr)));
			}
			else if(iMetaField._fieldDataType == "FldUTFString")
			{
				oDataRecord->addDataField(make_pair(i,createDataField<FldUTFString>(aFieldStr)));
			}
			else if(iMetaField._fieldDataType == "FldName")
			{
				oDataRecord->addDataField(make_pair(i,createDataField<FldName>(aFieldStr)));
			}
			else if(iMetaField._fieldDataType == "FldNumeric")
			{
				oDataRecord->addDataField(make_pair(i,createDataField<FldNumeric>(aFieldStr)));
			}
			beginPos = endPos;
			i++;
		}
		if(i > 0)
			return true;
		
		return false;	
	}
	
	void clearDataRecords(vector<DataRecord*> iDataRecords)
	{
		BOOST_FOREACH(DataRecord* iData, iDataRecords)
		{
			delete iData;
		}
		iDataRecords.clear();
	}
	
	void FileConverter::writeToOutputFile(ofstream& oOutFile, const std::vector<DataRecord*>& iDataRecords, bool iWriteHeader)
	{
		string aOutPutStream;
		if(iWriteHeader)
		{
			int i = 0, aFldSize = _fieldinfo.size();		
			BOOST_FOREACH(FieldMetaData& iMetaField, _fieldinfo)
			{
				aOutPutStream += iMetaField._name;
				i++;
				if(i < aFldSize)
				{
					aOutPutStream += ",";
				}
			}
			aOutPutStream += "\n";
		}
		
		BOOST_FOREACH(const DataRecord* iData, iDataRecords)
		{
			aOutPutStream.append(iData->getCSVString());
		}
		
		oOutFile.write(aOutPutStream.c_str(), aOutPutStream.size());		
	}
	

	FileConverter::~FileConverter()
	{		
	}

	bool FileConverter::isFilesValid()
	{
		cout <<  _inputFileName << " " << _metaFileName << "   " << _outputFileName << endl;
		if(_inputFileName.empty() ||
			_metaFileName.empty() ||
			_outputFileName.empty() )
		{
			cout << "File names are not valid, return \n";
			throw FFCException(kECInvalidFileName, "Invalid File names");
		}
		return true;
		// Check input and meta file exists and ouptut file don't exist			
	}
	
	bool FileConverter::parseAndStoreMetaData()
	{
		cout << "Parase meta data from file " << _metaFileName << endl;
		ifstream aMetaFile;
		
		 try 
		 {
			aMetaFile.open (_metaFileName.c_str());			
			string aMetaDataStr = "";
			while(getline(aMetaFile, aMetaDataStr))
			{
				boost::trim(aMetaDataStr);
				boost::char_separator<char> separator(",");
				boost::tokenizer< boost::char_separator<char> > tokens(aMetaDataStr, separator);
				
				FieldMetaData aMetaData;
				int i = 1;
				BOOST_FOREACH(string aToken, tokens)
				{
					if(i == 1)
					{
						aMetaData._name = aToken;
					}
					else if( i == 2)
					{
						aMetaData._length = atoi(aToken.c_str());
					}
					else if(i == 3)
					{
						aMetaData._flatFileDataType = aToken;
						if(aToken == "string")
						{
							aMetaData._fieldDataType = (aMetaData._name.find("name")!=std::string::npos)?"FldName":"FldUTFString";							
						}
						else if(aToken == "date")
						{
							aMetaData._fieldDataType = "FldDate";
						}
						else if(aToken == "numeric")
						{
							aMetaData._fieldDataType = "FldNumeric";
						}
					}
					i++;
				}
				if(!aMetaData._name.empty())					
					_fieldinfo.push_back(aMetaData);
			}
			
			aMetaFile.close();
		 }
		 catch (const ifstream::failure& e) 
		 {
			cout << "Exception opening/reading meta data file \n";
			string aExceptionStr = "Exception in processing meta file " + _metaFileName;
			throw FFCException(kECInvalidMetaFile, aExceptionStr);
		 }
		 
		 cout << "Meta data file is parsed successfully " << endl;
		 return true;
	}
	
	bool FileConverter::processInputFile()
	{
		cout << "Read and parse input file  " << _inputFileName 
		     <<  " and write to output file " << _outputFileName << endl;
		ifstream aInputFile;
		ofstream aOutputFile;
		
		try
		{
			aInputFile.open(_inputFileName.c_str());
		}
		catch (const ifstream::failure& e) 
		{
			cout << "Exception opening input data file \n";
			string aExceptionStr = "Exception in processing input file " + _inputFileName;
			throw FFCException(kECInvalidInputFile, aExceptionStr);
		}
		

		try
		{
			aOutputFile.open(_outputFileName.c_str(), ofstream::out);
		}
		catch (const ofstream::failure& e) 
		{
			cout << "Exception opening output data file \n";
			string aExceptionStr = "Exception in processing output file " + _outputFileName;
			throw FFCException(kECInvalidOutputFile, aExceptionStr);
		}		
		

		int  aNumberOfRecords = 0;
		try 
		{
			string aInputFileStr = "";
			std::vector<DataRecord*> aDataRecords;
			while(getline(aInputFile, aInputFileStr))
			{
				// remove any extra spaces
				boost::trim(aInputFileStr);
				
				// check if any CR or LF exists inthe string
				if(aInputFileStr.find(strCR) != std::string::npos ||
				   aInputFileStr.find(strLF) != std::string::npos)
				{
					cout << "String CR or LF, register exception \n";
					throw FFCException(kECInvalidFormat, "Input file contains invalid characters");
				}

				DataRecord* pDataRecord = new DataRecord();
				if(createDataRecord(aInputFileStr, _fieldinfo, pDataRecord))
				{
					aDataRecords.push_back(pDataRecord);
				}
				
				aNumberOfRecords++;
				
				if((aNumberOfRecords >= 1000) && (aNumberOfRecords%1000) == 0)
				{
					// write to output file
					writeToOutputFile(aOutputFile, aDataRecords, (aNumberOfRecords==1000));
					clearDataRecords(aDataRecords);
				}
			}
			writeToOutputFile(aOutputFile, aDataRecords, aNumberOfRecords < 1000);
			clearDataRecords(aDataRecords);
			
			aInputFile.close();
			aOutputFile.close();
		 }
		 catch(const ios_base::failure& e)
		 {
			aInputFile.close();
			aOutputFile.close();
			cout << "Exception reading input data file or writing output file\n";
			string aExceptionStr = "Exception in procesing input file or writing to output file\n";
			throw FFCException(kECOutputFileWritingError, aExceptionStr);			
		 }
		 return true;
	}
	

}
