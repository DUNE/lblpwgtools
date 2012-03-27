%module globes
%include cpointer.i
%include globes.h
int PInit(char *file);
%{
#include "globes.h"
  int PInit(char *file)
    {
      int s;
      s=glbInitExperiment(file,&glb_experiment_list[0],&glb_num_of_exps);
      return s;
    }
%}
