#include <chrono>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include "json.hpp"
using namespace std;
using json = nlohmann::json;

struct arrr {
string value;
arrr* next;
size_t size;
arrr() : value(""), next(nullptr) {}

void push(const string& newValue) {
	cout<<"Meow got called!\n";
	arrr* current = this;
	if (current->size == 0){
	this->size = 1;
	current->value = newValue;
	return;}
	while (current->next!=nullptr) {
	current = current->next;
	}
	current->next = new arrr();
	cout<<"Meow set a new value in push function!"<<newValue<<endl<<endl;
	current->next->value = newValue;
	this->size++;
	return;
}



string pop() {
	if (!this->next) {
	//throw out_of_range("Cannot pop from an empty arrr");
	string poppedValue = this->value;
	this->size = 1;
	this->value = "";
	return poppedValue;
	}
	arrr* current = this;
	while (current->next->next) {
	current = current->next;
	}
	string poppedValue = current->next->value;
	delete current->next;
	current->next = nullptr;
	this->size--;
	return poppedValue;
}

void popAll(){
	while(this->size>1){
		this->pop();
	}
}

void setSize(int sizze) {
if (sizze < 0) return;
if (sizze = 0) {
	cout<< "SET A SIZE TO 0!!!\n\n\n";
this->size = sizze;
this->next = nullptr;
return;
}
this->size = sizze;
arrr* current = this;
for (int index = 1; index < sizze; ++index) {
if (!current->next) {
current->size = sizze;
current->next = new arrr();
current->next->size = sizze;
}
current = current->next;
}
}

arrr* get(size_t index) {
arrr* current = this;
if(index==0){return current;}

if (index>current->size){
	cout<<endl<<index<<endl<<this->size;
	throw out_of_range("OOB!");
}

for (size_t i = 0; index > i; i++) {
if (current->next==nullptr){
//	cout<<index<<" GET ERR size "<<current->size;
	//throw out_of_range("OOB! but less cool");
	return current;
}
current = current->next;
}
return current;  // Return nullptr if index out of bounds
}

void print(){
	cout<<"\nPRINTING ARRAY SIZE "<<this->size<<"\n";
for (int j = 0; j < this->size+1; ++j) {
cout << this->get(j)->value<<" "<<j<<" ";
}
cout<<"\n printed arrr succ! \n";
return;
}

};

