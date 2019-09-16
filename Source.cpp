#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

// format string to lowercase (utility)
void lowercase(string& s) {
	for (size_t i = 0; i < s.size(); i++) {
		s[i] = tolower(s[i]);
	}
}

// extract words from line into vector (utility)
void extractWords(stringstream& ss, vector<string>& words) {
	// only taking words with size > 3
	string word;
	while (ss >> word) {
		if (word.size() > 3) {
			lowercase(word);
			words.push_back(word);
		}
	}
}

// drug class, contains information regarding drug
class Drug {
	friend ostream& operator<<(ostream& os, const Drug& aDrug);
	friend void addToFile(const vector<Drug>& drugs, string fileName);

public:
// constructor used when entering new drug info
Drug(string drugName, bool ctrled) : controlled(ctrled) {
	lowercase(drugName);
	genericName = drugName;
	size_t n = 0;
	string items;
	cout << "Enter number of brands: ";
	cin >> n;
	cout << "Enter brands, hit enter after each:" << endl;
	getline(cin, items); // get \n char
	while (n--) {
		getline(cin, items);
		lowercase(items);
		brands.push_back(items);
	}
	cout << "Enter number of indications: ";
	cin >> n;
	cout << "Enter indications, hit enter after each:" << endl;
	string info;
	getline(cin, info);
	while (n--) {
		getline(cin, info);
		lowercase(info);
		indications.push_back(info);
	}
}
// constructor used while loading drug info
Drug(string drugName) : controlled(false) {
	lowercase(drugName);
	genericName = drugName;
}

void addBrand(string& newBrand) {
	lowercase(newBrand);
	brands.push_back(newBrand);
}
void addUse(string& newUse) {
	lowercase(newUse);
	indications.push_back(newUse);
}
bool inquiry(string& in, int opCode) {
	bool out = false;
	if (opCode == 0) out = brandInquiry(in);
	else if (opCode == 1) out = useInquiry(in);
	return out;
}
void changeCtrled(bool ctrl) { controlled = ctrl; }

const string& getGeneric() const {
	return genericName;
}
const vector<string>& getBrands() const { return brands; }
int nOfBrands() const { return brands.size(); }
const vector<string>& getIndications() const { return indications; }
int nOfUses() const { return indications.size(); }
bool getControlled() const { return controlled; }

private:
	string genericName;
	vector<string> brands;
	vector<string> indications;
	bool controlled;

	bool brandInquiry(string& aBrand) {
		lowercase(aBrand);
		if (find(brands.begin(), brands.end(), aBrand) != brands.end()) {
			return true;
		}
		return false;
	}
	bool useInquiry(string& use) {
		stringstream ss(use);
		vector<string> keywords;
		size_t i = 0, j = 0;
		bool found = false;
		extractWords(ss, keywords);
		while (i < indications.size() && !found) {
			if (indications[i].find(keywords[j]) != string::npos) {
				for (size_t k = 0; k < keywords.size(); ++k) {
					if (indications[i].find(keywords[k]) == string::npos) {
						break;
					}
					else found = true;
				}
			}
			if (found) return true;
			if (j < keywords.size() - 1) ++j;
			else { j = 0; ++i; }
		}
		return false;
	}
};


// operator overload << for Drug class, prints drug info
ostream& operator<<(ostream& os, const Drug& aDrug) {
	os << "Generic: " << aDrug.genericName << endl;
	os << "Brands: ";
	for (size_t i = 0; i < aDrug.brands.size(); ++i) {
		if (i + 1 < aDrug.brands.size()) {
			os << aDrug.brands[i] << ", ";
		}
		else os << aDrug.brands[i] << endl;
	}
	os << "Indications: " << endl;
	for (size_t i = 1; i < aDrug.indications.size() + 1; ++i) {
		os << i << ". " << aDrug.indications[i - 1] << endl;
	}
	os << "Controlled Substance: ";
	if (!aDrug.controlled) os << "NO\n------------------" << endl;
	else os << "YES\n------------------" << endl;
	return os;
}

