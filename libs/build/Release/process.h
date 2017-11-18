#ifndef PROCESS_H
#define PROCESS_H

#include <stdlib.h>
#include <list>
#include <string>
#include "node.h"
#include "v8.h"

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
	JavaClasses(const v8::FunctionCallbackInfo<v8::Value>& exports);
	void NewInstance(const v8::FunctionCallbackInfo<v8::Value>& args);
	void init(const v8::Handle<v8::Array>& in_name, const v8::Handle<v8::Array>& in_code);
	v8::FunctionCallback GetNameArray(const v8::FunctionCallbackInfo<v8::Value>& args);
	v8::FunctionCallback GetCodeArray(const v8::FunctionCallbackInfo<v8::Value>& args);
	v8::Handle<v8::Array> handle_m_name; v8::Handle<v8::Array> handle_m_code;
	v8::Local<v8::Object> m_name; v8::Local<v8::Object> m_code;
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

v8::Handle<v8::Array> ListToV8ArrayName(Processor& in_processor, const v8::FunctionCallbackInfo<v8::Value>& exports){
	v8::Isolate* isolate = exports.GetIsolate();
	std::list<Classes>::iterator t = in_processor.classlist.begin();
	v8::Handle<v8::Array> _array = v8::Array::New(isolate, in_processor.classlist.size());
	//for(int i = 0; i < in_processor.classlist.size(); i++){
	int i = 0;
	for (t; t != in_processor.classlist.end(); t++){
		_array->Set(i, v8::String::NewFromUtf8(isolate, (*t).m_name.c_str())); //Can be made slightly more efficient but this is more readable
		++i;
	}
	return _array;
}

v8::Handle<v8::Array> ListToV8ArrayCode(Processor& in_processor, const v8::FunctionCallbackInfo<v8::Value>& exports){
	v8::Isolate* isolate = exports.GetIsolate();
	std::list<Classes>::iterator t = in_processor.classlist.begin();
	v8::Handle<v8::Array> _array = v8::Array::New(isolate, in_processor.classlist.size());
	for (t; t != in_processor.classlist.end(); t++) {
		int i = 0;
		_array->Set(i, v8::String::NewFromUtf8(isolate, (*t).m_code.c_str())); //Can be made slightly more efficient but this is more readable
		++i;
	}
	return _array;
}
#endif