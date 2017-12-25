#include "Array.h"

Array::Array() {
	size = -1;
	capacity = 0;
	fileArray = new MFile[0];
}

Array::~Array() {
	delete[] fileArray;
}

void Array::insureCapacity() {
	if (size == capacity) {

		capacity = capacity * 2;
		if (capacity == 0) {
			capacity = 1;
		}

		MFile* newArr = new MFile[capacity];
		for (int i = 0; i < size; ++i) {
			newArr[i] = fileArray[i];
		}
		delete[] fileArray;
		fileArray = newArr;
	}
}

void Array::add(std::tuple<std::string, unsigned int>& fileInfo) {
	++size;
	insureCapacity();
	fileArray[size] = MFile(std::get<0>(fileInfo), std::get<1>(fileInfo));
}

void Array::sort() {
	mergeSort(fileArray, 0, size);
}

void Array::mergeSort(MFile* fileArr, int left, int right) {
	if (left < right) {
		int mid = left + (right - left) / 2;

		mergeSort(fileArr, left, mid);
		mergeSort(fileArr, mid + 1, right);

		merge(fileArr, left, mid, right);
	}
}

void Array::merge(MFile* fileArr, int left, int mid, int right) {
	int size1 = mid - left + 1;
	int size2 = right - mid;

	MFile* leftArr = new MFile[size1];
	MFile* rightArr = new MFile[size2];

	for (int i = 0; i < size1; ++i) {
		leftArr[i] = fileArr[left + i];
	}

	for (int i = 0; i < size2; ++i) {
		rightArr[i] = fileArr[mid + i + 1];
	}

	int i = 0;
	int j = 0;
	int k = left;

	while (i < size1 && j < size2) {

		if (leftArr[i].getSize() < rightArr[j].getSize()) {
			fileArr[k] = leftArr[i++];

		} else if (leftArr[i].getSize() == rightArr[j].getSize()) {

			if (leftArr[i].getPath() < rightArr[j].getPath()) {
				fileArr[k] = leftArr[i++];
			} else {
				fileArr[k] = rightArr[j++];
			}

		} else {
			fileArr[k] = rightArr[j++];
		}
		++k;
	}

	while (i < size1) {
		fileArr[k] = leftArr[i];
		++i;
		++k;
	}

	while (j < size2) {
		fileArr[k] = rightArr[j];
		++j;
		++k;
	}

	delete[] rightArr;
	delete[] leftArr;
}

int Array::getSize() {
	return size;
}

void Array::clear() {
	size = -1;
	capacity = 0;
	delete[] fileArray;
	fileArray = new MFile[0];
}

void Array::mergeAndSort(std::string oldFileName, std::string newFileName) {
	sort();

	std::ifstream oldFile(oldFileName);
	std::ofstream newFile(newFileName);

	int i = 0;

	MFile file(oldFile);

	while (i <= size && !oldFile.eof()) {

		if (fileArray[i] < file) {
			newFile << fileArray[i++];

		} else {
			newFile << file;
			file = MFile(oldFile);
		}
	}

	while (i <= size) {
		newFile << fileArray[i++];
	}

	while (!oldFile.eof()) {
		newFile << file;
		file = MFile(oldFile);
	}

	oldFile.close();
	newFile.close();
}