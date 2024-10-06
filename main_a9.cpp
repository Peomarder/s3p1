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
size_t size = 0;
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

void popAll(){
	while(this->size>0){
		this->pop();
	}
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
if(index==0){return current;}
if (index>current->size){
	cout<<index;
	throw out_of_range("OOB!");
}
for (size_t i = 0; index > i; i++) {
if (current->next==nullptr){
	cout<<index<<" size "<<current->size;
	//throw out_of_range("OOB! but less cool");
	return current;
}
current = current->next;
}
return current;  // Return nullptr if index out of bounds
}

void print(){
for (int j = 0; j < this->size+1; ++j) {
cout << this->get(j)->value<<" "<<j<<" s:"<<this->size;
}
cout<<" printed arrr succ! \n";
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
arrr2d* current = this;
if (rowIndex >= current->numRows) {
throw out_of_range("Row index out of bounds");
}
for (size_t i = 0; i < rowIndex; ++i) {
current = current->next;
}
return current->row;
}

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
}


void pushRowdep2(arrr* newRow) {
	if (numRows == 0) {
		row = new arrr();
		row->setSize(newRow->size);
		for (size_t colIndex = 0; colIndex < newRow->size; colIndex++) {
			row->get(colIndex)->value = newRow->get(colIndex)->value;
		}
	} else {
		arrr2d* current = this;
		for (size_t i = 1; i < numRows; i++) {
			current = current->next;
		}
		current->next = new arrr2d();
		current->next->row = new arrr();
		current->next->row->setSize(numCols);
		for (size_t colIndex = 0; colIndex < newRow->size && colIndex < numCols; colIndex++) {
			current->next->row->get(colIndex)->value = newRow->get(colIndex)->value;
		}
	}
	numRows++;
}


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
	}*/
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

void trim(string& s) { //kinda mid? idk looks bad
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
arrr* resRow = new arrr();
resRow->setSize(0);
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
cout<<"2";
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
		if ((requestedTable == tableName) && (headers->get(columnIndex+1)->value == requestedColumn)) { //headers are wrong
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
		result->pushRow(resRow);
		cout<<"G NEXT ROW!\n";
		rowIndex++;
		resRow->popAll();
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
/*
"Pets": ["name", "type"],
"Fruits": ["name", "color", "taste"],
"Cars": ["make", "model", "year", "color"],
"Books": ["title", "author", "genre", "year", "pages"]*/

void initializeDatabase(){ //dummy ah data
	insertNewRecord("Pets",{"Fido","dog"});
	insertNewRecord("Pets",{"Whiskers","cat"});
	insertNewRecord("Pets", {"Bubbles", "fish"});
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
	insertNewRecord("Books", {"Dune", "Herbert", "scifi", "1965", "412"});
	return;
}


int main() {
try {
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
createDatabaseStructure(name, tables, tuplesLimit);
 insertNewRecord("a1",{"ass","bitch","cock","dick"});
 insertNewRecord("a1",{"1","2","3","4"});
 insertNewRecord("b2",{"a","b","c","d"});
 insertNewRecord("c3",{"ass","bitch","cock","dick"});
 insertNewRecord("c3",{"1","2","3","4"});
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