struct arrr2d {
arrr* row;
arrr2d* next;
size_t numRows;
size_t numCols;

//arrr2d() : row(nullptr), next(nullptr), numRows(0), numCols(0) {}

void setSize(size_t newRows, size_t newCols) {
numRows = newRows;
numCols = newCols;
arrr2d* current = this;
for (size_t i = 0; i < newRows; ++i) {
current->row = new arrr();
current->row->setSize(newCols);
for (size_t j = 0; j < newCols; ++j) {
current->row->push("");
}
if (i < newRows - 1) {
current->next = new arrr2d();
current = current->next;
}
}
}

arrr2d* getRow(size_t rowIndex) {
arrr2d* current = this;
if (rowIndex >= current->numRows) {
throw out_of_range("Row index out of bounds");
}
for (size_t i = 0; i < rowIndex; ++i) {
current = current->next;
}
return current;
}
/*
void setSized(size_t newRows, size_t newCols) {
// Preserve existing data
arrr2d* current = this;
size_t oldRows = numRows;
size_t oldCols = numCols;

// Resize existing rows
for (size_t i = 0; i < oldRows && i < newRows; ++i) {
if (current->row == nullptr) {
current->row = new arrr();
}
current->row->setSize(newCols);
// If new size is larger, initialize new elements
for (size_t j = oldCols; j < newCols; ++j) {
current->row->get(j + 1)->value = "";
}
if (i < newRows - 1) {
if (current->next == nullptr) {
current->next = new arrr2d();
}
current = current->next;
}
}

// Add new rows if necessary
for (size_t i = oldRows; i < newRows; ++i) {
if (current->row == nullptr) {
current->row = new arrr();
}
current->row->setSize(newCols);
for (size_t j = 0; j < newCols; ++j) {
current->row->get(j + 1)->value = "";
}
if (i < newRows - 1) {
if (current->next == nullptr) {
current->next = new arrr2d();
}
current = current->next;
}
}

// Update dimensions
numRows = newRows;
numCols = newCols;

// Truncate if new size is smaller
if (newRows < oldRows) {
current = this;
for (size_t i = 0; i < newRows - 1; ++i) {
current = current->next;
}
// Delete excess rows
arrr2d* temp;
while (current->next != nullptr) {
temp = current->next;
current->next = temp->next;
//delete temp->row;
//delete temp;
}
}
}
*/

void setSized(size_t newRows, size_t newCols) {
arrr2d* current = this;
size_t currentRow = 0;

while (current != nullptr && currentRow < newRows) {
// Ensure row exists
if (current->row == nullptr) {
current->row = new arrr();
}

// Resize row
current->row->setSize(newCols);

// Ensure all elements exist in the row
for (size_t col = 0; col < newCols; ++col) {
arrr* colElement = current->row->get(col);
if (colElement == nullptr || col >= current->row->size) {
current->row->push("");
}
}

// Move to next row
if (current->next == nullptr && currentRow < newRows - 1) {
current->next = new arrr2d();
}
current = current->next;
currentRow++;
}

// Update dimensions
this->numRows = newRows;
this->numCols = newCols;

// Trim excess rows if new size is smaller
current = this;
for (size_t i = 0; i < newRows - 1 && current->next != nullptr; ++i) {
current = current->next;
}
while (current->next != nullptr) {
arrr2d* temp = current->next;
current->next = temp->next;
delete temp->row;
delete temp;
}
}

void pushRowddd(arrr* newRow) {
if (this == nullptr || newRow == nullptr) return;

arrr2d* current = this;
size_t newCols = (newRow->size > this->numCols) ? newRow->size : this->numCols;

// Resize existing rows if necessary
while (current != nullptr) {
if (current->row == nullptr) current->row = new arrr();
current->row->setSize(newCols);
current->numCols = newCols;
current = current->next;
}

// Add new row
current = this;
while (current->next != nullptr) {
current = current->next;
}
current->next = new arrr2d();
current->next->row = newRow;
current->next->numCols = newCols;
current->next->numRows = ++this->numRows;

cout << "PUSHED ROW ";
newRow->print();
}

void pushRow(arrr* newRow) {
if (this == nullptr || newRow == nullptr) return;

size_t newCols = (newRow->size > this->numCols) ? newRow->size : this->numCols;
cout << " SETTING NEWCOLS TO "<<newCols<<" ";
// Resize existing rows if necessary
arrr2d* current = this;
while (current != nullptr) {
if (current->row == nullptr) current->row = new arrr();
current->row->setSize(newCols);
current->numCols = newCols;
current = current->next;
}

// Add new row
current = this;
while (current->next != nullptr) {
current = current->next;
}
current->next = new arrr2d();
current->next->row = new arrr(); // Create a new arrr, don't use newRow directly
//current->next->row->setSize(newCols);
//current->next->row->size = newCols;
// Copy values from newRow to the new row
for (size_t i = 0; i < newRow->size; ++i) {///////////////////////////////////////
	cout<<"current->next->row->size->"<<current->next->row->size<<" newRow->size->"<<newRow->size<<"value:"<<newRow->get(i)->value<<endl;
current->next->row->push(newRow->get(i)->value);
}

current->next->numCols = newCols;
current->next->numRows = 1; // Each arrr2d represents one row

this->numRows++; // Increment the total number of rows

cout << "PUSHED ROW ";
current->next->row->print();
}

//arrr2d() : row(nullptr), next(nullptr), numRows(0), numCols(0) {}
void pushRowdd(arrr* newRow) {
	try{
		if(!this->next){
		
		}
		cout<<"y "<<this->numCols<<endl<<newRow->size<<endl;
arrr2d* current = this;
if(newRow->size > current->numCols){ //increase size of all to fit arrr
	cout<<"u";
size_t newCols = newRow->size;
while (current->next) {
cout<<"w";
current->row->setSize(newCols);
current->row->print();
current = current->next;
}
cout<<"i";
}

if(newRow->size < this->numCols){ //increase size of newrow to fit arrr2d
cout<<"o";
newRow->setSize(this->numCols);

}

current = this;
size_t newRows = this->numRows+1;
cout<<"p";
if(!current->next){
	cout<<"uwu size 1 array";
current->numRows = newRows;
current->numCols = newRow->size;
}
while (current->next) {
if ((current == nullptr)||(current->next == nullptr)){break;}
cout<<"]";
current->numRows = newRows;
current->numCols = newRow->size;
cout<<"bb";
current = current->next;
}
cout<<"[";
//this->numCols++;
current->next = new arrr2d;
current->next->row = newRow;
current->next->numCols = this->numCols;
current->next->numRows = this->numRows;
cout<<"enddd\n\n";
	} catch (exception& e){ cout<<" Error:"<<e.what()<<"endl";}
cout<<" PUSHED ROW ";
newRow->print();
return;
}

void pushRowd(arrr* newRow) {
if (newRow == nullptr) return;
cout<<"\n PUSHHING ROW \n";
newRow->print();
// Determine the size of the new row
size_t newRowSize = 0;
arrr* srcCurrent = newRow;
while (srcCurrent != nullptr) {
newRowSize++;
srcCurrent = srcCurrent->next;
}

// Resize if necessary
if (newRowSize > numCols) {
setSize(numRows + 1, newRowSize);
} else {
setSize(numRows + 1, numCols);
}

// Find the last row
arrr2d* current = this;
for (size_t i = 0; i < numRows - 1; i++) {
current = current->next;
}

// Copy data to the new row
srcCurrent = newRow;
for (size_t colIndex = 0; colIndex < newRowSize; colIndex++) {
current->row->get(colIndex + 1)->value = srcCurrent->value;
srcCurrent = srcCurrent->next;
}

// Pad with empty strings if necessary
for (size_t colIndex = newRowSize; colIndex < numCols; colIndex++) {
current->row->get(colIndex + 1)->value = "";
}
}

/*
void pushRow(arrr* newRow) {
	// Expand if necessary
	if (newRow->size > numCols) {
		size_t oldSize = numCols;
		numCols = newRow->size;
		
		// Stretch existing rows
		arrr2d* current = this;
		for (size_t i = 0; i < numRows; i++) {
			current->row->setSize(numCols);
			// Pad with empty strings
			for (size_t j = oldSize; j < numCols; j++) {
				current->row->get(j)->value = "";
			}
			current = current->next;
		}
	}

	// Now add the new row
	if (numRows == 0) {
		row = new arrr();
		row->setSize(numCols);
		for (size_t colIndex = 0; colIndex < newRow->size; colIndex++) {
			row->get(colIndex)->value = newRow->get(colIndex)->value;
		}
		// Pad if necessary
		for (size_t colIndex = newRow->size; colIndex < numCols; colIndex++) {
			row->get(colIndex)->value = "";
		}
	} else {
		arrr2d* current = this;
		while (current->next != nullptr) {
			current = current->next;
		}
		current->next = new arrr2d();
		current->next->row = new arrr();
		current->next->row->setSize(numCols);
		for (size_t colIndex = 0; colIndex < newRow->size; colIndex++) {
			current->next->row->get(colIndex)->value = newRow->get(colIndex)->value;
		}
		// Pad if necessary
		for (size_t colIndex = newRow->size; colIndex < numCols; colIndex++) {
			current->next->row->get(colIndex)->value = "";
		}
	}
	numRows++;
}*/

/*
void pushRowdep(arrr* newRow) {
	if (numRows == 0) {
	row = new arrr();
	row->setSize(newRow->size);
	arrr* srcCurrent = newRow;
	size_t colIndex = 0;
	while (srcCurrent != nullptr) {
	row->get(colIndex)->value = srcCurrent->value;
	srcCurrent = srcCurrent->next;
	colIndex++;
	}
	} else {
	arrr2d* current = this;
	while (current->next != nullptr) {
	current = current->next;
	}
	current->next = new arrr2d();
	current->next->row = new arrr();
	current->next->row->setSize(numCols);
	arrr* srcCurrent = newRow;
	size_t colIndex = 0;
	while (srcCurrent != nullptr) {
	current->next->row->get(colIndex)->value = srcCurrent->value;
	srcCurrent = srcCurrent->next;
	colIndex++;
	}
	}
	numRows++;
	/*if (newRow->size > numCols) { //bad idea btw
	numCols = newRow->size;
	}
}

*/

/*
void pushRow(arrr* newRow) {
if (numRows == 0) {
row = new arrr();
row->setSize(numCols > 0 ? numCols : newRow->size);
} else {
arrr2d* current = this;
while (current->next != nullptr) {
current = current->next;
}
current->next = new arrr2d();
current->next->row = new arrr();
current->next->row->setSize(numCols > 0 ? numCols : newRow->size);
}

arrr2d* targetNode = (numRows == 0) ? this : this->next;
arrr* targetRow = targetNode->row;

arrr* srcCurrent = newRow;
size_t colIndex = 0;
while (srcCurrent != nullptr && colIndex < targetRow->size) {
targetRow->get(colIndex)->value = srcCurrent->value;
srcCurrent = srcCurrent->next;
colIndex++;
}

while (colIndex < targetRow->size) {
targetRow->get(colIndex)->value = "";
colIndex++;
}

numRows++;
if (newRow->size > numCols) {
numCols = newRow->size;
}
}
*/

void pushCol(arrr* newCol) {
arrr2d* current = this;
size_t rowIndex = 0;
while (current != nullptr) {
if (rowIndex < newCol->size) {
if (numCols >= current->row->size) {
current->row->push(newCol->get(rowIndex)->value);
} else {
current->row->get(numCols)->value = newCol->get(rowIndex)->value;
}
cout << "size: " << current->row->size << " " << newCol->get(rowIndex)->value << endl;
} else {
if (numCols >= current->row->size) {
current->row->push("");
} else {
current->row->get(numCols)->value = "";
}
cout << "ble " << endl;
}
current = current->next;
rowIndex++;
}
numCols++;
}



string& at(size_t rowIndex, size_t colIndex) {
return getRow(rowIndex)->row->get(colIndex)->value;
}
};

