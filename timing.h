#include <sys/time.h>
#include <time.h> /* for ctime() */
static double t_gettimeofday ;
static struct timeval s ;

void b_t( void ) 
{ /* hack together a clock w/ microsecond resolution */
  gettimeofday( &s , NULL ) ;
  t_gettimeofday = s.tv_sec + 1e-6 * s.tv_usec ;
}

double e_t() 
{
    gettimeofday( &s , NULL ) ;
    return s.tv_sec + 1e-6 * s.tv_usec - t_gettimeofday ;
}
