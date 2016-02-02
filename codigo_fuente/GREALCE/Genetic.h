
#include "gengui.h"
#include "genaux.h"



Tpopulation *initialization(TgenConfig cfg);

int genetic(Tpopulation *pop);
void finalize(Tpopulation *pop);
void final_stats(Tpopulation *pop);
void iter_stats(Tpopulation *pop);
int mutation(Tpopulation *pop);
int crossover(Tpopulation *pop);
Tpopulation *selection(Tpopulation *population);
int conditions(Tpopulation *pop);
int calculation(Tpopulation *population);
Tpopulation *iterate(Tpopulation *pop);
void dir_prepare(TgenConfig cfg);
