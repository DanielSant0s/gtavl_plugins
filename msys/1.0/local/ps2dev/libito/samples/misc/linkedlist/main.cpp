#include <Ito.h>
#include <stdio.h>

using namespace Ito;

int main()
{
	LinkedList<u32> list;

	for(u32 i=0; i < 10; i++) list.Add(10-i);

	list.Set(4, 999);

	list.Remove(5);

	for(u32 i=0; i < 10; i++) printf("Index %i: %i\n", i, list.Get(i));

	printf("Find 8 at index %i\n", list.IndexOf(8));

	list.Clear();

	printf("IsEmpty? %s\n", list.IsEmpty()?"True":"False");

	return 0;
}
