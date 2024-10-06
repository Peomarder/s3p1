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
	arrr* current = this;
	while (current->next!=nullptr) {
	current = current->next;
	}
	current->next = new arrr();
	current->next->value = newValue;
	this->size++;
}

string pop() {
	if (!this->next) {
	throw out_of_range("Cannot pop from an empty arrr");
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


void setSize(int size) {
if (size <= 0) return;
this->size = size;
arrr* current = this;
for (int index = 1; index < size; ++index) {
if (!current->next) {
current->next = new arrr();
}
current = current->next;
}
}

arrr* get(size_t index) {
arrr* current = this;
if (index>=current->size){
	cout<<index;
	throw out_of_range("OOB!");
}
for (size_t i = 0; index > i; i++) {
current = current->next;
}
return current;  // Return nullptr if index out of bounds
}

void print(){
for (int j = 0; j < this->size; ++j) {
cout << this->get(j)->value <<j<<" ";
}
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

arrr* getRow(size_t rowIndex) {
if (rowIndex >= numRows) {
throw out_of_range("Row index out of bounds");
}
arrr2d* current = this;
for (size_t i = 0; i < rowIndex; ++i) {
current = current->next;
}
return current->row;
}


void pushRow(arrr* newRow) {
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
	current->next->row->setSize(newRow->size);
	arrr* srcCurrent = newRow;
	size_t colIndex = 0;
	while (srcCurrent != nullptr) {
	current->next->row->get(colIndex)->value = srcCurrent->value;
	srcCurrent = srcCurrent->next;
	colIndex++;
	}
	}
	numRows++;
	if (newRow->size > numCols) {
	numCols = newRow->size;
	}
}


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
return getRow(rowIndex)->get(colIndex)->value;
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
		nlohmann::json jsonData = nlohmann::json::parse(jsonContent);

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
	result+= to_string(static_cast<int>(ch));
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

void insertNewRecord(const string& tableName, const vector<string>& record) {
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

		if (lineCount < tuplesLimit) {
			ifstream pkSequenceFile(pkSequenceFilePath);
			int currentPk;
			pkSequenceFile >> currentPk;
			pkSequenceFile.close();

			currentPk++;

			ofstream pkSequenceFileOut(pkSequenceFilePath);
			pkSequenceFileOut << currentPk;
			pkSequenceFileOut.close();

			ofstream csvFile(filePath, ios::app);
			if (!csvFile.is_open()) {
				releaseTableLock(lockFilePath);
				throw runtime_error("Failed to open CSV file for writing: " + filePath.string());
			}

			csvFile << currentPk << ",";

			for (size_t i = 0; i < record.size(); ++i) {
				csvFile << record[i];
				if (i < record.size() - 1) csvFile << ",";
			}
			csvFile << "\n";
			csvFile.close();

			releaseTableLock(lockFilePath);
			return;
		}

		fileNumber++;
	} while (true);
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

void trim(string& s) {
s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch) {
return !isspace(ch);
}));
s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
return !isspace(ch);
}).base(), s.end());
}

bool evaluateCondition(const string& leftOperand, const string& operand, const string& rightOperand) {
	if (operand == "=") {
		return leftOperand == rightOperand;
	}
	return false;
}

bool where(arrr2d* tables, const string& expression) {
	arrr* tokens = new arrr();
	string token;
	for (size_t i = 0; i < expression.length(); ++i) {
		if (expression[i] == ' ') {
			if (!token.empty()) {
				tokens->push(token);
				token.clear();
			}
		} else {
			token += expression[i];
		}
	}
	if (!token.empty()) {
		tokens->push(token);
	}

	bool result = true;
	for (size_t i = 0; i < tokens->size; i += 4) {
		string tableName = tokens->get(i)->value.substr(0, tokens->get(i)->value.find('.'));
		string columnName = tokens->get(i)->value.substr(tokens->get(i)->value.find('.') + 1);
		string operand = tokens->get(i + 1)->value;
		string value = tokens->get(i + 2)->value;

		bool conditionResult = evaluateCondition(tables->at(0, 0), operand, value);

		if (i + 3 < tokens->size) {
			string logicalOp = tokens->get(i + 3)->value;
			if (logicalOp == "AND") {
				result = result && conditionResult;
			} else if (logicalOp == "OR") {
				result = result || conditionResult;
			}
		} else {
			result = result && conditionResult;
		}
	}

	delete tokens;
	return result;
}


