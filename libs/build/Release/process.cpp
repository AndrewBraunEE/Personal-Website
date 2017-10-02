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

struct Processor{
public:
	std::list<Classes> classlist;
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
		return classlist;
	}

};

const char* ToCString(const v8::String::Utf8Value& value){
	return *value ? *value: NULL;
}


Handle<Array> ListToV8ArrayName(Processor& in_processor){
	HandleScope handle_scope;
	Handle<Array> array = Array::New(in_processor.classlist.size());
	for(int i = 0; i < classlist.size; i++){
		array->Set(i, v8::String::NewFromUtf8(isolate, in_processor.classlist.at(i).m_name.c_str())); //Can be made slightly more efficient but this is more readable
	}
	return array;
}

Handle<Array> ListToV8ArrayCode(Processor& in_processor){
	HandleScope handle_scope;
	Handle<Array> array = Array::New(in_processor.classlist.size());
	for(int i = 0; i < classlist.size; i++){
		array->Set(i, v8::String::NewFromUtf8(isolate, in_processor.classlist.at(i).m_code.c_str())); //Can be made slightly more efficient but this is more readable
	}
	return array;
}

void ProcessTextClasses(const v8::FunctionCallbackInfo<v8::Value>& args){
	v8::Isolate* isolate = args.GetIsolate();
	if(!args[0]->IsString()){
		isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "Argument for process.cpp must be a string")));
		return;
	}
	char* stringptr = ToCString(args[0]->ToString());
	if(stringptr == NULL){
		isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "Error Creating C ptr from V8 type (NULL PTR returned)")));
		return;
	}
	Processor(stringptr) classes;
	Handle<Array> Name = ListToV8ArrayName(classes);
	Handle<Array> Codename = ListToV8ArrayCode(classes);

	args.GetReturnValue().set(Name);
	args.GetReturnValue().set(Codename);
	return;
}

void Initialize(v8::Local<v8::Object> exports){
	NODE_SET_METHOD(exports, "processtextclasses", ProcessTextClasses );
}

NODE_MODULE("ProcessText", Initialize)