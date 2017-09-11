// Convert the fixed format file to csv format

#include <iostream>
#include <stdlib.h>
#include <FldName.hpp>
#include <FldDate.hpp>
#include <FldNumeric.hpp>
#include <FileConverter.hpp>
#include <FldDate.hpp>
#include <FFCException.hpp>


using namespace std;
using namespace FFC;

int main(int argc, char *argv[])
{
	
	// display usage
	if(argc < 6)
	{
		cout <<"Flat file converter written by J. Jose \n";
		cout <<"================ Usage is ========================= \n";
		cout << "          -i <input file> \n";
		cout << "          -m <meta file> \n";
		cout << "          -o <output file> \n";
		cout <<"=================================================== \n";
		cin.get();
		exit(0);
	}

	//extract the parameters
	string inputFile, metaFile, outputFile;
	
	for(int i=1; i < argc; i++)
	{
		if(i+1 > argc)
		{
			break;
		}
		string aArgument = argv[i];
		if(aArgument == "-i")
		{
			inputFile = argv[++i];
		}
		else if(aArgument == "-m")
		{
			metaFile = argv[++i];	
		}
		else if(aArgument == "-o")
		{
			outputFile = argv[++i];
		}
	}
	
	// check all the arguments are populated, if not exit
	if(inputFile.empty() ||
	   metaFile.empty()  ||
	   outputFile.empty() )
	{
		cout << "Required arguments are not populated, Please try again.\n";
		exit(0);
	}

	try{	
		// Actual processing starts here 
		FileConverter aFileConverter(inputFile, metaFile, outputFile);
		
		// validate the input file and meta file exist and output file don't exist
		if(!aFileConverter.isFilesValid())
		{
			cout << "Invalid files \n";
			exit(0);
		}
		
		// read the meta data
		if(aFileConverter.parseAndStoreMetaData())
		{
			cout << "Meta data file is read \n";		
		}
		
		// Process the input file and write to output file
		if(aFileConverter.processInputFile())
		{
			cout << "OK: Successfully converted " << inputFile << " to " << outputFile << "\n";
		}
		
		exit(0);
	}
	catch(const FFCException& ex)
	{
		cout << "KO: Exception in file processing. " << ex.getErrorString() << endl;
	}
	catch(...)
	{
		cout << "KO: Unknown exception in file processing." << endl;
	}
	exit(0);
}
