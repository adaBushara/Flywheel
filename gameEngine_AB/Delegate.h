#pragma once
#include <utility>

template<typename C>
class Delegate
{
	//this is a void pointer that can be used to store any instance
	typedef void* InstancePtr;
	//void pointer to a function pointer
	//this lets us to hold on to any type of function pointer
	typedef void(*InternalFunction)(InstancePtr, T);

	InstancePtr instancePtr;
	InternalFunction internalFunction;

	template<void(*Function)(T)>
	static void freeFunctionWrapper(InstancePtr, T arg)
	{
		(Function)(arg);
	}

	template<typename C, void(C::*Function)(T)>
	static void instanceFunctionWrapper(InstancePtr instance, T arg)
	{
		(static_cast<C*>(instance)->*Function)(arg);
	}

public:

	Delegate(void) : instancePtr(nullptr), internalFunction(nullptr) {};

	//bind a free function
	template<void(*Function)(T)>
	void bind(void)
	{
		//instance pointer is null
		//as there is no instance in free functions
		instancePtr = nullptr;
		internalFunction = &freeFunctionWrapper<Function>;
	}

	//bind an internal function
	template<typename C, void(C::*Function)(T)>
	void bind(C* instance)
	{
		instancePtr = instance;
		internalFunction = &instanceFunctionWrapper<C, Function>;
	}

	void invoke(T arg)
	{
		InternalFunction(instancePtr, arg);
	}

	bool operator==(const Delegate& other)
	{
		return instancePtr == other.instancePtr && internalFunction == other.internalFunction;
	}
};
