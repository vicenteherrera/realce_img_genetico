/* un cromosoma almacena una imagen y el error cuadratico medio de esta */

#include "gengrf.h"
#include "gengui.h"

#include <time.h>

typedef struct {
	//nombre de archivo
	char filename[1024];
	//Tamaño de poblacion
	unsigned int pop_size;
	//Maximas iteraciones
	unsigned int limit_iter;
	//error tolerado
	double limit_mse;
	//distorsion a original
	int o_distorsion;
	//ruido a imagen original
	int o_noise;
	//tipo de cromosomas
	int c_type;
	//distorsion a cromosomas
	int c_distorsion;
	//funcion de evaluación
	int f_eval;
	//probabilidades de cruce
	double crossprob[4];
	//probabilidades de mutación
	double mutprob[2];
	//cte cruce uniforme
	double crosscte;
	//cte mut no-uniforme
	double mutcte;
	//cte A desenfoque imagen
	double o_defocus_a;
	//cte sigma ruido
	double o_gauss_sigma;
	//cte mu ruido
	double o_gauss_mu;
	//cte A desenfoque cromosomas
	double c_defocus_a;
	//cte lambda Laplaciana
    double laplace_lambda;
} TgenConfig;

typedef struct {
	MyTImage *image;
	double mserror; /* mean-square error */
	double ticket; /* para la seleccion por ruleta */
	unsigned int num_child; /* veces elegido para nueva generacion */
} Tchromosome;

typedef struct {
	Tchromosome** chrom; //para array de chromosomas
	Tchromosome* best; //apunta al cromosoma de chrom[] con menor mse
	Tchromosome* original; //cromosoma que contiene la imagen original
	Tchromosome* t_orig;  //cromosoma que contiene la imagen original transformada

	char source_fn[1024];
	unsigned int num_cross[4];
	unsigned int num_mut[4];
	time_t initime;
	time_t endtime;
	time_t thistime;
	int state; // indica si se ha finalizado de iterar: 
	           // 0 no,1 maximas iters, -1 tolerancia mse encontrada
	double worse_mse;
	unsigned int iter;

	//nombre de archivo
	char filename[1024];
	//Tamaño de poblacion
	unsigned int pop_size;
	//Maximas iteraciones
	unsigned int limit_iter;
	//error tolerado
	double limit_mse;
	//distorsion a original
	int o_distorsion;
	//ruido a imagen original
	int o_noise;
	//tipo de cromosomas
	int c_type;
	//distorsion a cromosomas
	int c_distorsion;
	//funcion de evaluación
	int f_eval;
	//probabilidades de cruce
	double crossprob[4];
	//probabilidades de mutación
	double mutprob[2];
	//cte cruce uniforme
	double crosscte;
	//cte mut no-uniforme
	double mutcte;
	//cte A desenfoque imagen
	double o_defocus_a;
	//cte sigma ruido
	double o_gauss_sigma;
	//cte mu ruido
	double o_gauss_mu;
	//cte A desenfoque cromosomas
	double c_defocus_a;
	//cte lambda Laplaciana
    double laplace_lambda;

} Tpopulation;
/* La población es un array dinámico de punteros a Tchromosome */
/* population->chrom=(Tchromosome**) malloc(sizeof(Tchromosome*)*pop_size); */
/* asi lo utilizamos luego como si se hubiera declarado */
/* con: Tchromosome *Tpop.chrom[rango]; */
/* es decir: population->chrom[i]; */

int size_mem_chrom(Tchromosome *chrom);
//Devuelve la cantidad de memoria utilizada por un cromosoma
Tchromosome *image_encoding(TgenConfig cfg);
//Lee una image y almacena su contenido en un cromosoma
Tchromosome *chrom_creation(Tchromosome *original, int mode);
//Crea un cromosoma asignandole la memoria y un genotipo aleatorio de
//caracteristicas similares al original (una imagen aleatoria del mismo
//tamaño que la contenida en original).
int free_chrom(Tchromosome *chrom);
//libera memoria asignada a un cromosoma
int free_pop(Tpopulation *pop);
//libera memoria asignada a una poblacion
int white_borders(MyTImage *im);
//Pinta con blanco los bordes de una imagen
Tchromosome *duplicate_chrom(Tchromosome *source);
//duplica un cromosoma
int write_info_chrom(Tchromosome *chrom,char *filename);
//Vuelca a disco la imagen contenida en chrom como jpg
//esto no destruye el cromosoma
Tchromosome *transform_chrom(Tchromosome *original,Tpopulation *pop, int mode);
//transforma la informacion de un cromosoma segun la funcion
//especificada por 'mode', y la almacena en otro
double mse_chrom(Tchromosome *trans,Tchromosome *original);
//asigna a un cromosoma el error cuadratico medio en comparacion
//con el cromosoma original

int cross(Tpopulation *pop,int ctype,int ci,int cj);
//cruza el cromosoma ci y el cj de la poblacion pop segun el modo ctype
int mutate(Tpopulation *pop, int ci, int mtype);
//muta el cromosoma ci de la poblacion pop segun el modo mtype

void gfinalize(int i);
//realiza los ajustes necesarios para finalizar todo el algoritmo

double rand0to1(void);
// genera un double aleatorio en el rango [0,1]
double rand0till1(void);
// genera un double aleatorio en el rango [0,1)
double rand_prob(void);
// genera un double aleatorio en el rango (0,1]
int rand_0toint(int max);
// genera un entero aleatorio en el rango [0,max]

int write_html_log(Tpopulation *pop, int j);