arrr2d* select(const string& schemaName, const string& columns, const string& tableNames, const string& whereClause) {
arrr2d* result = new arrr2d();
result->setSize(1, 1);  // Initialize
arrr* columnList = new arrr();
istringstream columnStream(columns);
string column;

cout<<"1";
while (getline(columnStream, column, ',')) {
trim(column);
columnList->push(column);
}
columnList->print();
istringstream tableStream(tableNames);
string tableName;
cout<<"2";
while (getline(tableStream, tableName, ',')) {
trim(tableName);
fs::path tablePath = fs::path(schemaName) / tableName;

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

arrr* headers = new arrr();
istringstream headerStream(line);
string header;
cout<<"4";
while (getline(headerStream, header, ',')) {
headers->push(header);
headers->print();
}
cout<<"5";
while (getline(csvFile, line)) {
if (whereClause.empty() /*|| evaluateWhereClause(line, headers, whereClause)*/) { 
istringstream lineStream(line);
string value;
size_t columnIndex = 0;
cout<<"6";
while (getline(lineStream, value, ',')) {
	cout<<"7";
for (size_t j = 0; j < columnList->size; ++j) {
	cout<<"8";
//	if ((columnList->get(j)->value.find('.') == string::npos)){break;}
string requestedTable = columnList->get(j)->value.substr(0, columnList->get(j)->value.find('.'));
cout<<"S";
string requestedColumn = columnList->get(j)->value.substr(columnList->get(j)->value.find('.') + 1);
cout<<"9";
//fuck i'm tired, this doesn't work btw
if ((requestedTable == tableName) && (headers->get(columnIndex)->value == requestedColumn)) { //headers are wrong
result->at(result->numRows - 1, j) = value;
cout<<"10";
}
}


columnIndex++;
cout<<"A";
}
cout<<"B";
result->setSize(result->numRows + 1, columnList->size);
}
cout<<"got a line";
}
csvFile.close();
cout<<"C";
}
}
}
}
cout<<"R";
//delete columnList;
return result;
}


int main() {
try {
arrr* arr = new arrr;
/*
arr->setSize(1);
arr->push("Wibble");
arr->push("Wobble");
arr->push("Wubble");
arr->push("Bubble");
arr->get(0)->value = "Bibble";

cout << "Pushed values: ";
for (int i = 0; i < arr->size; ++i) {
cout << arr->get(i)->value << " "<<i<<" ";

}

arrr2d* arr2d = new arrr2d;
arr2d->setSize(4,4);
cout<<"\npush";
arr2d->pushRow(arr);
cout<<"push";
arr2d->pushCol(arr);
arr2d->pushCol(arr);


cout << "Pushed values: ";
for (int i = 1; i < arr2d->numRows; ++i) {
for (int j = 1; j < arr2d->numCols; ++j) {
cout << arr2d->at(i,j) << " "<<i<<"+"<<j<<" ";
}
cout << endl;
}*/

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
createDatabaseStructure(name, tables, tuplesLimit);
 insertNewRecord("a1",{"a","b","c","d"});
 insertNewRecord("a1",{"1","2","3","4"});
 insertNewRecord("b2",{"a","b","c","d"});
printAllTablesAndValues("scheme1");



while (true) {
string query = "";
cout << "Enter your query (or 'quit' to exit): ";
getline(cin, query);
cout << "\nQUERY:" <<query<<endl<<endl;
if (query == "quit") {
break;
}

if (query.substr(0, 6) == "SELECT") {
size_t fromPos = query.find("FROM");
size_t wherePos = query.find("WHERE");

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

// Print the result
for (size_t i = 0; i < result->numRows; ++i) {
for (size_t j = 0; j < result->numCols; ++j) {
cout << result->at(i, j) << "	";
}
cout << "\n";
}

delete result;
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