#include <string>
#include <vector>

namespace se {
	typedef std::string exploderex;

	class exploder {
		public:
			exploder();
			exploder(std::string string, char sep, bool smc)
				: expstr(string), separator(sep), skipmultichar(smc) {}
			exploder(char sep, bool smc)
				: separator(sep), skipmultichar(smc) {}
			std::vector<std::string> explode();
			void load(std::string string);
		private:
			std::string expstr;
			char separator;
			bool skipmultichar;
	};
}
