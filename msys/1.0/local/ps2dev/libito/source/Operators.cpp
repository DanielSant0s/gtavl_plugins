#include <tamtypes.h>
#include <malloc.h>

#ifdef F_Operators_new
__attribute__((weak))
void* operator new[](size_t size) 
{ 
	 return malloc(size); 
}
#endif

#ifdef F_Operators_new2
__attribute__((weak))
void* operator new(size_t size) 
{ 
	 return malloc(size); 
}
#endif

#ifdef F_Operators_delete
__attribute__((weak))
void operator delete(void* pData)
{
	free(pData);
}
#endif

#ifdef F_Operators_delete2
__attribute__((weak))
void operator delete[](void* pData)
{
	free(pData);
}
#endif
