#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef long int uint64_t;

#ifndef _countof
#define _countof(x) (sizeof((x)) / sizeof((x)[0]))
#endif

typedef struct _gbsff_entry_t
{
   const char * name;
   unsigned char (*proc)( uint64_t offset );
} gbsff_entry_t;

inline unsigned char func_flip_coin( uint64_t offset )
{
   return (offset & 0x1) ? 0 : 1;
}

inline unsigned char func_loop_increase( uint64_t offset )
{
   return (unsigned char)(offset % 0x100);
}

void gbsff_run_func( uint64_t filesize, unsigned char (*proc)(uint64_t) )
{
   unsigned char buffer[4096];
   uint64_t pages = ( filesize + 4095 ) / 4096;
   unsigned int last_page_length = (unsigned int)( filesize % 4096 );

   // write file page by page
   for ( int page = 0; page < pages; page++ )
   {
      int page_length = 4096;

      if ( page == pages - 1 && last_page_length != 0 )
         page_length = last_page_length;

      for ( int i = 0; i < page_length; i++ )
      {
         buffer[i] = proc( page * 4096 + i );
      }

      ::fwrite( buffer, sizeof( buffer ), 1, stdout );
   }
}

gbsff_entry_t gbsff_entries[] = 
{
   { "flip-coin", &func_flip_coin },
   { "loop-increase", &func_loop_increase },
};

void usage( const char * exec )
{
   fprintf( stderr, "usage of %s:\n\n", exec );
   fprintf( stderr, "\t%s --function [flip-coin|round-robin-byte] --filesize {any_integer_value_in_a_kilobytes}\n", exec );
}

uint64_t get_file_size_from_str( const char * str_filesize )
{
   if ( !str_filesize )
      return 0;

   int unit = 1;
   int str_filesize_len = strlen( str_filesize );
   char * str_tmp = ::strdup( str_filesize );
   
   switch ( str_filesize[ str_filesize_len - 1 ] )
   {
      case 'G': case 'g':
         unit = 1024 * 1024 * 1024;
         break;
      case 'M': case 'm':
         unit = 1024 * 1024;
         break;
      case 'K': case 'k':
         unit = 1024;
         break;
      default:
         unit = 1;
   }

   str_tmp[ str_filesize_len - 1 ] = '\0';
   unit *= ::atol( str_tmp );
   free( str_tmp );

   return unit;
}

int main( int argc, char * argv[] )
{
   const char * func = "flip-coin";
   uint64_t filesize = 4096;

   for ( int i = 1; i < argc; i++ )
   {
      if ( !strcmp( argv[i], "--function" ) ) {
         func = argv[i + 1];
         i++;
      } else if ( !strcmp( argv[i], "--filesize" ) ) {
         filesize = get_file_size_from_str( argv[i + 1] );
         i++;
      } else if ( !strcmp( argv[i], "--help" ) ) {
         usage( argv[0] );
         return 0;
      } else {
         usage( argv[0] );
         return -1;
      }
   }

   for ( int i = 0; i < (int)_countof( gbsff_entries ); i++ )
   {
      if ( !strcmp( func, gbsff_entries[i].name ) )
      {
         ::gbsff_run_func( filesize, gbsff_entries[i].proc );
         return 0;
      }
   }

   fprintf( stderr, "error! no matching function %s", func );
   return -1;
}
