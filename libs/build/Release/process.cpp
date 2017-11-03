#include <fstream>
#include <string>
#include <cerrno>
#include "process.h"


using namespace v8;

JavaClasses::JavaClasses(v8::Local<Object> exports){ //We pass in the exports object here in order to give access to v8 objects
	//m_name = in_name; m_code = in_code; 
	v8::Isolate* isolate = exports->GetIsolate();
	v8::Local<FunctionTemplate> tpl = v8::FunctionTemplate::New(isolate, New);	//Prepare constructor template aka create interface to other functions
	tpl->SetClassName(String::NewFromUtf8(isolate, "JavaClasses"));
	tpl->InstanceTemplate()->SetInternalFieldCount(3);

	v8::Local<Object> args = exports;

	//We then have to prototype the functions that should go in our internal field
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetNameArray", GetNameArray);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetCodeArray", GetCodeArray);
	if(args.IsConstructCall()){ //Invoked in JS using new MyObject(..._)
		double value = args[0] -> IsUndefined() ? JavaClasses* obj = new JavaClasses(value);
		obj->Wrap(args.This());
		args.GetReturnValue().Set(args.This());
	}
	else{
		const int argc = 1;
		v8::Local<Value> argv[argc] = {args[0]}; //These are local arguments created
		v8::Local<Context> context = isolate->GetCurrentContext();
		v8::Local<Function> cons = v8::Local<Function>::New(isolate, constructor);
		v8::Local<Object> result = cons->NewInstance(context, argc, argv).ToLocalChecked();
		args.getReturnValue().Set(result);
	}
}
void JavaClasses::NewInstance(const FunctionCallbackInfo<Value>& args){
	v8::Isolate* isolate = args.GetIsolate();
	const unsigned argc = 1;
	v8::Local<Value> argv[argc] = { args[0] };
	Local<Function> cons = Local<Function>::New(isolate, constructor);
	Local<Context> context = isolate->GetCurrentContext();
	Local<Object> instance = cons->NewInstance(context, argc, argv).ToLocalChecked();
	args.GetReturnValue().Set(instance);
}
void JavaClasses::init(const Handle<Array>& in_name, const Handle<Array>& in_code){
	m_name = in_name; m_code = in_code;
}
void JavaClasses::GetNameArray(const v8::FunctionCallbackInfo<v8::Value>& args){
	args.GetReturnValue().set(m_name);
}
void JavaClasses::GetCodeArray(const v8::FunctionCallbackInfo<v8::Value>& args){
	args.GetReturnValue().set(m_code);
}
//End of JavaClasses's Functions

Processor::Processor(const char* filename){
	std::string rawstring = get_file_contents(filename);
	CreateStructures(rawstring);
}
std::string Processor::get_file_contents(const char *filename)
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
void Processor::CreateStructures(std::string inputstring){
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
			if (str_switch) {
				str_to_insert_code += inputstring[i];
			}
			else{
				str_to_insert_name += inputstring[i];
			}
		}
		else if(inputstring[i] == ',' || i == t_size - 1){
			Classes tempclass(str_to_insert_name, str_to_insert_code);
			str_to_insert_name = ""; str_to_insert_code = "";
			classlist.push_back(tempclass);
		}
	}
	return;
}

void ProcessTextClasses(const v8::FunctionCallbackInfo<v8::Value>& args){ //Processes Raw Data, then returns JavaClasses object that can be used to get the array handles
	v8::Isolate* isolate = args.GetIsolate();
	if(!args[0]->IsString()){
		isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "Argument for process.cpp must be a string")));
		return;
	}
	const char* stringptr = ToCString(args[0]->ToString());
	if(stringptr == NULL){
		isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "Error Creating C ptr from V8 type (NULL PTR returned)")));
		return;
	}
	Processor classes(stringptr);
	Handle<Array> Name = ListToV8ArrayName(classes);
	Handle<Array> Codename = ListToV8ArrayCode(classes);

	JavaClasses(args) Java_Classes;
	Java_Classes.init(Name, Codename);

	args.GetReturnValue().Set(Java_Classes);
}

void Initialize(v8::Local<v8::Object> exports){
	NODE_SET_METHOD(exports, "processtextclasses", ProcessTextClasses );
}

NODE_MODULE("ProcessText", Initialize)//Sets up Entry Point