#include <fstream>
#include <string>
#include <cerrno>

struct Classes{ //Should be placed into a sorted linked list or vector.
	public:
		Classes(std::string name, std::string code){
			m_name = name;
			m_code = code;
		}
		std::string m_name;
		std::string m_code;
}


struct RawProcessor{
	Processor(const char* filename){
		std::string& result = get_file_contents(filename);
		size_t pos = result.find("var subjects = ");
		result.substr(pos, 16720);
	}
	std::string get_file_contents(const char *filename)
	{
	  std::ifstream in(filename, std::ios::in | std::ios::binary);
	  if (in)
	  {
	    std::string contents;
	    in.seekg(0, std::ios::end);
	    contents.resize(in.tellg());
	    in.seekg(0, std::ios::beg);
	    in.read(&contents[0], contents.size());
	    in.close();
	    return(contents);
	  }
	  throw(errno);
	}
};

struct Process{
	Processor(const char* filename){
		std::string rawstring = get_file_contents(filename);
		CreateStructures(rawstring);
	}
	std::string get_file_contents(const char *filename)
	{
	  std::ifstream in(filename, std::ios::in | std::ios::binary);
	  if (in)
	  {
	    std::string contents;
	    in.seekg(0, std::ios::end);
	    contents.resize(in.tellg());
	    in.seekg(0, std::ios::beg);
	    in.read(&contents[0], contents.size());
	    in.close();
	    return(contents);
	  }
	  throw(errno);
	}
	void CreateStructures(std::string inputstring){
		size_t t_size = inputstring.size();
		std::list<Classes> classlist;
		unsigned int cnt = 0; 
		unsigned int name_cnt = 0; std::string str_to_insert_name; std::string str_to_insert_code; bool str_switch = false;
		for(size_t i = 0; i < t_size; ++i){
			if(inputstring[i] == '('){
				str_switch = true;
			}
			else if(inputstring[i] == ')'){
				str_switch = false;
			}
			else if(isalpha(inputstring[i])){
				if(str_switch)
					std::string str_to_insert_code += inputstring[i];
				else{
					std::string str_to_insert_name += inputstring[i];
				}
			}
			else if(inputstring[i] == ',' || i == t_size - 1){
				Classes tempclass(str_to_insert_name, str_to_insert_code);
				str_to_insert_name = ""; str_to_insert_code = "";
				classlist.push_back(tempclass);
			}
		}
	}
};

int main(int argc, char* argv[], FILE* out){





}