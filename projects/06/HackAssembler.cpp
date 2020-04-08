#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstdint>
#include <bitset>

namespace HackAssembler {
	// Comp, Dest and Jump tables
	std::map<std::string, int16_t> comp_table {
		// a = 0
		{ "0", 0b0101010 } ,
		{ "1", 0b0111111 } ,
		{ "-1", 0b0111010 } ,
		{ "D", 0b0001100 } ,
		{ "A", 0b0110000 } ,
		{ "!D", 0b0001101 } ,
		{ "!A", 0b0110001 } ,
		{ "-D", 0b0001111 } ,
		{ "-A", 0b0110011 } ,
		{ "D+1", 0b0011111 } ,
		{ "A+1", 0b0110111 } ,
		{ "D-1", 0b0001110 } ,
		{ "A-1", 0b0110010 } ,
		{ "D+A", 0b0000010 } ,
		{ "D-A", 0b0010011 } ,
		{ "A-D", 0b0000111 } ,
		{ "D&A", 0b0000000 } ,
		{ "D|A", 0b0010101 } ,

		// a = 1
		{ "M", 0b1110000 } ,
		{ "!M", 0b1110001 } ,
		{ "-M", 0b1110011 } ,
		{ "M+1", 0b1110111 } ,
		{ "M-1", 0b1110010 } ,
		{ "D+M", 0b1000010 } ,
		{ "D-M", 0b1010011 } ,
		{ "M-D", 0b1000111 } ,
		{ "D&M", 0b1000000 } ,
		{ "D|M", 0b1010101 }
	} ;

	std::map<std::string, int16_t> dest_table {
		{ "", 0b000 } ,
		{ "M", 0b001 } ,
		{ "D", 0b010 } ,
		{ "MD", 0b011 } ,
		{ "A", 0b100 } ,
		{ "AM", 0b101 } ,
		{ "AD", 0b110 } ,
		{ "AMD", 0b111 }
	} ;

	std::map<std::string, int16_t> jump_table {
		{ "", 0b000 } ,
		{ "JGT", 0b001 } ,
		{ "JEQ", 0b010 } ,
		{ "JGE", 0b011 } ,
		{ "JLT", 0b100 } ,
		{ "JNE", 0b101 } ,
		{ "JLE", 0b110 } ,
		{ "JMP", 0b111 }
	} ;

	// Initialize the symbol table before the first_pass
	void initialize (std::map<std::string, int16_t>& symbol_table) {
		symbol_table["SP"] = 0x0 ;
		symbol_table["LCL"] = 0x1 ;
		symbol_table["ARG"] = 0x2 ;
		symbol_table["THIS"] = 0x3 ;
		symbol_table["THAT"] = 0x4 ;

		// Registers R0 - R15
		symbol_table["R0"] = 0x0 ;
		symbol_table["R1"] = 0x1 ;
		symbol_table["R2"] = 0x2 ;
		symbol_table["R3"] = 0x3 ;
		symbol_table["R4"] = 0x4 ;
		symbol_table["R5"] = 0x5 ;
		symbol_table["R6"] = 0x6 ;
		symbol_table["R7"] = 0x7 ;
		symbol_table["R8"] = 0x8 ;
		symbol_table["R9"] = 0x9 ;
		symbol_table["R10"] = 0xA ;
		symbol_table["R11"] = 0xB ;
		symbol_table["R12"] = 0xC ;
		symbol_table["R13"] = 0xD ;
		symbol_table["R14"] = 0xE ;
		symbol_table["R15"] = 0xF ;

		// Peripheral memory map
		symbol_table["SCREEN"] = 0x4000 ;
		symbol_table["KBD"] = 0x6000 ;

		return ;
	}

	// Removes all whitespace
	std::string strip (std::string line) {
		line.erase(std::remove_if(line.begin(), line.end(), [] (char ch) { return std::isspace(ch, std::locale()) ; }), line.end());
		return line ;
	}

	// Build the whole symbol table
	void first_pass (std::vector<std::string> program, std::map<std::string, int16_t>& symbol_table) {
		int count { 0 } ;
		for (auto& line : program) {
			// Removes all whitespace
			line = HackAssembler::strip(line) ;
			if (line.size() == 0) continue ;

			// Only read input of type (XXX) and insert into symbol table
			if (line[0] == '(' && line[line.size() - 1] == ')') {
				symbol_table[line.substr(1, line.size() - 2)] = count ;
				// std::cout << line.substr(1, line.size() - 2) << " : " << count << "\n" ;
			}
			else {
				count++ ;
			}
		}
		return ;
	}

