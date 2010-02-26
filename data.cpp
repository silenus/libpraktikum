#include "data.h"

Data::Data(const unsigned int length, const unsigned int cols) {
	data = new double*[cols];
	min = new double[cols];
	max = new double[cols];
	_mean = new double[cols];
	errorMean = new double[cols];
	rms = new double[cols];
	for (uint i = 0; i < cols; i++) {
		data[i] = new double[length];
	}
	_hasHeader = false;
	data = NULL;
	names = NULL;
	symbols = NULL;
	units = NULL;
}

Data::Data(const string &filename) {
	posData = 0; //pointer in stream to the beginning of the data
	
	////////////////////////////////////////////////////////////
	//make a quick loop through the file to gather information//
	////////////////////////////////////////////////////////////
	if (scanLab(filename, _length, cols, _hasHeader, posData) != 0) {
		cerr << "Could not scan file" << filename << endl;
		_length = 0;
		cols = 0;
		_hasHeader = false;
		data = NULL;
		names = NULL;
		symbols = NULL;
		units = NULL;
		return;
	}
	
	////////////////////////////////////////////
	//Now we know what the file looks like and//
	//can allocate some space for the data    //
	////////////////////////////////////////////
	
	//get an array of pointers to the arrays of data
	data = new double*[cols];
	min = new double[cols];
	max = new double[cols];
	_mean = new double[cols];
	errorMean = new double[cols];
	rms = new double[cols];
	
	//generate the arrays for the data
	for(uint i = 0; i< cols; i++){
		data[i] = new double[_length];
	}
	//allocate space for the column-info
	if (_hasHeader)
	{
		names = new string[cols];
		symbols = new string[cols];
		units = new string[cols];
		readLabHeader(filename, cols, names, symbols, units);
	}
	else
	{
		names = NULL;
		symbols = NULL;
		units = NULL;
	}
	
	// TODO: Prope error handling, maybe with exceptions
	readLabData(filename, _length, cols, data, posData);
}

double Data::getMin(const unsigned int col) {
	static bool isCached = false;
	if (isCached)
		return min[col];
	else
		min[col] = utils::min(data[col], _length);

	isCached = true;
	return min[col];
}

double Data::getMax(const unsigned int col) {
	static bool isCached = false;
	if (isCached)
		return max[col];
	else
		max[col] = utils::max(data[col], _length);

	isCached = true;
	return max[col];
}

double Data::mean(const unsigned int col) {
	static bool isCached = false;

	if (isCached)
		return _mean[col];
	else
		_mean[col] = utils::mean(data[col], _length);

	isCached = true;
	return _mean[col];
}

double Data::mean(const unsigned int col, double &errorMean, double &rms) {
	static bool isCached = false;

	if (isCached) {
		errorMean = this->errorMean[col];
		rms = this->rms[col];
		return _mean[col];
	}
	else
		_mean[col] = utils::mean(data[col], _length, this->errorMean[col], this->rms[col]);

	isCached = true;
	errorMean = this->errorMean[col];
	rms = this->rms[col];
	return _mean[col];
}

double *Data::getValuesBetween(const unsigned int col, const double first, const double second, int &count) {
	unsigned int i, j;
	if (first < second) {
		for (i = 0; data[col][i] < first && i < _length-1; i++);
		for (j = 0; data[col][j] < second && j < _length-1; j++);
	}
	else {
		for (i = 0; data[col][i] > first && i < _length-1; i++);
		for (j = 0; data[col][j] > second && j < _length-1; j++);
	}
	count = j - i + 1;
	return &data[col][i];
}


// private functions
//------------------------------

