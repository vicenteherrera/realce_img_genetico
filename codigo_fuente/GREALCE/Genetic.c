
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>
//#include <chdir.h>
//#include <mkdir.h>

#include "genetic.h"

char buf[256]; //para mensajes texto

//-------------------------------------------------------------------------------------------
// inicializa la población de cromosomas con valores aleatorios
Tpopulation *initialization(TgenConfig cfg) {
	int error,mem;
	unsigned int i,j;
	Tpopulation *population;

    assert(cfg.pop_size>0);

	srand(time(NULL)); //inicializamos el generador de num aleatorios
	population=(Tpopulation*) malloc(sizeof(Tpopulation)); //reservamos memoria

	//nombre de archivo
	strcpy(population->source_fn,cfg.filename);
	//Tamaño de poblacion
	population->pop_size=cfg.pop_size;
	//Maximas iteraciones
	population->limit_iter=cfg.limit_iter;
	//error tolerado
	population->limit_mse=cfg.limit_mse;
	//distorsion a original
	population->o_distorsion=cfg.o_distorsion;
	//ruido a imagen original
	population->o_noise=cfg.o_noise;
	//tipo de cromosomas
	population->c_type=cfg.c_type;
	//distorsion a cromosomas
	population->c_distorsion=cfg.c_distorsion;
	//funcion de evaluación
	population->f_eval=cfg.f_eval;
	//probabilidades de cruce
	population->crossprob[0]=cfg.crossprob[0];
	population->crossprob[1]=cfg.crossprob[1];
	population->crossprob[2]=cfg.crossprob[2];
	population->crossprob[3]=cfg.crossprob[3]; 
	//probabilidades de mutación
	population->mutprob[0]=cfg.mutprob[0];
	population->mutprob[1]=cfg.mutprob[1];  
	//cte cruce uniforme
	population->crosscte=cfg.crosscte;
	//cte mut no-uniforme
	population->mutcte=cfg.mutcte;
	//cte A desenfoque imagen
	population->o_defocus_a=cfg.o_defocus_a;
	//cte sigma ruido
	population->o_gauss_sigma=cfg.o_gauss_sigma;
	//cte mu ruido
	population->o_gauss_mu=cfg.o_gauss_mu;
	//cte A desenfoque cromosomas
	population->c_defocus_a=cfg.c_defocus_a;
	//cte lambda Laplaciana
    population->laplace_lambda=cfg.laplace_lambda;

	population->iter=0;
	population->state=0;
	population->num_cross[0]=0;
	population->num_cross[1]=0;
	population->num_cross[2]=0;
	population->num_cross[3]=0;
	population->num_mut[0]=0;
	population->num_mut[1]=0;

	//cargamos la imagen original y la transformamos
	population->original=image_encoding(cfg);
	population->t_orig=transform_chrom(population->original,population,population->o_distorsion);
	sprintf(buf,"Transformada imagen en torig\n");log_mes(buf);
	write_info_chrom(population->t_orig,"torig");

	//iniciamos la poblacion de cromosomas
    sprintf(buf,"Inicializando la población de %i cromosomas... \n",cfg.pop_size);log_mes(buf);

	if (population==NULL) {
		sprintf(buf,"\nError al asignar memoria al puntero de poblacion\n");
		gfinalize(-1);
	}
    population->chrom=(Tchromosome**) malloc(sizeof(Tchromosome*)*cfg.pop_size);
	if (population->chrom==NULL) {
		sprintf(buf,"\nError al asignar memoria al array poblacion\n");log_mes(buf);
		gfinalize(-1);
	}

	error=0;mem=0;j=0;
	for (i=0;i<cfg.pop_size;i++) {
		population->chrom[i]=chrom_creation(population->original,cfg.c_type);
		if (population->chrom[i]==NULL) {
			sprintf(buf,"Error al asignar memoria al cromosoma %i\n",i);log_mes(buf);
			sprintf(buf,"Liberando memoria de población para finalizar\n");log_mes(buf);
			for(;i>=0;i--) free_chrom(population->chrom[i]);
			free(population->chrom);
			free(population);
			gfinalize(-1);
		}
		mem+=size_mem_chrom(population->chrom[i]);
     	//sprintf(buf,"Cromosoma ocupa %i bytes\n",size_mem_chrom(population[i]));log_mes(buf);
		if (i>(j*cfg.pop_size/10)) {
			sprintf(buf,"%i",j);log_mes(buf);
			j++;
		}

	}
	mem+=sizeof(population->chrom);
	mem+=sizeof(population);

	sprintf(buf,"Memoria utilizada para la población: %i bytes\n", mem);log_mes(buf);

	write_html_log(population,-1);

	return population;
}
//-------------------------------------------------------------------------------------------
int calculation(Tpopulation *population) { //consta de calculation, constrains, fitness
	Tchromosome *trans;
	unsigned int i,j;
	double mine;
	assert(population!=NULL);
	//calculos de la iteración
    if (population->iter==0) {
      time(&(population->initime));
      population->thistime=population->initime;
	}
    population->iter++;
    //cálculo de fitness
	population->worse_mse=population->chrom[0]->mserror;
	j=0;
	for (i=0;i<population->pop_size;i++) {
		assert(population->chrom[i]!=NULL);
		
		trans=transform_chrom(population->chrom[i],population,population->c_distorsion);
		population->chrom[i]->mserror=mse_chrom(trans,population->t_orig);
		
		if (i==0 || (population->chrom[i]->mserror < mine)) {
			mine=population->chrom[i]->mserror;
			population->best=population->chrom[i];
		}

		if (population->chrom[i]->mserror > population->worse_mse) {
			population->worse_mse=population->chrom[i]->mserror;
		}
		free_chrom(trans);
	}
	return 0;
}
//-------------------------------------------------------------------------------------------
int conditions(Tpopulation *pop) {
	int estate;
	estate=0;
	if (pop->iter>=pop->limit_iter) estate=-1;
	else if (pop->best->mserror<pop->limit_mse) estate=1;
	return estate;
}
//-------------------------------------------------------------------------------------------
Tpopulation *selection(Tpopulation *population) {
	Tpopulation *newp;
	double min,max,elong,ticket;
	double randp,max_randp,min_randp,pchild,pticket;
	unsigned int n_0child,n_1child,n_2child,n_3child;
	unsigned int n_otherchild,maxchild,n_maxchild;

	unsigned int i,j,k;

    newp=(Tpopulation *) malloc(sizeof(Tpopulation));
	memcpy(newp,population,sizeof(Tpopulation));
	newp->original=duplicate_chrom(population->original);
	newp->t_orig=duplicate_chrom(population->t_orig);
	
	newp->chrom=(Tchromosome **)(malloc(sizeof(Tchromosome *)*population->pop_size));
	
	min=1/(population->worse_mse);
	max=1/(population->best->mserror);
	
	//el 'tamaño' de cada ticket es la inversa del error
	//cuanto menor error, un ticket más grande
	//asignamos tickets
	elong=0;
	for (i=0; i<population->pop_size; i++) {
		elong=elong+(1 / population->chrom[i]->mserror);
		population->chrom[i]->ticket=elong;
	}
	//mensaje5("Sorteamos de cero a %f\n",elong);

	//el primero de la nueva generacion siempre es el mejor de la ant
	newp->chrom[0]=duplicate_chrom(population->best);
	//sorteamos el resto de puestos a partir del 1
	k=0;
	for (i=1;i<newp->pop_size;i++) {
		//lanzamos un ticket entre el rango
		ticket=((double)rand() / (double)(RAND_MAX+1)); //entre [0,1)
		ticket=ticket*elong; //entre [0,elong)
		//mensaje5("Ticket %f",ticket);
		//buscamos el ganador del ticket
		j=0;
		while (ticket>population->chrom[j]->ticket) {
			assert(j<=population->pop_size);
			j++;
		}
		//propagamos el ganador a la siguiente generación
		//mensaje2(" ganador %i\n",j);
		newp->chrom[i]=duplicate_chrom(population->chrom[j]);
		newp->chrom[i]->num_child=0;
		population->chrom[j]->num_child++;
		//getchar();

		if (i>(k*population->pop_size/10)) {
			//mensaje2("%i",k);
			k++;
		}
	}
	//mensaje1("\n");


	//analizamos lo bien que ha resultado la propagación, esto
	//se puede saltar, es solo para comprobar que va bien

	if(0) {
		min_randp=0;
		for (i=0;i<population->pop_size;i++) {
			pticket=(population->best->mserror)/(population->chrom[i]->mserror)-(1/(double)(population->worse_mse));
			pchild=((double)population->chrom[i]->num_child)/((double)population->pop_size);
			//mensaje4("Chrom %i hijos=%i",i,population->chrom[i]->num_child);
			//mensaje5(", prop ticket=%f",pticket);
			//mensaje5(", prop hijos=%f\n",pchild);
			//getchar();
			randp=pchild/pticket;

			if(i==0 || max_randp<randp)
				max_randp=randp;
			if(i==0 || randp!=0 && (min_randp>randp || min_randp==0))
				min_randp=randp;
		}
		//mensaje5("proporcion rand max=%f",max_randp);
		//mensaje5(", min=%f\n",min_randp);
		getchar();
	}

	//Sacamos algunas estadísticas que si son útiles
	n_0child=0;	n_1child=0;	n_2child=0;	n_3child=0;
	n_otherchild=0; maxchild=0;	n_maxchild=0;
	for (i=0;i<population->pop_size;i++) {
		switch(population->chrom[i]->num_child) {
			case 0:
				n_0child++;
				break;
			case 1:
				n_1child++;
				break;
			case 2:
				n_2child++;
				break;
			case 3:
				n_3child++;
				break;
			default:
				n_otherchild++;
		}
		if (maxchild < population->chrom[i]->num_child) {
			maxchild=population->chrom[i]->num_child;
			n_maxchild=1;
		} else if (maxchild == population->chrom[i]->num_child) {
			n_maxchild++;		
		}
	}
	//mensaje4("  %ic->0h, %ic->1h, ",n_0child,n_1child);
	//mensaje4("%ic->2h, %ic->3h, ",n_2child,n_3child);
	//mensaje2("%ic->xh, ",n_otherchild);
	//mensaje4("%ic->%ih max\n",n_maxchild,maxchild);

	free_pop(population);
	return newp;
}
//-------------------------------------------------------------------------------------------
int crossover(Tpopulation *pop) {
	double dado;
	int ctype;
	unsigned int ci,cj;

	for (ctype=0; ctype<4; ctype++) {
		for (ci=0;ci<pop->pop_size;ci++) {
			dado=rand_prob();
			if (dado<pop->crossprob[ctype]) {
				cj=(unsigned int)(rand0till1()*(double)pop->pop_size);
				//mensaje2("  cruce %i",ctype);
				//mensaje4(" entre crom %i y %i\n",ci,cj);
				pop->num_cross[ctype]++;
				cross(pop,ctype,ci,cj);
			}
		}
	}
	return 0;
}
//-------------------------------------------------------------------------------------------
int mutation(Tpopulation *pop) {
	double dado;
	int mtype;
	unsigned int i;

	for (mtype=0; mtype<2; mtype++) {
		for (i=0;i<pop->pop_size;i++) {
			dado=rand_prob();
			if (dado<pop->mutprob[mtype]) {
				//mensaje2("  mutacion %i",mtype);
				//mensaje2(" en crom %i\n",i);
				pop->num_mut[mtype]++;
				mutate(pop,i,mtype);
			}
		}
	}
	return 0;
}
//------------------------------------------------------------------------------------------
void iter_stats(Tpopulation *pop) {
  time_t auxtime;
  static int ini_report=1;
  static unsigned int j=0;
  char fn[3];
  Tchromosome *aux;
  long int taux;

  //buscamos el paso de un segundo
  time(&auxtime);

  if (auxtime>pop->thistime) {
	pop->thistime=auxtime;
	sprintf(buf,"--Iteración %i \n",pop->iter);log_mes(buf);
  }
  taux=(auxtime - pop->initime);
  if ( ini_report && (auxtime - pop->initime) >=5 ) {
	sprintf(buf,"Quedan %f segundos para terminar (%f minutos)\n",
		pop->limit_iter/((double)pop->iter/(double)taux)-taux,
		(pop->limit_iter/((double)pop->iter/(double)taux)-taux)/60
		);log_mes(buf);
//	sprintf(buf,"       %f minutos\n",(pop->limit_iter/(pop->iter/5.0F)-5)/60);log_mes(buf);
	sprintf(buf,"Velocidad media: %f iteraciones/segundo\n",((double)pop->iter/(auxtime-pop->initime)));log_mes(buf);
	ini_report=0;
  }

  if (pop->iter>(j*pop->limit_iter/10)) {
	sprintf(buf,"--Iteración %i ",pop->iter);log_mes(buf);
	sprintf(buf,"  mejor mse: %f",pop->best->mserror);log_mes(buf);
	sprintf(buf,", peor mse: %f\n",pop->worse_mse);log_mes(buf);
	fn[0]='0'+j; 	fn[1]='\0';
	write_info_chrom(pop->best,fn);
	aux=transform_chrom(pop->best,pop,pop->c_distorsion);
	fn[0]='t'; fn[1]='0'+j;	fn[2]='\0';
	write_info_chrom(aux,fn);
	free_chrom(aux);
	write_html_log(pop,j);
	j++;
  }
}
//-------------------------------------------------------------------------------------------
void final_stats(Tpopulation *pop) {
	sprintf(buf,"--------------------------------------------------------\n");log_mes(buf);
	if (pop->state==-1)
		sprintf(buf,"--Se alcanzó el límite de %i iteraciones\n",pop->limit_iter);log_mes(buf);
	sprintf(buf,"Tiempo empleado: %im:%is\n",(pop->thistime-pop->initime)/60,(pop->thistime-pop->initime)%60);log_mes(buf);
	sprintf(buf,"Velocidad media global: %f iteraciones/segundo\n",((double)pop->iter)/(pop->thistime-pop->initime));log_mes(buf);
	sprintf(buf,"Escribiendo a disco el mejor ajuste: best.jpg\n");log_mes(buf);
	write_info_chrom(pop->best,"best");

	//dependiente de la libreria **CAMBIAR***
	write_Image(pop->best->image,"best.tif",CVIP_YES,CVIP_NO,TIF,CVIP_NO);

	write_html_log(pop,10);

}
//-----------------------------------------------------------------------------------------
void finalize(Tpopulation *pop) {
    final_stats(pop);
	sprintf(buf,"Liberando memoria de población\n");log_mes(buf);
	free_pop(pop); pop=NULL;
}
//-------------------------------------------------------------------------------------------
int genetic(Tpopulation *pop) {
	if (pop==NULL) {
		sprintf(buf,"Error, población no inicializada\n");log_mes(buf);
		gfinalize(-1);
	}
	while (pop->state==0) {
      pop=iterate(pop);
	}
    finalize(pop);
	return pop->state;
}
//-------------------------------------------------------------------------------------------
int genetic2(TgenConfig cfg) {
	Tpopulation *pop;
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
	return 0;
}
//-------------------------------------------------------------------------------------------
Tpopulation *iterate(Tpopulation *pop) {
  //sprintf(buf,"--+Iteración %i\n",pop->iter);log_mes(buf);
  calculation(pop);
  iter_stats(pop);
  pop->state=conditions(pop);
  if (pop->state!=0) 
	return pop;
  pop=selection(pop);
  crossover(pop);
  mutation(pop);
  return pop;
}
//-------------------------------------------------------------------------------------------
void dir_prepare(TgenConfig cfg) {
	/*
	FILE *f;
	Tchromosome *caux;
	char dir[1024]="prueba";
	return;
	//eliminamos la extensión
	//strcpy(dir,cfg.filename);
	//dir[strlen(dir)-4]='\0';
	mkdir(dir);
	chdir(dir);
	f=fopen("do_gen.bat","w+");
	fprintf(f,"@echo off\n");
	fprintf(f,"if exist vista.htm start vista.htm\n");		
	fprintf(f,"cd..\n");
	//fprintf(f,"grealce.exe %s %i %i %i %i ",cfg.filename,cfg.pop_size,cfg.limit_iter,
	//	    cfg.mode,cfg.original_blurred);
	//fprintf(f," %f %f %f %f %f %f\n",cfg.crossprob[0],cfg.crossprob[1],cfg.crossprob[2],
	//	    cfg.crossprob[3],cfg.mutprob[0],cfg.mutprob[1]);
	fprintf(f,"cd %s\n",dir);
	fclose(f);
	strcpy(dir,"..\\");
	strcat(dir,cfg.filename);
	caux=image_encoding(dir,cfg);
	strcpy(dir,cfg.filename);
	dir[strlen(dir)-4]='\0';
	write_info_chrom(caux,dir);
	free_chrom(caux);
	*/
}