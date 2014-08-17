#if !defined( BITOPS_H )
#define BITOPS_H

#define SET_BIT(var, pos)    ((var) |= (1<<(pos)))
#define CLEAR_BIT(var, pos)  ((var) &= (~(1<<(pos))))
#define TOGGLE_BIT(var, pos) ((var) ^= (1<<(pos)))
#define CHECK_BIT(var, pos)  ((var) &  (1<<(pos)))

#endif // !defined( BITOPS_H )
