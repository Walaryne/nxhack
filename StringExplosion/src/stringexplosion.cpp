#include <string>
#include <vector>
#include <iostream>
#include "stringexplosion.hpp"

namespace se {

	std::vector<std::string> exploder::explode() {

		if(expstr.empty() || expstr == "\n") {
			throw exploderex("Constructor not called with string or no string loaded");
		}

		if(expstr.find(" ") == std::string::npos) {
			return std::vector<std::string> {expstr};
		}

		int strlen = expstr.size();
		std::vector<int> clocsv; // Character locations vector
		int iteration = 0, skipcount = 0;
		bool skipflag = false;

		for(char c : expstr) {
			if(c == separator) {
				if(skipmultichar) {
					if(!skipflag) {
						clocsv.push_back(iteration);
						skipflag = true;
					} else {
						skipcount++;
					}
				} else {
					clocsv.push_back(iteration);
				}
			} else {
				if(skipmultichar) {
					skipflag = false;
					if(skipcount > 0) {
						clocsv.back() += skipcount;
						skipcount = 0;
					}
				}
			}
			iteration++;
		}

		int start = 0, count = 0;
		int size = clocsv.size();
		std::vector<std::string> exploded;

		for(int i = 0; i <= size; i++) {
			if(i == 0) {
				start = 0;
				count = clocsv[i];
			} else {
				start = clocsv[i - 1] + 1; //Start is the previous separator's pos plus one
				count = clocsv[i] - start; //Count is the current separator's pos minus the starting pos
			}
			exploded.push_back(expstr.substr(start, count));
		}

		return exploded;
	}

	void exploder::load(std::string string) {
		expstr = string;
	}
}
