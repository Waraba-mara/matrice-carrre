#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int fon_f(int a){
	int s=1;
	for (int i = 0; i < a; ++i)
	{
		s=s*2;
	}
	return s;
}


int fon_g(int b){
	
	return (2*b);
}


 int main(int argc, char  *argv[])
{  
	int f=0;
	int g=0;
	int resultat=0;
	int opt;
	int premier_opt=1;

	if(argc<3)
	{
         printf("le nombre d'argument est insuffisant\n" );
         return 0;
	}


   while( (opt=getopt(argc,argv,"f:g"))!=-1)
   {
      switch(opt)
      {
         case 'f': 
         	  f=atoi(optarg);
         	  if (premier_opt)
         	  {
         	  	resultat=f;
         	  	premier_opt=0;
         	  }
         	  resultat=fon_f(resultat);
         	break;

         case 'g':
	         	 
	         	  if (premier_opt)
	         	  { 
	         	  	g=atoi(argv[optind]);
	         	  	resultat=g;
	         	  	optind++;
	         	  	premier_opt=0;
	         	  }
	         	  resultat=fon_g(resultat);
         	break;


         case '?':
                printf("Option invalide\n");
                return 1;
      }

   }

   printf("le resultat est : %d \n",resultat);

	return 0;
}