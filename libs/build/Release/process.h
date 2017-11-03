#ifndef PROCESS_H
#define PROCESS_H

#include <list>
#include <string>

struct Classes{ //Should be placed into a sorted linked list or vector.
	public:
		Classes(std::string name, std::string code){
			m_name = name;
			m_code = code;
		}
		std::string m_name;
		std::string m_code;
};

struct JavaClasses{
	JavaClasses(v8::Local<Object> exports);
	void NewInstance(const v8::FunctionCallbackInfo<Value>& args);
	void init(const v8::Handle<Array>& in_name, const v8::Handle<Array>& in_code);
	void GetNameArray(const v8::FunctionCallbackInfo<v8::Value>& args);
	void GetCodeArray(const v8::FunctionCallbackInfo<v8::Value>& args);
	v8::Handle<Array> m_name; v8::Handle<Array> m_code;
};

struct Processor{
public:
	Processor(const char* filename);
	std::string get_file_contents(const char *filename);
	void CreateStructures(std::string inputstring);
	std::list<Classes> classlist;
};

const char* ToCString(const v8::String::Utf8Value& value){
	return *value ? *value: NULL;
}

Handle<Array> ListToV8ArrayName(Processor& in_processor){
	v8::Isolate* isolate = args.GetIsolate();
	v8::HandleScope handle_scope;
	v8::Handle<v8::Array> _array = v8::Array::New(in_processor.classlist.size());
	for(int i = 0; i < classlist.size; i++){
		_array->Set(i, v8::String::NewFromUtf8(isolate, in_processor.classlist.at(i).m_name.c_str())); //Can be made slightly more efficient but this is more readable
	}
	return _array;
}

Handle<Array> ListToV8ArrayCode(Processor& in_processor){
	v8::Isolate* isolate = args.GetIsolate();
	v8::HandleScope handle_scope;
	v8::Handle<v8::Array> _array = v8::Array::New(in_processor.classlist.size());
	for(int i = 0; i < classlist.size; i++){
		_array->Set(i, v8::String::NewFromUtf8(isolate, in_processor.classlist.at(i).m_code.c_str())); //Can be made slightly more efficient but this is more readable
	}
	return _array;
}
#endif