string readFileContentToWstring(const string& filename) { //throws errors omg!!!!!!
	ifstream File(filename, ios::binary);
	if (!File.is_open()) {
	throw runtime_error("Failed to open file: " + filename);
	}
	return string(istreambuf_iterator<char>(File), 
	istreambuf_iterator<char>());
}


arrr2d* tables = new arrr2d;
string name = "";
size_t tuplesLimit = 1000;
namespace fs = filesystem;



void readJSONSchemaIntoStructures(const string& filename) {
	try {
		string jsonContent = readFileContentToWstring(filename);
		json jsonData = json::parse(jsonContent);

		name = jsonData["name"];
		tuplesLimit = jsonData["tuples_limit"];

		const auto& structure = jsonData["structure"];
		size_t maxColumns = 2;  // Start at 2 for table name and primary key

		for (const auto& [tableName, columns] : structure.items()) {
			maxColumns = max(maxColumns, columns.size() + 2);
		}

		tables->setSize(structure.size(), maxColumns);

		size_t tableIndex = 0;
		for (const auto& [tableName, columns] : structure.items()) {
			tables->at(tableIndex, 0) = tableName;
			tables->at(tableIndex, 1) = tableName + "_pk";  // Add primary key column

			for (size_t i = 0; i < columns.size(); ++i) {
				tables->at(tableIndex, i + 2) = columns[i];
			}

			for (size_t i = columns.size() + 2; i < maxColumns; ++i) {
				tables->at(tableIndex, i) = "";
			}

			tableIndex++;
		}
	} catch (const exception& e) {
		throw runtime_error("Error parsing JSON: " + string(e.what()));
	}
}

