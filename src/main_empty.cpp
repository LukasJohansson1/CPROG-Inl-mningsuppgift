/*
  c++_empty
  skriver ut innehållet i strängen 's1' på 'consol out'
*/ 

#include <iostream>
#include <string>
#include <format> //C++20, bortkommentera temporärt om Fel
#include <print>  //C++23, bortkommentera temporärt om Fel

int main(int argc, char* argv[]) {
	std::string s1 = "Hejsan";
	std::cout << s1 << std::endl;

	//Bortkommentera temporärt följande om Fel 
	std::cout << std::format("{}-format\n",s1);// C++20
	std::println("{}-{}!", s1, "println" );    // C++23
	std::print("{}-{}!\n", s1, "print" );      // C++23
	
	return 0;
}