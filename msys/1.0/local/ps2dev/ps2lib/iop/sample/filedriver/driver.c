/*
  PSX2 OpenSource Project
  (C)2001, Gustavo Scotti (gustavo@scotti.com)
  ------------------------------------------------------------------------
  fileio.c
			IOP FILEIO driver 
			so much code reversed, simple implementation
*/

#include <tamtypes.h>
#include <fileio.h>
#include <stdlib.h>
#include <stdio.h>

static struct fileio_driver gus_driver;

void dump_struct( u8 *addr, int size)
{
   int		i, n;
   for (i = 0; i < size; i++)
      {
      if (!(i % 16))
         printf("%04x  ", i);
      printf("%02x ", addr[i]);
      if ((i % 16)==15)
         {
         printf("  ");
         for (n = i - 15; n <=i; n++)
            {
            if (addr[n] < 32)
               printf(".");
            else
               printf("%c", addr[n]);
            }
         printf("\n");
         }
      }
   printf("\n");
}

int dummy()
{
   printf("dummy function called\n");
   return -5;
}

void gd_initialize( struct fileio_driver *driver)
{
   printf("initializing %s\n", driver->device);
}

int gd_open( int fd, char *name, int mode)
{
   printf("fd......... %x\n", fd);
   dump_struct((u8 *)fd, 16);
   printf("name....... %s %x\n", name, name);
   printf("mode....... %d\n", mode);
   return 0;
}

int gd_close( int fd)
{
   printf("close fd..... %d\n", fd);
   return 0;
}

static void *gd_functarray[16];

void
_start( int argc, char **argv)
{
   int	i, fd;
   
   printf("Teste de um driver de FILEIO\n");
   printf("(C)2001, Gustavo Scotti (gustavo@scotti.com)\n\n");
   gus_driver.device = "gusfile";
   gus_driver.xx1 = 16;
   gus_driver.version = 1;
   gus_driver.description = "Teste do fileIO thing";
   gus_driver.function_list = gd_functarray;
   for (i=0;i < 16; i++)
      gd_functarray[i] = dummy;
   gd_functarray[ 0] = gd_initialize;
   gd_functarray[ 3] = gd_open;
   gd_functarray[ 4] = gd_close;
   FILEIO_del( "gusfile");
   FILEIO_add( &gus_driver);
   fd = open("gusfile:debug", 0);
   printf("open return %d\n", fd);
   printf("close return %d\n", close(fd));
   FILEIO_del( "gusfile");
}