string tocharints(string& str){
	string result;
	for (char ch:str){
	result+= to_string(static_cast<size_t>(ch));
	}
	return result;
}

void createDatabaseStructure(const string& schemaName, arrr2d* tables, size_t tuplesLimit) {
	fs::path schemaPath = schemaName;
	fs::create_directory(schemaPath);
	
	for (size_t i = 0; i < tables->numRows; ++i) {
		string tableName = tables->at(i, 0);
		fs::path tablePath = schemaPath / tableName;
		fs::create_directory(tablePath);
		
		// Create primary key sequence file
		ofstream pkSequenceFile(tablePath / (tableName + "_pk_sequence.txt"));
		pkSequenceFile << tocharints(tableName) + string(to_string(tuplesLimit).length(), '0');
		pkSequenceFile.close();
		
		// Create lock file
		ofstream lockFile(tablePath / (tableName + "_lock.txt"));
		lockFile << "0";
		lockFile.close();
		
		ofstream csvFile(tablePath / "1.csv");
		if (!csvFile.is_open()) {
			throw runtime_error("Failed to create CSV file for table: " + tableName);
		}
		
		for (size_t j = 1; j < tables->numCols; ++j) {
			csvFile << tables->at(i, j);
			if (j < tables->numCols - 1) csvFile << ",";
		}
		csvFile << "\n";
		csvFile.close();
	}
}

