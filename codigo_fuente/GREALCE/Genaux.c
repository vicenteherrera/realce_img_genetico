
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "genaux.h"
char buf[512];
//-------------------------------------------------------------------------------------------
Tchromosome *image_encoding(TgenConfig cfg) {
	Tchromosome *original, *out;
	int rows, cols, mem;

	original=(Tchromosome *) malloc(sizeof(Tchromosome));
	if (original==NULL) {
		sprintf(buf,"Error al asignar memoria para cromosoma de imagen original.\n");log_mes(buf);
		exit(-1);
	}

	sprintf(buf,"Leyendo imagen '%s'\n",cfg.filename);log_mes(buf);
	original->image = read_image(cfg.filename);
	/* FALTA: comprobación de error al leer imagen */
	rows = num_rows(original->image);
	cols = num_cols(original->image);
	white_borders(original->image);
	
	sprintf(buf,"Leida imagen %i filas",rows);log_mes(buf);
    sprintf(buf," y %i columnas\n",cols);log_mes(buf);
	mem=size_mem_chrom(original);
	sprintf(buf,"Cromosoma original ocupa %i bytes\n",mem);log_mes(buf);
	original->mserror=0;
	//if (blurred!=0)
	//	out=transform_chrom(original,mode);
	//else
		out=original;
	return out;
}
//-----------------------------------------------------------------------
Tchromosome *chrom_creation(Tchromosome *original,int mode) {
    Tchromosome *chrom;
    int rows,cols,i,j;

	chrom=(Tchromosome *) malloc(sizeof(Tchromosome));
	if (chrom!=NULL) {
        rows = num_rows(original->image);
       	cols = num_cols(original->image);
       	chrom->image=create_image(rows,cols);
		for (i=1;i<(rows-1);i++) {
			for (j=0;j<(cols-1);j++) {
				set_pixel(chrom->image,i,j,random_color(mode));
			}
		}
		white_borders(chrom->image);
		chrom->num_child=0;
		//chrom->mserror=INF;
	}
    return chrom;
}
//-----------------------------------------------------------------------
Tchromosome *transform_chrom(Tchromosome *original,Tpopulation *pop, int mode) {
	Tchromosome *trans;
	int rows,cols,i,j;
	unsigned char color;
	unsigned char a,b,c,d,e;
	trans=(Tchromosome *) malloc(sizeof(Tchromosome));

	if (mode==0) {
		trans->image=duplicate_image(original->image);
		return trans;
	}

	rows=num_rows(original->image);
	cols=num_cols(original->image);
	trans->image=create_image(rows,cols);
	//assert(rows==num_rows(trans->image) && cols==num_cols(trans->image));
	for (i=1;i<(rows-1);i++) {
		for(j=1;j<(cols-1);j++) {
			switch(mode) {
				case 1:
				default:
					a=read_pixel(original->image,i,j);
					b=read_pixel(original->image,i-1,j);
					c=read_pixel(original->image,i,j-1);
					d=read_pixel(original->image,i+1,j);
					e=read_pixel(original->image,i,j+1);
					color=(unsigned char)(a/5+b/5+c/5+d/5+e/5);
					set_pixel(trans->image,i,j,color);
					break;
			}
		}
	}
	white_borders(trans->image);
	return trans;
}	
//-----------------------------------------------------------------------
int white_borders(MyTImage *im) {
	int i,rows,cols;
	rows=num_rows(im);
	cols=num_cols(im);
	for (i=0;i<rows;i++) {
		set_pixel(im,i,0,Color_White);
		set_pixel(im,i,cols-1,Color_White);
	}
	for (i=0;i<cols;i++) {
		set_pixel(im,0,i,Color_White);
		set_pixel(im,rows-1,i,Color_White);
	}
	return 0;

}
//-------------------------------------------------------------------------------------------
double mse_chrom(Tchromosome *trans,Tchromosome *original) {
	int rowso,colso,rowst,colst;
	int i,j;
	double error,e;
	assert(trans!=NULL && original != NULL);
	rowso=num_rows(original->image);
	colso=num_cols(original->image);
	rowst=num_rows(trans->image);
	colst=num_cols(trans->image);
	//mensaje4("original: %i filas, %i columnas\n",rowso,colso);
	//mensaje4("trans: %i filas, %i columnas\n",rowst,colst);
	assert(colso==colst && rowso==rowst);
	/* FALTA: comprobar que el mserror se calcula asi */
	error=0;
	for(i=0;i<rowso;i++) {
		for(j=0;j<colso;j++) {
			e=(double)(read_pixel(original->image,i,j)-read_pixel(trans->image,i,j));
			error=error+e*e;
		}
	}
	error=error/pow((double)rowso*(double)colso,2.0F);
	return error;
}	
//-------------------------------------------------------------------------------------------
int size_mem_chrom(Tchromosome *chrom) {
	int mem;
	assert(chrom!=NULL);
	mem=0;
	mem+=sizeof(chrom);
	mem+=sizeof(chrom->mserror);
	mem+=sizeof(chrom->num_child);
	mem+=sizeof(chrom->ticket);
	mem+=size_mem_image(chrom->image);
	return mem;
}
//-----------------------------------------------------------------------
int free_chrom(Tchromosome *chrom) {
	int err;
	err=0;
	assert(chrom!=NULL);
	free_image(chrom->image);
	free(chrom);
	/* FALTA: analizar si ocurrio error */
	return err;
}	
//-----------------------------------------------------------------------
int free_pop(Tpopulation *pop) {
	unsigned int i;
	assert(pop!=NULL); assert(pop->pop_size>0);
	for (i=0;i<pop->pop_size;i++) 
		free_chrom(pop->chrom[i]);
	if (pop->original!=NULL)
	  free_chrom(pop->original);
	if (pop->t_orig!=NULL)
	  free_chrom(pop->t_orig);
	free(pop);
	return 0;
}	
//-----------------------------------------------------------------------
Tchromosome *duplicate_chrom(Tchromosome *source) {
	Tchromosome *target;
	/* FALTA: ¿se puede hacer esto directamente? */
	target=(Tchromosome *) malloc(sizeof(Tchromosome));
	target->mserror=source->mserror;
	target->num_child=source->num_child;
	target->image=duplicate_image(source->image);
	return target;
}	
//-----------------------------------------------------------------------
int write_info_chrom(Tchromosome *chrom,char *filename) {
	MyTImage *im;
	char name_ext[1024];
	assert(chrom!=NULL);
	strcpy(name_ext,filename);
	strcat(name_ext,GRF_EXT);
	im=duplicate_image(chrom->image);
	write_image(name_ext,im); //esto destruye im
	return 0;
}
//-----------------------------------------------------------------------
double rand0to1(void) {	return ((double)rand() / (double)(RAND_MAX)); }	
double rand0till1(void) { return ((double)rand() / (double)(RAND_MAX+1)); }
double rand_prob(void) { return (1.0F- ((double)rand() / (double)(RAND_MAX+1)) ); }
int rand_0toint(int max) { return (int)(((double)rand() / (double)(RAND_MAX))*(double)max); }
//-----------------------------------------------------------------------
int cross(Tpopulation *pop,int ctype,int ci,int cj) {
	int i,k1,k2,r_c,numc,numr;
	unsigned char aux1,aux2;
	double Auac, A;
	assert(pop!=NULL);

	Auac=pop->mutcte; //0.5F
	numc=num_cols(pop->chrom[ci]->image);
	numr=num_rows(pop->chrom[ci]->image);
	r_c=(int) rand() %2; //row or column exchange
	switch(ctype) {
		case 0: //Uniform R/C crossover
		case 1: //Random R/C crossover
		if (r_c==0) { //intercambio de fila
			k1= rand_0toint(numr-2)+1;
			k2= ctype==0 ? k1 : rand_0toint(numr-2)+1;
			//mensaje4("  intercambio entre fila %i y %i\n",k1,k2);
			for(i=1;i<(numc-1);i++) {
				aux1=read_pixel(pop->chrom[ci]->image,k1,i);
				aux2=read_pixel(pop->chrom[cj]->image,k2,i);
				set_pixel(pop->chrom[ci]->image,k1,i,aux2);
				set_pixel(pop->chrom[cj]->image,k2,i,aux1);
			}
		} else { //intercambio de columna
			k1= rand_0toint(numc-2)+1;
			k2= ctype==0 ? k1 : rand_0toint(numc-2)+1;
			//mensaje4("  intercambio entre col. %i y %i\n",k1,k2);
			for(i=1;i<(numr-1);i++) {
				aux1=read_pixel(pop->chrom[ci]->image,i,k1);
				aux2=read_pixel(pop->chrom[cj]->image,i,k2);
				set_pixel(pop->chrom[ci]->image,i,k1,aux2);
				set_pixel(pop->chrom[cj]->image,i,k2,aux1);
			}
		
		}
		break;
		case 2: //uniform arithmetic crossover
				//ci=a*ci+(1-a)*cj
			    //cj=a*cj+(1-a)*ci
			   // A=cte
			   A=Auac;
		case 3: //non-uniform arithmetic crossover
			//idem, pero..  A=(1-t/T)*0.5
			A=(1.0F-((float)pop->iter/(float)pop->limit_iter))*0.5F;
			if (r_c==0) { //intercambio de una fila
			  k1= rand_0toint(numr-2)+1;
			  k2= rand_0toint(numr-2)+1;
              for(i=1;i<(numc-1);i++) {
			    aux1=read_pixel(pop->chrom[ci]->image,k1,i);
				aux2=read_pixel(pop->chrom[ci]->image,k2,i);
				set_pixel(pop->chrom[ci]->image,k1,i,black_or_white((unsigned char)(A*aux1+(1-A)*aux2),pop->c_type));
				set_pixel(pop->chrom[cj]->image,k1,i,black_or_white((unsigned char)(A*aux2+(1-A)*aux1),pop->c_type));			    
			  }
			} else {
			  k1= rand_0toint(numc-2)+1;
			  k2= rand_0toint(numc-2)+1;
              for(i=1;i<(numr-1);i++) {
			    aux1=read_pixel(pop->chrom[ci]->image,i,k1);
				aux2=read_pixel(pop->chrom[ci]->image,i,k2);
			    set_pixel(pop->chrom[ci]->image,i,k1,black_or_white((unsigned char)(A*aux1+(1-A)*aux2),pop->c_type));
	  		    set_pixel(pop->chrom[cj]->image,i,k1,black_or_white((unsigned char)(A*aux2+(1-A)*aux1),pop->c_type));			    
			  }

			}
			break;
		default:
		break;

	}
	return 0;
}	
//-----------------------------------------------------------------------

