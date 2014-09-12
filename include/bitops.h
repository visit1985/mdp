#if !defined( BITOPS_H )
#define BITOPS_H

/*
 * Macros to do bit operations on integer variables.
 *
 * macro: SET_BIT to set bit at pos of var to 1
 * macro: CLEAR_BIT to set bit at pos of var to 0
 * macro: TOGGLE_BIT to toggle bit at pos of var from 1 to 0 and vice versa
 * macro: CHECK_BIT returns > 0 if bit at pos of var is set to 1
 *
 * Example:
 *      int i = 0;
 *      i = SET_BIT(i, 2);
 *      if(CHECK_BIT(i, 2))
 *          printf("bit 2 is set\n");
 *      printf("value of i is %d\n", i);
 *
 */

#define SET_BIT(var, pos)    ((var) |= (1<<(pos)))
#define CLEAR_BIT(var, pos)  ((var) &= (~(1<<(pos))))
#define TOGGLE_BIT(var, pos) ((var) ^= (1<<(pos)))
#define CHECK_BIT(var, pos)  ((var) &  (1<<(pos)))

#endif // !defined( BITOPS_H )