bool acquireTableLock(const fs::path& lockFilePath) {
fstream lockFile(lockFilePath, ios::in | ios::out);
if (!lockFile) {
lockFile.open(lockFilePath, ios::out);
lockFile << "0";
lockFile.close();
lockFile.open(lockFilePath, ios::in | ios::out);
}

string lockStatus;
getline(lockFile, lockStatus);

if (lockStatus == "0") {
lockFile.seekp(0);
lockFile << "1";
lockFile.close();
return true;
}

lockFile.close();
return false;
}


void releaseTableLock(const fs::path& lockFilePath) {
	ofstream lockFile(lockFilePath, ios::out | ios::in);
	lockFile.seekp(0);
	lockFile << "0";
	lockFile.close();
}

void iterate(string& number) {
	int i = number.length() - 1;
	
	while (i >= 0) {
		if (number[i] < '9') {
			number[i]++;
			return;
		} else {
			number[i] = '0';
			i--;
		}
	}
	
	if (i < 0) {
		number = "1" + number;
	}
}

void insertNewRecord(const string& tableName, /*const vector<string>&*/ arrr*& record) {
fs::path tablePath = fs::path(name) / tableName;
	fs::path lockFilePath = tablePath / (tableName + "_lock.txt");
	fs::path pkSequenceFilePath = tablePath / (tableName + "_pk_sequence.txt");
if(!acquireTableLock(lockFilePath)){
	throw runtime_error("CSV file for writing is locked: " + lockFilePath.string());
}

	size_t fileNumber = 1;
	string fileName;

	do {
		fileName = to_string(fileNumber) + ".csv";
		fs::path filePath = tablePath / fileName;
		ifstream countFile(filePath);
		size_t lineCount = count(istreambuf_iterator<char>(countFile), istreambuf_iterator<char>(), '\n');
cout<<"LINE COUNT: "<<lineCount<<endl;
		if (lineCount < tuplesLimit) {
			
			ofstream csvFile(filePath, ios::app);
			if(lineCount==0){
				string firstName = "1.csv";
				fs::path fileFirstPath = tablePath / firstName;
				ifstream firstFile(fileFirstPath);
				string line;
				getline(firstFile,line);
				csvFile << line<<"\n";
				//if (i < record->size - 1) csvFile << ",";
				continue;
			}
			ifstream pkSequenceFile(pkSequenceFilePath);
			string currentPk;
			pkSequenceFile >> currentPk;
			pkSequenceFile.close();

			iterate(currentPk);

			ofstream pkSequenceFileOut(pkSequenceFilePath);
			pkSequenceFileOut << currentPk;
			pkSequenceFileOut.close();

			if (!csvFile.is_open()) {
				releaseTableLock(lockFilePath);
				throw runtime_error("Failed to open CSV file for writing: " + filePath.string());
			}

			csvFile << currentPk << ",";
			
			
			
			for (size_t i = 0; i < record->size; ++i) {
				csvFile << record->get(i)->value;
				if (i < record->size - 1) csvFile << ",";
			}
			csvFile << "\n";
			csvFile.close();

			releaseTableLock(lockFilePath);
			return;
		}

		fileNumber++;
	} while (true);
}

arrr* stringtoArrr(string data){
	istringstream dataStr(data);
	string value;
	arrr* result = new arrr;
	result->setSize(0);
	while(getline(dataStr,value,',')){
	result->push(value);
	}
	return result;
}


void printAllTablesAndValues(const string& schemaName) {
fs::path schemaPath = schemaName;

if (!fs::exists(schemaPath) || !fs::is_directory(schemaPath)) {
cout << "Schema directory not found: " << schemaName << endl;
return;
}

for (const auto& tableEntry : fs::directory_iterator(schemaPath)) {
if (fs::is_directory(tableEntry)) {
string tableName = tableEntry.path().filename().string();
cout << "Table: " << tableName << endl;
cout << "------------------------" << endl;

for (const auto& fileEntry : fs::directory_iterator(tableEntry)) {
if (fs::is_regular_file(fileEntry) && fileEntry.path().extension() == ".csv") {
ifstream csvFile(fileEntry.path());
if (csvFile.is_open()) {
string line;
while (getline(csvFile, line)) {
cout << line << endl;
}
csvFile.close();
cout << endl;
}
}
}
cout << endl;
}
}
}

void trim(string& s) { //kinda mid? idk looks bad
s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch) {
return !isspace(ch);
}));
s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
return !isspace(ch);
}).base(), s.end());
}

