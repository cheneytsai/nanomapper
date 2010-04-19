

#ifndef OPTIONDB_H
#define OPTIONDB_H
#include "Option.hh"

/*
 * This options package allows the user to specify the name, description,
 * location, and default values of program option variable. To load options from a file -config builtin option should be used
 *
 *   -config <filename>		# load options from <filename>
 *  
 * NOTE: all user-installed option names must begin with a `-', e.g., `-debug'
 */
/* option database definition */
/* handle `-config' builtin option */
#define MAX_LINE_ARGS		256 // max number of arguments in a line
#define MAX_FILENAME_LEN	1024//max length of filename
class OptionDB {
 private:
  Option *options;	/* user-installed options in option database */
 Option *builtin_options;	/* built in options in option database */
 public:
  OptionDB();
  ~OptionDB();
  
/* register an integer option  */
  void reg_int(char *name,			/* option name */
	    char *desc,			/* option description */
	    int *var,			/* pointer to option variable */
	    int def_val);		/* default value of option variable */

  
/* register an integer option list */
void reg_int_list(char *name,		/* option name */
		 char *desc,		/* option description */
		 int *vars,		/* pointer to option array */
		 int nvars,		/* total entries in option array */
		 int *nelt,		/* number of entries parsed */
		 int *def_val);		/* default value of option array */
		
/* register an unsigned integer option variable */
void reg_uint(char *name,		/* option name */
	     char *desc,		/* option description */
	     unsigned int *var,		/* pointer to option variable */
	     unsigned int def_val);	/* default value of option variable */
	    
/* register an unsigned integer option list */
void reg_uint_list(char *name,		/* option name */
		  char *desc,		/* option description */
		  unsigned int *vars,	/* pointer to option array */
		  int nvars,		/* total entries in option array */
		  int *nelt,		/* number of elements parsed */
		  unsigned int *def_val);/* default value of option array */
		 
/* register an unsigned long long option variable */
void reg_ulonglong(char *name,		/* option name */
	       char *desc,		/* option description */
	       unsigned long long *var,	/* target variable */
	       unsigned long long def_val);/* default variable value */
	      
/* register a single-precision floating point option variable */
void reg_float(char *name,		/* option name */
	      char *desc,		/* option description */
	      float *var,		/* target option variable */
	      float def_val);		/* default variable value */
	     
/* register a single-precision floating point option array */
void reg_float_list(char *name,		/* option name */
		   char *desc,		/* option description */
		   float *vars,		/* target array */
		   int nvars,		/* target array size */
		   int *nelt,		/* number of args parsed goes here */
		   float *def_val);	/* default variable value */
		   
/* register a double-precision floating point option variable */
void reg_double(char *name,		/* option name */
	       char *desc,		/* option description */
	       double *var,		/* target variable */
	       double def_val);		/* default variable value */
	       

/* register a double-precision floating point option array */
void reg_double_list(char *name,		/* option name */
		    char *desc,		/* option description */
		    double *vars,	/* target array */
		    int nvars,		/* target array size */
		    int *nelt,		/* number of args parsed goes here */
		    double *def_val);	/* default variable value */

/* register a string option variable */
void reg_string(char *name,		/* option name */
	       char *desc,		/* option description */
	       char **var,		/* pointer to string option variable */
	       char *def_val);		/* default variable value */
	      
/* register a string option array */
void reg_string_list(char *name,		/* option name */
		    char *desc,		/* option description */
		    char **vars,	/* pointer to option string array */
		    int nvars,		/* target array size */
		    int *nelt,		/* number of args parsed goes here */
		    char **def_val);	/* default variable value */
		   
/* process command line arguments */
void process_options(int argc,			/* number of arguments */
		    char **argv);		/* argument array */


/* find an option by name in the option database, returns NULL if not found */
Option* find_option(char *opt_name);	/* option name */
/*process the options specified in configuration file*/
void process_file(char *fname, int depth);
/* add option OPT to option database*/
void add_option(Option *opt);	/* option variable */

/* process command line arguments, returns index of next argument to parse */
int process_option(int index,		/* index of the first arg to parse */
	       int argc,		/* total number of arguments */
	       char **argv);		/* argument string array */

void __opt_process_options(int argc,			/* number of arguments */
		      char **argv,		/* argument array */
			   int depth);
void opt_process_options(int argc,			/* number of arguments */
		    char **argv);		/* argument array */ 
 void print();
  };
#endif /* OPTIONDB_H */

