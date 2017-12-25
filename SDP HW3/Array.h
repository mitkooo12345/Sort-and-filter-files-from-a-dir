#pragma once
#include "MFile.h"
#include <tuple>
#include <iostream>

class Array {
public:
	Array();
	~Array();
public:
	void add(std::tuple<std::string, unsigned int>&);
	int getSize();
	void clear();
	void mergeAndSort(std::string, std::string);

private:
	Array(const Array&);
	Array& operator=(const Array&);

private:
	void insureCapacity();
	void mergeSort(MFile*, int, int);
	void merge(MFile*, int, int, int);
	void sort();

private:
	int capacity;
	int size;
	MFile* fileArray;
};