bool evaluateWhereClause(const std::string& line, arrr* headers, const std::string& expression) {
std::cout << "Evaluating expression: " << expression << std::endl;

std::istringstream lineStream(line);
std::string value;
arrr values;

while (std::getline(lineStream, value, ',')) {
values.push(value);
}

std::istringstream exprStream(expression);
std::string token;
bool result = true;

while (exprStream >> token) {
std::cout << "Processing token: " << token << std::endl;

if (token == "AND") continue;

std::string column = token.substr(0, token.find('.'));
std::string field = token.substr(token.find('.') + 1);

std::cout << "Column: " << column << ", Field: " << field << std::endl;

std::string op, rightValue;
exprStream >> op >> rightValue;

std::cout << "Operator: " << op << ", Right Value: " << rightValue << std::endl;

if (op != "=") {
std::cout << "Unsupported operator: " << op << std::endl;
return false;  // Only equality is supported
}

int columnIndex = -1;
for (int i = 0; i < headers->size; ++i) {
if (headers->get(i + 1)->value == field) {
columnIndex = i;
break;
}
}

if (columnIndex == -1) {
std::cout << "Column not found: " << field << std::endl;
return false;
}

std::string leftValue = values.get(columnIndex + 1)->value;
std::cout << "Left Value: " << leftValue << std::endl;

if (rightValue[0] == '\'' && rightValue[rightValue.length() - 1] == '\'') {
rightValue = rightValue.substr(1, rightValue.length() - 2);
std::cout << "Right Value (after quote removal): " << rightValue << std::endl;
} else {
int rightColumnIndex = -1;
for (int i = 0; i < headers->size; ++i) {
if (headers->get(i + 1)->value == rightValue.substr(rightValue.find('.') + 1)) {
rightColumnIndex = i;
break;
}
}
if (rightColumnIndex != -1) {
rightValue = values.get(rightColumnIndex + 1)->value;
std::cout << "Right Value (from column): " << rightValue << std::endl;
}
}

result = result && (leftValue == rightValue);
std::cout << "Comparison result: " << (result ? "true" : "false") << std::endl;

if (!result) {
std::cout << "Returning false due to failed comparison" << std::endl;
return false;  // Short-circuit if any condition is false
}
}

std::cout << "Final result: " << (result ? "true" : "false") << std::endl;
return result;
}


