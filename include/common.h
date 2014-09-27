#if !defined( COMMON_H )
#define COMMON_H

#define MAX(a, b) ({ typeof(a) _a = a; typeof(b) _b = b; _a > _b? _a : _b; })
#define MIN(a, b) ({ typeof(a) _a = a; typeof(b) _b = b; _a < _b? _a : _b; })

#endif // !defined( COMMON_H )
