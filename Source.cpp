#include<iostream>
#include<vector>
#include<string>
#include<Windows.h>
#include<fstream>
#include<algorithm>

#define SHOW_FINAL_ITEMS 0

#define ADD_ELEMENT(E) elementList.push_back(Element(#E)); \
                       const size_t E = i++;
#define ADD_RECIPE(E1,E2,R) recipeList.push_back(Recipe(elementList[E1], elementList[E2], elementList[R]));
#define ADD_RECIPE_(E1,E2,R,A) recipeList.push_back(Recipe(elementList[E1], elementList[E2], elementList[R],A));

enum class Age {
	DEFAULT, HUMAN
};

inline void setColor(short color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),color);
}

class Element {
public:
	Element();
	Element(std::string name);

	const std::string& getName()const ;
	bool isFinal;
private:
	std::string name;
	
};

class Recipe {
public:
	Recipe();
	Recipe(Element& e1, Element& e2, Element& result, Age age);

	bool satisfy( Element*& e1, Element*& e2, const std::vector<Age>& achieved);

	Element* getResult();
	Element *element1, * element2, * result;
private:
	Age ageRequirement;
	
};


Element::Element()
	:name(), isFinal(0)
{

}

Element::Element(std::string name)
	:name(name), isFinal(0)
{

}

const std::string& Element::getName()const
{
	return name;
}

Recipe::Recipe()
	:element1(nullptr), element2(nullptr), result(nullptr),ageRequirement(Age::DEFAULT)
{

}

Recipe::Recipe(Element& e1, Element& e2, Element& result,Age age = Age::DEFAULT)
	:element1(&e1),element2(&e2),result(&result),ageRequirement(age)
{
}

bool Recipe::satisfy(Element*& e1, Element*& e2,const std::vector<Age>&achieved)
{
	if (!element1 && !element2)
		throw 1;
	bool meetRequirement = false;

	for (auto& i : achieved) {
		if (i == ageRequirement)
			meetRequirement = true;
	}
	if (!meetRequirement) return false;

	if (element1 == e1 && element2 == e2 ||
		element1 == e2 && element2 == e1)
		return true;
	return false;
}

Element* Recipe::getResult()
{
	return result;
}

std::string split(std::string str) {

	for (size_t i = 0; i < str.size(); i++) {
		if (i != 0 && str[i] <= 'Z') {
			str.insert(str.begin() + i++,' ');
		}
	}
	return str;
}

std::string join(std::string str) {

	for (size_t i = 0; i < str.size(); i++) {
		if (str[i] == ' ') {
			str.erase(str.begin() + i--);
		}
	}
	return str;
}

void init(std::vector<Element>& elementList, std::vector<Recipe>& recipeList) {
	if (elementList.size()&&recipeList.size())
		throw "The two std::vectors must be empty!";

	size_t i = 0;
	std::cout << "Loading Elements...\n";
#include"ELEMENTS.txt"

	std::cout << "Loading Recipes...\n";
#include"RECIPES.txt"

}

inline void getLine(std::istream& stream, std::string& var){
	do {
		std::getline(stream, var);
	} while (var.length()==0);
}

bool elementCheck(const std::string& e,Element*& ep,const std::vector<Element*>& eList) {
	for (auto& i : eList) {
		if (i->getName() == e) {
			ep = i;
			return true;
		}
	}
	ep = nullptr;
	return false;
}

void pushAge(std::vector<Age>& achieved,Age age,std::string ageStr, bool log=true) 
{
	for (auto& i : achieved)
		if (age == i)return;

	achieved.push_back(age);
	if(log)
		std::cout << "New age: " << ageStr << std::endl;
}