arrr2d* select(const string& schemaName, const string& columns, const string& tableNames, const string& whereClause) {
arrr2d* result = new arrr2d();

result->setSize(1, 1);  // Initialize
arrr* columnList = new arrr();
columnList->setSize(0);
istringstream columnStream(columns);
string column;

cout<<"1";
while (getline(columnStream, column, ',')) {
trim(column);
columnList->push(column);
cout<<"pushed col:"<<column<<"; ";
}
	cout<<"\n!COLUMN LIST!\n";
	columnList->print();
	
	result->setSize(columnList->size, 2);
	istringstream tableStream(tableNames);
	string tableName;
	size_t k=0;
	size_t rowIndex = 0;
	
	
	//result->setSize(10, 10); 
while (getline(tableStream, tableName, ',')) {
	trim(tableName);
	fs::path tablePath = fs::path(schemaName) / tableName;

	cout << "Table found: " << tableName << endl;
	if (!fs::exists(tablePath) || !fs::is_directory(tablePath)) {
	cout << "Table not found: " << tableName << endl;
	continue;
	}
	cout<<"3";
	for (const auto& fileEntry : fs::directory_iterator(tablePath)) {
	if (fs::is_regular_file(fileEntry) && fileEntry.path().extension() == ".csv") {
	ifstream csvFile(fileEntry.path());
	if (csvFile.is_open()) {
	string line;
	getline(csvFile, line);  // Read header

	cout<<"GETTING HEAD\n";
	arrr* headers = new arrr();
	headers->setSize(0);
	istringstream headerStream(line);
	string header;
	while (getline(headerStream, header, ',')) {
	headers->push(header);
	} //tbh dc
	headers->print();
	
	cout<<"5";
	
	while (getline(csvFile, line)) {
			
			arrr* resRow = new arrr();
			resRow->setSize(0);
			if (evaluateWhereClause(line, headers, whereClause)) { 
			istringstream lineStream(line);
			string value;
			size_t columnIndex = 0;
			cout<<"6";
			while (getline(lineStream, value, ',')) { //get cols
				cout<<"\n7 got val:"<<value<<"; \n";
			
			for (size_t j = 1; j < columnList->size+1; ++j) {
				cout<<"8";
			//	if ((columnList->get(j)->value.find('.') == string::npos)){break;}
			string requestedTable = columnList->get(j)->value.substr(0, columnList->get(j)->value.find('.'));
			cout<<"S req t"<<requestedTable<<" \t";
			string requestedColumn = columnList->get(j)->value.substr(columnList->get(j)->value.find('.') + 1);
			cout<<"9 req c"<<requestedColumn<<" \n";;
			try{
			if ((requestedTable == tableName) && (headers->get(columnIndex)->value == requestedColumn)) { //headers are wrong
			//result->at(result->numRows - 1, j) = value;
			//result->setSize(result->numRows + 1, result->numCols);
			resRow->push(value);
			cout<<"got a line j was"<<j<<"\n";
			
			}else{resRow->push("");}}
			catch(exception& error){resRow->push("");}
			}


			columnIndex++;
			cout<<"A";
			}
			cout<<"B";
			//result->setSize(result->numCols, rowIndex); //fuck i'm tired, this doesn't work btw
			}
			resRow->print();
			result->pushRow(resRow);
			//result->setSized(result->numRows,result->numCols);
			for (size_t i = 0; i < result->numRows; ++i) {
			result->getRow(i)->row->print();
			cout << "\n" <<result->numRows<<"cols"<<result->numCols;
			}		

			
			cout<<"G NEXT ROW!\n";
			rowIndex++;
			//resRow->popAll();
			delete resRow;
		}
	//delete headers;
	csvFile.close();
	cout<<"C closing csv...\n";
	}
	}
	}
}
cout<<"R";
//delete columnList;
return result;
}
/*
"Pets": ["name", "type"],
"Fruits": ["name", "color", "taste"],
"Cars": ["make", "model", "year", "color"],
"Books": ["title", "author", "genre", "year", "pages"]*/

void initializeDatabase(){ //dummy ah data
arrr* data = new arrr;
data = stringtoArrr("Fido,dog");
	insertNewRecord("Pets",data);
data = stringtoArrr("Fido,dog");
insertNewRecord("Pets", data);
 data = stringtoArrr("Whiskers,cat");
insertNewRecord("Pets", data);
 data = stringtoArrr("Bubbles,fish");
insertNewRecord("Pets", data);
 data = stringtoArrr("Fido,dog");
insertNewRecord("Pets", data);
 data = stringtoArrr("Slither,snake");
insertNewRecord("Pets", data);
 data = stringtoArrr("Whiskers,cat");
insertNewRecord("Pets", data);
 data = stringtoArrr("Hoppy,rabbit");
insertNewRecord("Pets", data);
 data = stringtoArrr("apple,red,sweet");
insertNewRecord("Fruits", data);
 data = stringtoArrr("lemon,yellow,sour");
insertNewRecord("Fruits", data);
	//insertNewRecord("Pets",stringtoArrr("Whiskers,cat")); <-- incorrect
	/*insertNewRecord("Pets", {"Bubbles", "fish"});
	insertNewRecord("Pets", {"Fido", "dog"});
	insertNewRecord("Pets", {"Slither", "snake"});
	insertNewRecord("Pets", {"Whiskers", "cat"});
	insertNewRecord("Pets", {"Hoppy", "rabbit"});

	insertNewRecord("Fruits", {"apple", "red", "sweet"});
	insertNewRecord("Fruits", {"lemon", "yellow", "sour"});
	insertNewRecord("Fruits", {"grape", "purple", "sweet"});
	insertNewRecord("Fruits", {"lime", "green", "sour"});
	insertNewRecord("Fruits", {"orange", "orange", "sweet"});
	insertNewRecord("Fruits", {"banana", "yellow", "sweet"});
	insertNewRecord("Fruits", {"kiwi", "brown", "tart"});
	insertNewRecord("Fruits", {"apple", "green", "tart"});
	insertNewRecord("Fruits", {"grape", "green", "sour"});

	insertNewRecord("Cars", {"Ford", "Mustang", "2022", "red"});
	insertNewRecord("Cars", {"Toyota", "Corolla", "2021", "blue"});
	insertNewRecord("Cars", {"Honda", "Civic", "2023", "white"});
	insertNewRecord("Cars", {"Tesla", "Model3", "2022", "black"});
	insertNewRecord("Cars", {"Chevy", "Malibu", "2020", "silver"});
	insertNewRecord("Cars", {"BMW", "X5", "2023", "green"});
	insertNewRecord("Cars", {"Ford", "Mustang", "1960", "blue"});
	insertNewRecord("Cars", {"Toyota", "Corolla", "2023", "red"});

	insertNewRecord("Books", {"Moby", "Melville", "fiction", "1851", "378"});
	insertNewRecord("Books", {"1984", "Orwell", "scifi", "1949", "328"});
	insertNewRecord("Books", {"Pride", "Austen", "romance", "1813", "432"});
	insertNewRecord("Books", {"Hobbit", "Tolkien", "fantasy", "1937", "310"});
	insertNewRecord("Books", {"Gatsby", "Fitzgerald", "fiction", "1925", "180"});
	insertNewRecord("Books", {"Dune", "Herbert", "scifi", "1965", "412"});
	insertNewRecord("Books", {"Hamlet", "Shakespeare", "drama", "1603", "289"});
	insertNewRecord("Books", {"Mockingbird", "Lee", "fiction", "1960", "281"});
	insertNewRecord("Books", {"1984", "Orwell", "scifi", "1949", "328"});
	insertNewRecord("Books", {"Dune", "Herbert", "scifi", "1965", "412"});*/
	return;
}