	void second_pass (std::vector<std::string> program, std::map<std::string, int16_t> symbol_table, std::vector<std::bitset<16>>& program_binary) {
		// C instruction
		char equals { '=' } ;
		char semi_colon { ';' } ;
		std::string dest ;
		std::string comp ;
		std::string jump ;

		// A instruction and Label
		int n { 16 } ;

		// Result
		std::bitset<16> result ;

		for (auto& line : program) {
			// Stripping whitespace from lhs and rhs
			line = HackAssembler::strip(line) ;
			if (!line.size()) continue ;
			
			// std::cout << "Pass 2 : " << line << "\n" ;

			// Check if A instruction, C instruction or Label
			// A instruction
			if (line[0] == '@') {
				// Drop @
				// std::cout << "A instruction\n" ;
				line = line.substr(1) ;

				try {
					// Try to convert to integer
					result = std::stoi(line) ;
				}
				catch (...) {
					// Could not convert to integer, therefore, it must be a string
					if (symbol_table.find(line) != symbol_table.end()) {
						result = symbol_table[line] ;
					}
					else {
						// std::cout << "Could not find " << line << "\n" ;
						symbol_table[line] = n ;
						result = n ;
						n++ ;
					}
				}
			}

			// Label
			else if (line[0] == '(' && line[line.size() - 1] == ')') {
				// std::cout << "Label\n" ;
				continue ;
			}

			// C instruction 
			else {
				// std::cout << "C instruction\n" ;

				// If dest is empty, the = is omitted
				if (line.find(equals) == std::string::npos) {
					dest = "" ;

					int semi_colon_pos = line.find(semi_colon) ;
					comp = line.substr(0, semi_colon_pos) ;
					jump = line.substr(semi_colon_pos + 1) ;
				}
				// If jump is empty, the ; is omitted
				else if (line.find(semi_colon) == std::string::npos) {
					jump = "" ;

					int equals_pos = line.find(equals) ;
					dest = line.substr(0, equals_pos) ;
					comp = line.substr(equals_pos + 1) ;
				}
				// All fields are present
				else {
					int equals_pos = line.find(equals) ;
					dest = line.substr(0, equals_pos) ;
					line = line.substr(equals_pos + 1) ;

					int semi_colon_pos = line.find(semi_colon) ;
					comp = line.substr(0, semi_colon_pos) ;
					jump = line.substr(semi_colon_pos + 1) ;
				}

				// std::cout << "Dest : " << dest << "\n" ;
				// std::cout << "Comp : " << comp << "\n" ;
				// std::cout << "Jump : " << jump << "\n" ;
				
				// Binary result
				// 0000 0000 0000 0111
				result = 0b111 ;

				// 0000 0011 1acc cccc
				result <<= 7 ;
				result |= comp_table[comp] ;

				// 0001 11ac cccc cddd
				result <<= 3 ;
				result |= dest_table[dest] ;

				// 111a cccc ccdd djjj
				result <<= 3 ;
				result |= jump_table[jump] ;
			}

			program_binary.push_back(result) ;
			// std::cout << "Result : " << result << "\n\n" ;
		}

		return ;
	}
}

int main(int argc, char* argv[]) {
	if (argc == 0) {
		std::cerr << "No files supplied.\n" ;
		std::exit(0) ;
	}

	std::map<std::string, int16_t> symbol_table ;
	std::vector<std::string> program ;
	std::vector<std::bitset<16>> program_binary ;

	std::string file_name ;
	for (int i {1} ; i < argc ; i++) {
		file_name = argv[i] ;
		// std::cout << "Current file : \"" << file_name << "\"\n" ;
		std::ifstream asm_file { file_name } ;

		if (!asm_file) {
			std::cerr << "Cannot read file : \"" << file_name << "\"\n" ;
			continue ;
		}

		HackAssembler::initialize(symbol_table) ;

		std::string line ;
		int comment ;

		// Read the entire file
		while (!asm_file.eof()) {
			std::getline(asm_file, line) ;
			line = HackAssembler::strip(line) ;
			comment = line.find("//") ;
			// If comment, remove
			if (comment != std::string::npos) {
				line = line.substr(0, comment) ;
			}
			// If not empty, keep
			if (line.size()) {
				program.push_back(line) ;
			}
		}

		// Label resolution
		HackAssembler::first_pass(program, symbol_table) ;

		HackAssembler::second_pass(program, symbol_table, program_binary) ;

		auto extension_pos { file_name.rfind('.') } ;
		std::ofstream hack_file { file_name.substr(0, extension_pos) + ".hack" } ;

		// std::cout << "Hack file contents : \n" ;
		for (auto& line : program_binary) {
			// std::cout << line << "\n" ;
			hack_file << line << "\n" ;
		}

		// Clearing all objects for next iteration
		symbol_table.clear() ;
		program.clear() ;
		program_binary.clear() ;
		asm_file.close() ;
		hack_file.close() ;
	}

	return 0 ;
}