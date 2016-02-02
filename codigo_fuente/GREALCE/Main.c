/* Trabajo de PID 2003/2004 */

#include <stdio.h>
#include <string.h>
#include "genetic.h"

char buf[512]; //para mensajes texto

void main( int argc, char *argv[]) {
  TgenConfig cfg;
  Tpopulation *pop;
  char aux[1024]="..\\";

  printf("Restauración de imagenes grises\n");
  printf("Algoritmo genético con restriccion Laplaciana\n");

  if (argc!=23) {
	
	printf("Error: No se especificaron correctamente todos los argumentos.\n\n");
	printf("No llame directamente a grealce.exe, utilice en su lugar realce.exe para\n");
	printf("utilizar el algoritmo genético.\n");
	exit(-1);
	//nombre de archivo
	strcpy(cfg.filename,"prueba");
	strcat(cfg.filename,GRF_EXT); //añade la extensión grafica definida en defs.h (.jpg, .bmp, etc)
	//Tamaño de poblacion
	cfg.pop_size=60;
	//Maximas iteraciones
	cfg.limit_iter=1000;
	//error tolerado
	cfg.limit_mse=0.02F;
	//distorsion a original
	cfg.o_distorsion=0;
	//ruido a imagen original
	cfg.o_noise=0;
	//tipo de cromosomas
	cfg.c_type=0;
	//distorsion a cromosomas
	cfg.c_distorsion=0;
	//funcion de evaluación
	cfg.f_eval=0;
	//crossprob[0]
	cfg.crossprob[0]=0.05F;	
	//crossprob[1]
	cfg.crossprob[1]=0.05F;
	//crossprob[2]
	cfg.crossprob[2]=0.02F;
	//crossprob[3]
	cfg.crossprob[3]=0.02F;
	//cte cruce uniforme
	cfg.crosscte=0.5;
	//mutprob[0]
	cfg.mutprob[0]=0.007F;
	//mutprob[1]
	cfg.mutprob[1]=0.007F;
	//cte mut no-uniforme
	cfg.mutcte=0.5F;
	//cte A desenfoque imagen
	cfg.o_defocus_a=7.9F;
	//cte sigma ruido
	cfg.o_gauss_sigma=0.1F;
	//cte mu ruido
	cfg.o_gauss_mu=0.0F;
	//cte A desenfoque cromosomas
	cfg.c_defocus_a=7.0F;
	//cte lambda Laplaciana
    cfg.laplace_lambda=3.0F;

  } else {

	//nombre de archivo
	strcpy(cfg.filename,argv[1]);
	//Tamaño de poblacion
	cfg.pop_size=atoi(argv[2]);
	//Maximas iteraciones
	cfg.limit_iter=atoi(argv[3]);
	//error tolerado
	cfg.limit_mse=(double)atof(argv[4]);
	//distorsion a original
	cfg.o_distorsion=atoi(argv[5]);
	//ruido a imagen original
	cfg.o_noise=atoi(argv[6]);
	//tipo de cromosomas
	cfg.c_type=atoi(argv[7]);
	//distorsion a cromosomas
	cfg.c_distorsion=atoi(argv[8]);
	//funcion de evaluación
	cfg.f_eval=atoi(argv[9]);
	//crossprob[0]
	cfg.crossprob[0]=(double)atof(argv[10]);	
	//crossprob[1]
	cfg.crossprob[1]=(double)atof(argv[11]);
	//crossprob[2]
	cfg.crossprob[2]=(double)atof(argv[12]);
	//crossprob[3]
	cfg.crossprob[3]=(double)atof(argv[13]);
	//cte cruce uniforme
	cfg.crosscte=(double)atof(argv[14]);
	//mutprob[0]
	cfg.mutprob[0]=(double)atof(argv[15]);
	//mutprob[1]
	cfg.mutprob[1]=(double)atof(argv[16]);
	//cte mut no-uniforme
	cfg.mutcte=(double)atof(argv[17]);
	//cte A desenfoque imagen
	cfg.o_defocus_a=(double)atof(argv[18]);
	//cte sigma ruido
	cfg.o_gauss_sigma=(double)atof(argv[19]);
	//cte mu ruido
	cfg.o_gauss_mu=(double)atof(argv[20]);
	//cte A desenfoque cromosomas
	cfg.c_defocus_a=(double)atof(argv[21]);
	//cte lambda Laplaciana
    cfg.laplace_lambda=(double)atof(argv[22]);
  }

    //creamos un directorio para los resultados
    dir_prepare(cfg);
	//añadimos a nombre de archivo la ruta padre
	//strcat(aux,cfg.filename); //añadimos ( aux="..\" + filename ) y guarda en aux
	//strcpy(cfg.filename,aux); //volvemos a guardar de aux a filename


    pop=initialization(cfg);

	if (pop==NULL) {
		sprintf(buf,"Error, población no inicializada\n");log_mes(buf);
		gfinalize(-1);
	}
	while (pop->state==0) {
		calculation(pop);
		iter_stats(pop);
		pop->state=conditions(pop);
		if (pop->state!=0) 
			break;
		pop=selection(pop);
		crossover(pop);
		mutation(pop);
	}
    finalize(pop);

  //printf("...pulse INTRO para continuar...");
  //getchar();

}