bool loadProgress(std::ifstream& file, std::vector<Element*>& myElements, std::vector<Element*>& myElementsShown, std::vector<Element>& elements) {
	bool Exit = true, isFinal;
	std::string tempStr;

	while (!file.eof()) {

		file >> tempStr;
		file >> isFinal;

		for (unsigned i = 0; i < elements.size(); i++) {
			if (elements[i].getName() == tempStr) {
				myElements.push_back(&elements[i]);
				if(!isFinal)
					myElementsShown.push_back(&elements[i]);
				Exit = false;
			}
		}
	}
	if (Exit)
		return false;
	return true;
}

void alphabeticalVectorPush(std::vector<Element*>& myElements,Element* element) {
	for (unsigned i = 0; i < myElements.size(); i++) {
		if (element->getName() < myElements[i]->getName()) {
			myElements.insert(myElements.begin() + i,element);
			return;
		}
	}
}

const Recipe& recipe(const std::vector<Recipe>& recipes, const std::vector<const Element*>& myElements) {
	const Element* randomElement = myElements[rand() % myElements.size()];
	return recipes[0];
}

int main() {
	std::vector<Element> elements;
	std::vector<Recipe> allRecipes;
	std::vector<Recipe*> availableRecipes;

	//Initializing all elements and recipes

	init(elements, allRecipes);
	for (unsigned i = 0; i < allRecipes.size();i++) {
		availableRecipes.push_back(& allRecipes[i]);
	}

	std::vector<Element*> myElements, myElementsShown;
	std::vector<Age> achieved;
	std::string element1,element2;
	std::ofstream outputFile("progress.txt",std::ios::app);
	outputFile.close();
	std::ifstream inputFile("progress.txt");

	bool unclockedContainer = false;



	size_t currentPage = 0;
	const size_t pageSize = 25;
	Element* e1,* e2, *r;
	size_t tipTimer = 0;
	const size_t timeToTim = 3;
	bool elementList = true;

	
	//Starting or Loading elements
	if(inputFile.is_open()){

		if (!loadProgress(inputFile, myElements, myElementsShown, elements)) {
			for (unsigned i = 0; i < 4; i++) {
				myElements.push_back(&elements[i]);
				myElementsShown.push_back(&elements[i]);

				if (elements[i].getName() == "Human") {
					pushAge(achieved, Age::HUMAN, "Humanity", false);
				}
			}
		}
	}
	else {
		std::cerr << "Could not read stored progress data!" << std::endl;
		system("pause");
		exit(0);
	}

	//Alphabetical Order
	std::sort(myElementsShown.begin(), myElementsShown.end(), [](const Element* a,const Element* b) {return a->getName() < b->getName(); });

	
	inputFile.close();

	achieved.push_back(Age::DEFAULT);



	//Game loop
	while (true) {

	NEXT_PAGE:
		
		Sleep(150);
		system("cls");
		setColor(7);



		//Display the element list

		if (elementList) {

			

			for (size_t i = 0; i < pageSize; i++) {
				
				if (i + currentPage * pageSize >= myElementsShown.size())
					std::cout << '\n';
				else
					std::cout << "      " << split(myElementsShown[i+currentPage*pageSize]->getName()) << std::endl;
			}

			if (currentPage != 0)
				setColor(7);
			else
				setColor(8);

			
			std::cout << "\n  < ";

			setColor(7);
            std::cout << "Page: " << currentPage + 1 << "/" << (myElementsShown.size()-1) / pageSize + 1;

			if (currentPage != (myElementsShown.size() - 1) / pageSize)
				setColor(7);
			else
				setColor(8);

			std::cout << " >\n";
			setColor(7);
			std::cout << std::endl;

			//Display the ratio elements found / total elements
			std::cout << "Found: " << myElements.size() << "/" << elements.size() << std::endl << std::endl;

			//Page switching:
			GetAsyncKeyState(VK_RETURN);
			GetAsyncKeyState(VK_LEFT);
			GetAsyncKeyState(VK_RIGHT);
			do {
				if (GetAsyncKeyState(VK_LEFT) && currentPage != 0) {
					currentPage--;
					goto NEXT_PAGE;
				}

				else if (GetAsyncKeyState(VK_RIGHT) && currentPage != (myElementsShown.size() - 1) / pageSize) {
					currentPage++;
					goto NEXT_PAGE;
				}

				else if (GetAsyncKeyState(VK_RETURN)) {
					break;
				}

			} while (true);

			elementList = true;
		}


		//Entering the first element
		std::cout << "Type the element 1:";
		setColor(4);
		element1 = "";
		do {
			getLine(std::cin, element1);
		} while (element1 == "");
		if (!elementCheck( join(element1), e1, myElements)) {
			elementList = true;
			continue;
		}
			

		//Entering the second element
		setColor(7);
		std::cout << "Type the element 2:"<<std::endl;
		setColor(4);
		element2 = "";
		do {
			getLine(std::cin, element2);
		} while (element2 == "");
		if (!elementCheck(join(element2), e2, myElements)) {
			elementList = true;
			continue;
		}

		bool gotAnythingNew = false;

		//Get the results
		setColor(7);
		std::endl(std::cout);
		for (auto& i : allRecipes) {

			//Checking if there's a recipe that meets the requirements
			if (i.satisfy(e1, e2, achieved)) {

				bool elementAlreadyAquired = false;
				r = i.getResult();

				for (size_t j = 0; j < myElements.size(); j++) {
					if (r == myElements[j])
						elementAlreadyAquired = true;
				}

				if (!elementAlreadyAquired) {

					std::cout << "New element: ";
					setColor(5);
					std::cout << split(r->getName());
					setColor(7);
					std::cout << "!" << std::endl;
					gotAnythingNew = true;
					elementList = true;

					//	Deleting The Recipe
					for (unsigned j = 0; j < availableRecipes.size(); j++) {
						if(availableRecipes[j]->result==r) {
							availableRecipes.erase(availableRecipes.begin() + j);
						}
					}

					//	Check If one of the elements is final
					e1->isFinal = true;
					e2->isFinal = true;
					r->isFinal = true;
					for (unsigned j = 0; j < availableRecipes.size(); j++) {
						if (availableRecipes[j]->element1 == e1 || availableRecipes[j]->element2 == e1)
						{
							e1->isFinal = false;
						}
						if (availableRecipes[j]->element1 == e2 || availableRecipes[j]->element2 == e2)
						{
							e2->isFinal = false;
						}
						if (availableRecipes[j]->element1 == r || availableRecipes[j]->element2 == r)
						{
							r->isFinal = false;
						}
					}
#if SHOW_FINAL_ITEMS == 0

					//Deleting Final Items
					for (unsigned j = 0; j < myElementsShown.size();j++) {
						bool fin = false;
						if (myElementsShown[j] == e1 && e1->isFinal)
							myElementsShown.erase(myElementsShown.begin() + j);

						else if( myElementsShown[j] == e2 && e2->isFinal)
							myElementsShown.erase(myElementsShown.begin() + j);
					}





					if(!r->isFinal)
#endif

						alphabeticalVectorPush(myElementsShown, r);
					alphabeticalVectorPush(myElements,r);
					
					//Check For Ages:
					if (r->getName() == "Human")
						pushAge(achieved, Age::HUMAN, "Humanity");
				}
				else {
					setColor(8);
					std::cout << "Made " << split(r->getName()) << " again " << std::endl;
					setColor(7);
				}
			}
		}

		if (!gotAnythingNew)
			std::cout << "Nothing new came out" << std::endl;
		std::endl(std::cout);

		//Save data:
		outputFile.open("progress.txt");
		if (outputFile.is_open()) {

			outputFile.clear();

			for (auto& i : myElements)
				outputFile <<'\n'<< i->getName() << " " << i->isFinal;
			
		}
		else {
			std::cerr << "Could not read stored progress data!" << std::endl;
			system("pause");
			exit(0);
		}

		outputFile.close();

		system("pause");
	}

}