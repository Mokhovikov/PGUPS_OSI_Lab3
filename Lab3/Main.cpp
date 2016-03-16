#include "windows.h"
#include <iomanip>
#include <locale>
#include <iostream>
#include <sstream>

using namespace std;

DWORD64 standartHeapFunc();
DWORD64 dynamicHeapFunc();
void strReverse (char*);
string number_fmt(long long n, char sep = '.');

char *str = new char[50];

void main() {
	DWORD64 standartMaxSize = standartHeapFunc();
	DWORD64 dynamicMaxSize = dynamicHeapFunc();
	cout << "-- STANDART HEAP --" << endl;
	cout << "Max Size: " << number_fmt((long long)(standartMaxSize / pow(1024, 1))) << " kB\n\n";
	cout << "\n-- DYNAMIC HEAP --" << endl;
	cout << "Max Size: " << number_fmt((long long)(dynamicMaxSize / pow(1024, 1))) << " kB\n\n";
	system("pause");
}

void strReverse (char* str) {
	const size_t len = strlen(str);
	for (size_t i = 0; i < len / 2; i++) {
		swap(str[i], str[len - i - 1]);
	}
}

DWORD64 standartHeapFunc() {
	HANDLE defaultHeapId;
	char *heapAllocated;
	DWORD64 lastSize = 0;
	bool isOverfilled = false;
	//int *intToFill;
	defaultHeapId = GetProcessHeap();
	int iter = 0;

	while (!isOverfilled) {
		__try {
			heapAllocated = (char*)HeapAlloc(
				defaultHeapId,
				HEAP_GENERATE_EXCEPTIONS,
				lastSize
			);
			/*intToFill = new int[lastSize];
			for (int i = 0; i < lastSize; i++) {
			intToFill[i] = i;
			}
			memcpy_s(heapAllocated, lastSize, intToFill, lastSize);*/
			SIZE_T size = HeapSize(defaultHeapId, 0, heapAllocated);
			system("cls");
			cout << "-- STANDART HEAP --" << endl;
			cout << "SIZE: " << size / pow(1024, 2) << " Mb" << endl;
			HeapFree(defaultHeapId, 0, heapAllocated);
			lastSize += 1024 * 1024 * 128;
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			isOverfilled = true;
		}
		iter++;
	}
	return lastSize;
}

DWORD64 dynamicHeapFunc() {
	char *heapAllocated;
	bool isOverfilled = false;
	DWORD64 heapSize = 0x1000000;
	while (!isOverfilled) {
		__try {
			HANDLE dynamicHeap = HeapCreate(HEAP_GENERATE_EXCEPTIONS, heapSize, 0);
			if (dynamicHeap != NULL) {
				system("cls");
				cout << "-- DYNAMIC HEAP --" << endl;
				cout << "SIZE: " << heapSize / pow(1024, 2) << " Mb" << endl;
				HeapDestroy(dynamicHeap);
				heapSize += 0x1000000;
			}
			else {
				isOverfilled = true;
				heapSize -= 0x1000000;
				HANDLE dynamicHeap = HeapCreate(HEAP_GENERATE_EXCEPTIONS, heapSize, 0);
				heapAllocated = (char*)HeapAlloc(dynamicHeap, HEAP_GENERATE_EXCEPTIONS, heapSize);
				char *_str = "The greatest string I've ever seen!";
				strcpy_s(str, strlen(_str) + 1, _str);
				cout << "BEFORE STRING: " << str << endl;
				strReverse(str);
				__try {
					strcpy_s(heapAllocated, strlen(str) + 1, str);
					if (heapAllocated != NULL) {
						cout << "RESULT STRING: " << heapAllocated << endl << endl;
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER) {
					cout << "strcpy_s excepted: " + GetExceptionCode() << endl << endl;
				}
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			isOverfilled = true;
		}
	}
	return heapSize;
}

string number_fmt(long long n, char sep) {
	stringstream fmt;
	fmt << n;
	string s = fmt.str();
	s.reserve(s.length() + s.length() / 3);
	for (int i = 0, j = 3 - s.length() % 3; i < s.length(); ++i, ++j) {
		if (i != 0 && j % 3 == 0) {
			s.insert(i++, 1, sep);
		}
	}
	return s;
}