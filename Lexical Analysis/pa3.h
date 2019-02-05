#include <string>
#include <vector>

class stack
{
	struct node 
	{
		node(std::string keyword);
		std::string keyword;
		node* next;
	};

	public:
		stack(int s);
		//void addKeyword(std::string keyword);
		void push(std::string keyword);
		void pop();
		int stackSize();
		std::string get_topKeyword(); 

	private:
		node* head;
		std::vector<std::string> keywords;
};

class wordSorter
{
	public:
		wordSorter(int s);
		void add(std::string token);
		void printTokens();
		void addTo_error(std::string er);

	private:
		std::vector<std::string> identifiers;
		std::vector<std::string> constants;
		std::vector<std::string> operators;
		std::vector<std::string> delimiters;
		std::vector<std::string> errors;
		int bracketCount;
};