int Data::scanLab(string filename, unsigned int &length, unsigned int &cols, bool &hasHeader, int &posData) {
	/////////////////////////
	//Define some variables//
	/////////////////////////
	bool eof=false; //eof reached
	
	posData=0; //pointer in stream to the beginning of the data
	
	//we have found nothing until now
	length=0;
	cols=0;
	
	//define string for storing a line of the input file
	string str_line;
	
	///////////////////
	//open input file//
	///////////////////
	ifstream in(filename.c_str());
	if (!in){
		cout << "ERROR opening input file " << filename <<endl;
		return 1;
	}
	
	
	//read first line
	if(in.eof()){
		eof=true;
		str_line=("");
	}
	else{
		getline(in, str_line);
		eof=false;
	}
	
	//look for header of *.lab file 
	if (str_line.find("CL4")!=string::npos){ //<string::npos is a constant meaning "not found"
		hasHeader=true;
		cout << "this is a *.lab file with header" <<endl;
	}
	else{
		hasHeader=false;
	}
	
	
	//fast skip through header (until finding tabstop <=> line with data)
	while(str_line.find("\t")==string::npos){ //<repeat while the last read line does not contain tabstop
		posData=in.tellg(); //saves the postion of the pointer
		if(in.eof()){
			eof=true;
			str_line=("");
			//return error code when not finding any data
			return 2;
		}
		else{
			getline(in, str_line); //< scan next line, will be analized in next repetition of loop
			eof=false;
		}
	}
	
	//we have found first dataset (datasets must not include "NAN")
	if(str_line.find("NAN")==string::npos){
		length++;
	}
	
	
	//count number of columns
	//(count number of tabstops; tabstop after the last data column)
	while(str_line.find("\t")!=string::npos){
		int tabpos=0;
		cols++;
	
		//locate tabstop and replace it so that it is not counted again
		tabpos=str_line.find("\t");
		str_line.replace(tabpos,1," "); //< replace one character with " " (space) at tabpos
	
	}
	cout << "cols=" <<cols <<endl;
	
	
	//Read the next lines until it does not contain any tabstop or
	//till eof; count the lines
	do{
		if(in.eof()){
			eof=true;
			str_line=("");//< fake an empty line
		}
		else{
			getline(in, str_line);
			eof=false;
		}
		if((str_line.find("\t")!=string::npos)&&(str_line.find("NAN")==string::npos)){
			length++;
		}
	
	}
	while(str_line.find("\t")!=string::npos);
	
	cout << "length=" <<length << endl;

	in.close();
	
	return 0;
	
};


/**
 * \brief Reads Header of labfile in already allocated arrays
 * 
 * \warning This might or might not work for a particular labfile
 * 
 * \param[in] filename c++ string with filename of the labfile
 * \param[out] cols number of data-cols (can be determined with scanLab() )
 * \param[out] names pointer to array with cols string elements, the names of the data columns will be put here
 * \param[out] symbol pointer to array with cols string elements, an array with the symbols of the data coluns used in cassy lab will be put here
 * \param[out] unit pointer to array with cols string elements, array with units for the data in each column will be given back here
 * 
 * \returns 0 Data read successfully \n
 * 		1 Could not open input file
 */

int Data::readLabHeader(string filename, unsigned int cols, string* names, string* symbol, string* unit){
	
	//define string for storing a line of the input file
	string str_line;
	
	///////////////////
	//open input file//
	///////////////////
	ifstream in(filename.c_str());
	if (!in){
		cout << "ERROR opening input file " << filename <<endl;
		return 1;
	}
	//skip the first line 
	getline(in, str_line);
	
	for(uint i=0; i<cols; i++){
		do{
			getline(in, str_line);
		}
		while(str_line.find_first_of("qwertzuiopasdfghjklyxcvbnmQWERTZUIOPASDFGHJKLYXCVBNM")==string::npos); //< search for any letter
		
		//The field for "Frequenz" seems to be totally useless, ignore it
		if(str_line.find("Frequenz")!=string::npos){
			getline(in, str_line);
			getline(in, str_line);
			do{
				getline(in, str_line);
			}
			while(str_line.find_first_of("qwertzuiopasdfghjklyxcvbnmQWERTZUIOPASDFGHJKLYXCVBNM")==string::npos);
			
		}
		names[i]=str_line;

		getline(in, str_line);
		symbol[i]=str_line;
		
		getline(in, str_line);
		unit[i]=str_line;
		

	}
	in.close();	
	return 0;
};

/**
 * \brief Reads data from labfiles generated by cassy lab
 * 
 * \param[in] filename c++ string with filename of the labfile
 * \param[out] &length numbers of found dataset (might be changed if a line turns out to not contain data)
 * \param[out] cols numbers of data-cols 
 * \param[out] data pointer to pointer to double, two dimensional array for data has to be allocated (data[cols][length])
 * \param[in] posData position of the data in the data stream
 * 
 * \returns 0 Data read successfully \n
 * 		1 Could not open input file
 */

int Data::readLabData(string filename, unsigned int &length, unsigned int cols, double** data, int posData){
	
	int linebeg;
	string str_line;

	ifstream in(filename.c_str());
	if (!in){
		cout << "ERROR opening input file " << filename <<endl;
		return 1;
	}
		
	in.seekg(posData);
		
	cout << "starting data extraction" << endl;

	for (uint i=0; i< length; i++){
		
		//first look at the line, if it's OK, go to the beginning of the line and extract data
		linebeg=in.tellg();
		getline(in, str_line);
		if(str_line.find("NAN")==string::npos){
			in.seekg(linebeg);
			for (uint j=0; j< cols; j++){
				in >> data[j][i];
			}
		}
		else{
			i=i-1;		
		}
	}
	in.close();	
	return 0;
}