int f_num_delta(unsigned int t,unsigned int T,int y,double b) {
	return (int)(y*(1.0F-pow( rand0to1(),pow((1.0F-(t/T)),b) )));
}
//-----------------------------------------------------------------------
int mutate(Tpopulation *pop, int ci, int mtype) {
	int i,j;
	int numc,numr;
	int rdigit;
	double aux,divp,Bnum;
	int color;

	Bnum=pop->mutcte; //0.1F
	numr=num_rows(pop->chrom[ci]->image);
	numc=num_cols(pop->chrom[ci]->image);

	i=rand_0toint(numr-3)+1;
	j=rand_0toint(numc-3)+1;

	switch(mtype) {
		case 0:
			aux=0.0F;
			divp=8.0F;
			aux+=(double)read_pixel(pop->chrom[ci]->image,i-1,j-1);
			aux+=(double)read_pixel(pop->chrom[ci]->image,i-1,j);
			aux+=(double)read_pixel(pop->chrom[ci]->image,i-1,j+1);
			aux+=(double)read_pixel(pop->chrom[ci]->image,i,j-1);
			aux+=(double)read_pixel(pop->chrom[ci]->image,i,j+1);
			aux+=(double)read_pixel(pop->chrom[ci]->image,i+1,j-1);
			aux+=(double)read_pixel(pop->chrom[ci]->image,i+1,j);
			aux+=(double)read_pixel(pop->chrom[ci]->image,i+1,j+1);
			aux=aux/divp;
			if (pop->c_type==0) {
			  if (aux>128.0F) set_pixel(pop->chrom[ci]->image,i,j,255);
			  else set_pixel(pop->chrom[ci]->image,i,j,0);
			} else set_pixel(pop->chrom[ci]->image,i,j,(unsigned char)aux);
		break;
		case 1:
			rdigit=(rand_0toint(255)-128);
			color=read_pixel(pop->chrom[ci]->image,i,j);
			if (rdigit>0) {
				color=f_num_delta(pop->iter,pop->limit_iter,256-color,Bnum);
				color=read_pixel(pop->chrom[ci]->image,i,j)+color;
			} else {
				color=f_num_delta(pop->iter,pop->limit_iter,0-color,Bnum);
				color=read_pixel(pop->chrom[ci]->image,i,j)-color;
			}
			color=black_or_white((unsigned char)color,pop->c_type);
		default:
		break;
	}

	return 0;
}
//-----------------------------------------------------------------------
void gfinalize(int i){
	exit(i);
}
//-----------------------------------------------------------------------
int adapt_color(double f) {
	int color;
	if (f>128.0F) color=255;
	else color=0;
	return color;
}
//-----------------------------------------------------------------------
int write_html_log(Tpopulation *pop, int k) {
	long int taux;
	FILE *f;
	int i;
	static int j=-1;
	unsigned int numc,numm;
	if (k>9) j--;
	sprintf(buf,"Escribiendo fichero log\n");log_mes(buf);
	f=fopen("vista.htm","w+");
	if (f==NULL) { 
		sprintf(buf,"Error al abrir el fichero\n");log_mes(buf);
		return -1;
	}
	fprintf(f,"<html><head>\n");
	if (pop->state==0 && pop->iter<pop->limit_iter)
	  fprintf(f,"<meta http-equiv=\"refresh\" content=\"%i;URL=vista.htm;target=_top\">\n",4);
	fprintf(f,"<title>Realce de imagenes grises mediante algoritmo genético</title>\n");
	fprintf(f,"</head>\n<body>\n");
	fprintf(f,"<b><u>Realce de imagenes grises mediante algoritmo genético</u></b><br><br>\n");
	fprintf(f,"Imagen:  <b>%s</b>, Tamaño: <b>%ix%i</b><br>\n",pop->source_fn,num_rows(pop->original->image),num_cols(pop->original->image));
	fprintf(f,"Poblacion: <b>%i</b> cromosomas, Max iteraciones: <b>%i</b><br>",pop->pop_size,pop->limit_iter);
//	fprintf(f,"Modo: <b>%i</b> , Original ya trans.: <b>%i</b><br>",pop->mode,pop->original_blurred);
	fprintf(f,"Tolerancia error cuadrático medio: <b>%.2f</b><br>\n",pop->limit_mse);
	fprintf(f,"<br>");
	fprintf(f,"Imagen original: <img src=\"%s\">",pop->source_fn);
	fprintf(f," => Transformada: <img src=\"torig%s\"><br>",GRF_EXT);
	for (i=0;i<=j && i<=9;i++) {
		fprintf(f,"<img src=\"%i%s\">",i,GRF_EXT);
	}
	fprintf(f,"<br>\n");
	for (i=0;i<=j && i<=9;i++) {
		fprintf(f,"<img src=\"t%i%s\">",i,GRF_EXT);
	}
	fprintf(f,"<br>");
	if (pop->iter>0) {
	    fprintf(f,"Mejor ajuste: ");
		if (pop->state!=0) fprintf(f,"<img src=\"best.jpg\">");
		else
		  fprintf(f,"<img src=\"%i%s\">",j,GRF_EXT);

		fprintf(f," , error: <b>%.2f</b><br><br>",pop->best->mserror);
	}
	else
	  fprintf(f,"<br>");
	if (pop->state==-1) {
      fprintf(f,"<b>--Proceso finalizado</b><br>\n");
      fprintf(f,"--Se alcanzó el límite de %i iteraciones<br>\n",pop->limit_iter);
	} else if (pop->state==1) {
      fprintf(f,"<b>--Proceso finalizado</b><br>\n");
	  fprintf(f,"--Se alcanzó el error tolerado %.2f en la iter %i<br>\n",pop->limit_mse,pop->iter);
	}else {
		if (j<10) {
          fprintf(f,"(Iteración  %i)<br>\n",pop->iter);
	      taux=(pop->thistime - pop->initime);
		  if (pop->iter<2)
			fprintf(f,"<br>");
		  else
	        fprintf(f,"Quedan <b>%.2f s</b> para terminar (<b>%.2f min</b>)<br>\n",
		       pop->limit_iter/((double)pop->iter/(double)taux)-taux,
		      (pop->limit_iter/((double)pop->iter/(double)taux)-taux)/60
		      );
		}
	}
	if (pop->iter>1) {
	  fprintf(f,"Tiempo empleado: <b>%im:%is</b><br>\n",(pop->thistime-pop->initime)/60,(pop->thistime-pop->initime)%60);
	  fprintf(f,"Velocidad media: <b>%.2f iter/s</b><br>\n",((double)pop->iter)/(pop->thistime-pop->initime));
	} else {
	  fprintf(f,"<br><br>");
	}
	fprintf(f,"<br>\n");


	numc=pop->num_cross[0]+pop->num_cross[1]+pop->num_cross[2]+pop->num_cross[3];
	numm=pop->num_mut[0]+pop->num_mut[1];
	fprintf(f,"<table><tr><td valign=\"top\">");
	fprintf(f,"<font size=2>");
	fprintf(f,"<u>Cruces:</u><br>");
	fprintf(f,"(1) F/C uniforme<br>");
	fprintf(f,"(2) F/C aleatorio<br>");
	fprintf(f,"(3) Aritmético uniforme<br>");
	fprintf(f,"(4) Aritmético no uniforme<br>");
	fprintf(f,"<br>Total cruces: %i<br>\n",numc);
	fprintf(f,"media %.2f cruces/iter\n",numc/(double)pop->iter);
	fprintf(f,"</font>");
	fprintf(f,"</td><td valign=\"top\">");
	fprintf(f,"<font size=2>");
	fprintf(f,"prob<br>");
	fprintf(f,"<b>%.4f</b><br>",pop->crossprob[0]);
	fprintf(f,"<b>%.4f</b><br>",pop->crossprob[1]);
	fprintf(f,"<b>%.4f</b><br>",pop->crossprob[2]);
	fprintf(f,"<b>%.4f</b><br>",pop->crossprob[3]);
	fprintf(f,"</font>");
	fprintf(f,"</td><td valign=\"top\">");
	fprintf(f,"<font size=2>");
	fprintf(f,"num<br>");
	fprintf(f,"%i<br>",pop->num_cross[0]);
	fprintf(f,"%i<br>",pop->num_cross[1]);
	fprintf(f,"%i<br>",pop->num_cross[2]);
	fprintf(f,"%i<br>",pop->num_cross[3]);
	fprintf(f,"</font>");
	fprintf(f,"</td><td valign=\"top\">");
	fprintf(f,"<font size=2>");

	fprintf(f,"<u>Mutaciones:</u><br>");
	fprintf(f,"(1) Mutación Uniforme<br>");
	fprintf(f,"(2) Mutación No uniforme<br>");
	fprintf(f,"<br><br><br>Total mutaciones: %i<br>\n",numm);
	fprintf(f,"media %.2f mut/iter\n",numm/(double)pop->iter);
	fprintf(f,"</font>");
	fprintf(f,"</td><td valign=\"top\">");
	fprintf(f,"<font size=2>prob<br>");
	fprintf(f,"<b>%.4f</b><br>",pop->mutprob[0]);
	fprintf(f,"<b>%.4f</b><br>",pop->mutprob[1]);
	fprintf(f,"</font>");
	fprintf(f,"</td><td valign=\"top\">");
	fprintf(f,"<font size=2>num<br>");
	fprintf(f,"%i<br>",pop->num_mut[0]);
	fprintf(f,"%i<br>",pop->num_mut[1]);
	fprintf(f,"</font>");
	fprintf(f,"</td></tr></table>");
	fprintf(f,"<br>\n");

	fprintf(f,"\n</body>\n</html>\n");	
	fclose(f);
	j++;
	return 0;
	//system("start vista.htm"); //esto no funciona y no se porq
}
//-----------------------------------------------------------------------