// append new drug info to end of file
void addToFile(const vector<Drug>& drugs, string fileName) {
	ofstream file;
	file.open(fileName, ios::app);
	while (!file.is_open()) {
		cout << "Error: file name not found. Please enter another file name:";
		cout << endl;
		cin >> fileName;
		file.open(fileName, ios::app);
	}

	for (size_t i = 0; i < drugs.size(); ++i) {
		file << drugs[i].genericName << '\n';
		file << drugs[i].brands.size() << '\n';
		for (size_t j = 0; j < drugs[i].brands.size(); ++j) {
			file << drugs[i].brands[j] << '\n';
		}
		file << drugs[i].indications.size() << '\n';
		for (size_t j = 0; j < drugs[i].indications.size(); ++j) {
			file << drugs[i].indications[j] << '\n';
		}
		if (!drugs[i].controlled) file << "no\n";
		else file << "yes\n";
	}
	file.close();
}

// load drug info from file
void loadDrugInfo(vector<Drug>& drugs, string fileName) {
	// initializing variables
	ifstream file;
	string genericName;
	size_t numOfBrands;
	size_t numOfIndications;
	string brand;
	vector<string> brandNames;
	string indication;
	vector<string> uses;
	string controlled;

	// opening file
	file.open(fileName);
	while (!file.is_open()) {
		cout << "Error: file name not found. Please enter another file name:";
		cout << endl;
		cin >> fileName;
		file.open(fileName);
	}

	// loading data
	while (getline(file, genericName)) {
		Drug aDrug(genericName);
		file >> numOfBrands;
		getline(file, indication); // get \n char
		for (size_t i = 0; i < numOfBrands; ++i) {
			getline(file, brand);
			aDrug.addBrand(brand);
		}
		file >> numOfIndications;
		getline(file, indication); // get \n char
		for (size_t i = 0; i < numOfIndications; ++i) {
			getline(file, indication);
			aDrug.addUse(indication);
		}
		file >> controlled;
		getline(file, indication); // get \n char
		if (controlled == "no") aDrug.changeCtrled(false);
		else aDrug.changeCtrled(true);
		drugs.push_back(aDrug);
	}
	file.close();
}

// main menu
int mainMenu() {
	int option;
	string temp;
	cout << "Main Menu\n------------------" << endl;
	cout << "1. Look Up" << endl;
	cout << "2. Add New Drug(s)" << endl;
	cout << "3. Quiz Mode" << endl;
	cout << "0. QUIT" << endl;
	cin >> option;
	while (option != 0 && option != 1 && option != 2 && option != 3) {
		cout << "Invalid option, try 1, 2, or 3" << endl;
		cin >> option;
	}
	cout << "------------------" << endl;
	getline(cin, temp); // getting rid of \n
	return option;
}
// lookup
void lookup(vector<Drug>& drugs) {
	string s;
	cout << "LOOKUP MODE" << endl;

	cout << "Enter generic/brand name, OR enter an use/indication: ";
	getline(cin, s); 
	lowercase(s);
	for (size_t i = 0; i < drugs.size(); ++i) {
		if (drugs[i].getGeneric() == s ||
			drugs[i].inquiry(s, 0) || 
			drugs[i].inquiry(s, 1)) {
			cout << drugs[i];
			return;
		}
	}
	cout << "Drug not found: try again foo- :D" << endl;
}

// add new drugs
void addNewDrug(vector<Drug>& drugs) {
	string drugName;
	char isCtrled;
	bool ctrled;
	cout << "Enter generic name: ";
	getline(cin, drugName);
	cout << "Is it controlled? (y/n): ";
	cin >> isCtrled;
	if (isCtrled == 'y') {
		ctrled = true;
	}
	else {
		ctrled = false;
	}
	drugs.emplace_back(drugName, ctrled);
}


