#include "windows.h"
#include <string>
#include <iostream>

using namespace std;

void main() {
	HANDLE defaultHeapId;
	char *heapAllocated;
	SIZE_T lastSize = 0;
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
			lastSize += 1024*1024*64;
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			isOverfilled = true;
		}
		iter++;
	}
	cout << "Max Size: " << lastSize / pow(1024, 2) << " Mb" << endl << "Iters: " << iter << endl;
	system("pause");
	isOverfilled = false;
	DWORD heapSize = 0x100000;
	while (!isOverfilled) {
		__try {
			heapSize += 0x100000;
			HANDLE dynamicHeap = HeapCreate(HEAP_GENERATE_EXCEPTIONS, heapSize, 0);
			if (dynamicHeap != NULL) {
				system("cls");
				cout << "-- DYNAMIC HEAP --" << endl;
				cout << "SIZE: " << heapSize / pow(1024, 2) << " Mb" << endl;
				HeapDestroy(dynamicHeap);
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			isOverfilled = true;
		}
	}
	cout << "Max Size: " << heapSize / pow(1024, 2) << " Mb" << endl;
	system("pause");
}