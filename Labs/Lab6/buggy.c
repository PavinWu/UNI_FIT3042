/* 
 * buggy.c: a buggy program
 *        
 *          This program is intended to allow the user to enter a positive
 *          integer x, and then print a table for all numbers i between 
 *          1 and x (inclusive) print a table containing i and i squared.
 *          It doesn't do that!
 *
 *          eg if the user enters 3, the output should be
 *          1 	1
 *          2   4
 *          3	9
 *  
 * Robert Merkel <robert.merkel@monash.edu>
 * 21/02/2012
 */

#include <stdio.h>


int main()
{
  int x, i;
  printf("Enter a positive whole number :");
  scanf("%d", &x);

  for(i = 1; i <= x; i++)
  {
    printf("%d\t%d\n", i, i * i);
  }

  return 0;
}
