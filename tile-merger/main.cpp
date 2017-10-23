#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>

using namespace std;


void merge( char *file1, char *file2, char *alphamap, char *resfilename = 0 )
{
   char Buf[200];

   sprintf(Buf, "convert %s -negate alpha_neg.png", alphamap);
   system(Buf);

   sprintf(Buf, "composite -compose Multiply %s %s tmp1.png", file1, alphamap);
   system(Buf);

   sprintf(Buf, "composite -compose Multiply %s %s tmp2.png", file2, "alpha_neg.png");
   system(Buf);

   char resfile[100];
   char tmp[100];

   strcpy(tmp, file1);
   char *search = ".";
   char *l1 = strtok(tmp, search);

   char tmp2[100];

   strcpy(tmp2, l1);

   strcpy(tmp, file2);
   char *l2 = strtok(tmp, search);

   sprintf(resfile, "%s_%s_%s", tmp2, l2, alphamap);

   if (resfilename == 0)
     sprintf(Buf, "composite -compose Add tmp1.png tmp2.png %s", resfile);
   else
     sprintf(Buf, "composite -compose Add tmp1.png tmp2.png %s", resfilename);

   system(Buf);

   sprintf(Buf, "del tmp1.png tmp2.png alpha_neg.png");
   system(Buf);
}



int main( int argc, char *argv[] )
{
   if (argc < 4)
      return 1;

   //merge("grass01@pc.png", "sand01@pc.png", "alpha_tile01@pc.png");
   if (argc < 5)
     merge(argv[1], argv[2], argv[3]);
   else
     merge(argv[1], argv[2], argv[3], argv[4]);

   return 0;
}