// utility function for quiz mode 1
int quizzing1(vector<Drug>& drugs, int dI, int& attempts, 
			   int n, int opCode) {
	string answer;
	int tries = 3;
	int totalN = n;
	int nOfCorrectAnswers = 0;
	while (n-- && tries > 0) {
		while (tries) {
			cout << "Tries left: " << tries << endl;
			cout << "answer? ";
			getline(cin, answer);
			if (drugs[dI].inquiry(answer, opCode)) {
				cout << "mmHmm u right!!!" << endl;
				++nOfCorrectAnswers;
				break;
			}
			else {
				cout << "oopsie try again u can do it!" << endl;
				--tries;
			}
			cout << endl;
		}
		++attempts;
		if (nOfCorrectAnswers == totalN) {
			cout << "you got all of them!! ~ sending virtual kisses\n\n";
			break;
		}
		if (tries == 0) {
			cout << "oof you outta tries, practice some more loveeee" << endl;
		}
	}
	return nOfCorrectAnswers;
}
// quiz mode 1 
void quiz1(vector<Drug>& drugs) {
	int n = 0;
	int attempts = 0;
	int nOfCorrectAnswers = 0;
	int tries = 3;
	int drugInd = rand() % drugs.size();
	string answer;
	getline(cin, answer); // get rid of \n

	// test by generic name 
	cout << "HERE'S THE GENERIC NAME: " << drugs[drugInd].getGeneric();
	cout << endl;

	// give brands
	cout << "Gimme da brands yo ->" << endl;
	cout << "(hint: there's " << drugs[drugInd].nOfBrands() << ")" << endl;
	cout << endl;
	n = drugs[drugInd].nOfBrands();
	nOfCorrectAnswers += quizzing1(drugs, drugInd, attempts, n, 0);

	// give indications
	cout << "What this do??" << endl;
	cout << "hint: there are " << drugs[drugInd].nOfUses() << " uses" << endl;
	n = drugs[drugInd].nOfUses();
	nOfCorrectAnswers += quizzing1(drugs, drugInd, attempts, n, 1);

	// is it controlled?
	cout << "is dis boi controlled??" << endl;
	answer = "";
	bool ans = false;
	while (tries--) {
		cout << "Tries left: " << tries + 1 << endl;
		cout << "answer? (y/n) ";
		cin >> answer;
		if (answer == "y") ans = true;
		else ans = false;
		if (ans == drugs[drugInd].getControlled()) {
			cout << "mmHmm u right!!!" << endl;
			++nOfCorrectAnswers;
			break;
		}
		else cout << "oopsie try again u can do it!" << endl;
		cout << endl;
	}
	++attempts;

	cout << "So far you've gotten: " << nOfCorrectAnswers
		<< " out of " << attempts << " questions" << endl;
	cout << "-------------------------\n" << endl;
}
// quiz mode 2 (in-progress)
void quiz2(vector<Drug>& drugs) {}
// quiz
void quiz(vector<Drug>& drugs) {
	int option;
	cout << "How you want to study today love?" << endl;
	cout << "1. Flashcard Mode" << endl;
	cout << "2. Fill-in Mode" << endl;
	cin >> option;

	while (option != 1 && option != 2) {
		cout << "Blerg- that's not one of the options cupcake" << endl;
		cout << "Gimme another one------> ";
		cin >> option;
	}

	if (option == 1) quiz1(drugs);
	else if (option == 2) quiz2(drugs);

}


int main() {
	int option;
	vector<Drug> drugs;
	vector<Drug> newDrugs;
	// loading info
	loadDrugInfo(drugs, "drugInfo.txt");
	
	while (true) {
		// main menu
		option = mainMenu();

		// terminate
		if (option == 0) break;

		// look up drug
		else if (option == 1) lookup(drugs);

		// add new drug
		else if (option == 2) addNewDrug(newDrugs);

		// quiz
		else if (option == 3) quiz(drugs);
	}

	addToFile(newDrugs, "drugInfo.txt");
}