int main() {
try {
string choi="";
arrr* arr = new arrr;
cout << endl;
ifstream File("schema.json", ios::binary);
File.open("schema.json");
	if (!File.is_open()) {
	throw runtime_error("Failed to open file: " );
	}
	 readJSONSchemaIntoStructures("schema.json");

cout << tables->numCols;
cout << "Pushed values: ";
for (int i = 0; i < tables->numRows; ++i) {
for (int j = 0; j < tables->numCols; ++j) {
cout << tables->at(i,j) << " "<<i<<"+"<<j<<" ";
}
cout << endl;
}
/*
cout<<"\n Create a new database? Y/N";
getline(cin, choi);
if(choi=="y"){
	createDatabaseStructure(name, tables, tuplesLimit);
}
	else if (choi=="n"){}
	else{cout<<"invalid input"; return -1;}

cout<<"\n Initialize the database with default values? Y/N";
choi="";
getline(cin, choi);
if(choi=="y"){
	
	initializeDatabase();
}
	else if (choi=="n"){}
	else{cout<<"invalid input"; return -1;}
*/
printAllTablesAndValues("scheme1");



while (true) {
string query = "";
cout << "Enter your query (or 'quit' to exit): ";
getline(cin, query);
cout << "\nQUERY:" <<query<<endl<<endl;
if (query == "quit"||query == "exit") {
break;
}
if (query == "b") {
query = "SELECT Pets.name FROM Pets";
}
if (query == "c") {
query = "SELECT Pets.name,Pets.type FROM Pets";
}
if (query.substr(0, 6) == "SELECT") {
size_t fromPos = query.find("FROM");
size_t wherePos = query.find("WHERE"); //must have spaces

if (fromPos == string::npos) {
cout << "Invalid query: Missing FROM clause\n";
continue;
}

string columns = query.substr(7, fromPos - 8);
string tableNames = query.substr(fromPos + 5, (wherePos != string::npos ? wherePos - fromPos - 6 : string::npos));
string whereClause = (wherePos != string::npos ? query.substr(wherePos + 6) : "");

trim(columns);
trim(tableNames);
trim(whereClause);
cout << "\ncols:" <<columns<<endl<< "\ntablens:" <<tableNames<< "\nwclase:" <<whereClause;

arrr2d* result = select(name, columns, tableNames, whereClause);

cout<<"\n Print the result!\n";
cout<<"rows "<<result->numRows<<" cols "<<result->numCols<<"\n\n";
result->setSized(result->numRows,result->numCols);
for (size_t i = 0; i < result->numRows; ++i) {
for (size_t j = 0; j < result->numCols; ++j) {
cout << result->at(i, j) << " ";
}
cout << "\n" <<result->numRows<<"cols"<<result->numCols;
}

} else {
cout << "Invalid query: Must start with SELECT\n";
}
}

delete tables;












cout << "Program completed successfully. No crashes here!" << endl;
return 0;
}
catch (const exception& error) {
cout << "\nError: " << error.what() << endl;
}

}