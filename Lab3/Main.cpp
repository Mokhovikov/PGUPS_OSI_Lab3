#include "windows.h"
#include "iostream"

using namespace std;

void main()
{
	HANDLE defaultHeapId;
	LPVOID heapAllocated;
	SIZE_T lastSize = 0;
	bool isOverfilled = false;
	defaultHeapId = GetProcessHeap();
	heapAllocated = (char*)HeapAlloc(defaultHeapId, HEAP_ZERO_MEMORY, 4096);
	heapAllocated = (char*)HeapReAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY | HEAP_GENERATE_EXCEPTIONS, heapAllocated, 288000000 * sizeof(heapAllocated));
	cout << HeapSize(defaultHeapId, HEAP_NO_SERIALIZE, heapAllocated)*pow(10, -9);
	int iter = 0;

	while (!isOverfilled) {
		/*if (lastSize == 65050)
		lastSize = 65050;
		heapAllocated = HeapAlloc(defaultHeapId, HEAP_GENERATE_EXCEPTIONS, ++lastSize);*/
		__try {
			heapAllocated = (char*)HeapReAlloc(defaultHeapId, HEAP_ZERO_MEMORY | HEAP_GENERATE_EXCEPTIONS, heapAllocated, sizeof(heapAllocated) + (iter * iter * iter * 128));
			lastSize = HeapSize(defaultHeapId, HEAP_NO_SERIALIZE, heapAllocated);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			isOverfilled = true;
		}
		iter++;
	}
	cout << "Max Size: " << lastSize << endl;
}