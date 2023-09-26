#ifndef stdarg_included
#define stdarg_included

typedef unsigned char* va_list;

#define	va_start(ptr, LASTARG) (ptr=((va_list)&(LASTARG) + sizeof(LASTARG)))

/* nothing for va_end */
#define va_end(ptr)

#define va_arg(ptr, TYPE) (ptr += sizeof(TYPE), *((TYPE *)(ptr - sizeof(TYPE))))

#endif // stdarg_included
