
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <cctype>

#include "OptionDB.hh"
using namespace std;
/* create a new option database */
OptionDB::OptionDB()   
{
  options = NULL;
  /* builtin options */
   builtin_options = new Option();
    builtin_options->name="-config";
  builtin_options->desc="load";
  builtin_options->nvars=0;
  builtin_options->nelt=NULL;
  builtin_options->oc=oc_string;
  builtin_options->next=NULL;
}
OptionDB::~OptionDB()
{
  delete builtin_options;
  Option *temp=options;
  Option *temp1=temp->next;
    while(temp->next != NULL)
    {
      delete temp;
      temp=temp1;
      temp1=temp->next;
    }
    delete(temp);
}
//print out all the options
void OptionDB::print()
{
  Option* opt;
  for(opt=options;opt!=NULL;opt=opt->next)
    {
      cout << opt->name<<": "<<opt->desc;
      int i;
      for(i=0;i<opt->nvars;i++)
	{
	  switch(opt->oc)
	    {
	    case oc_int:
	      cout<<" "<<opt->int_var[i];
	      break;
	    case oc_uint:
	      cout<<" "<<opt->uint_var[i];
	      break;
	    case oc_ulonglong:
	       cout<<" "<<opt->ulint_var[i];
	      break;
	    case oc_float:
	       cout<<" "<<opt->float_var[i];
	       break;	
	    case oc_double:
	      cout<<" "<<opt->double_var[i];
	      break;
	    case oc_string:
	      if(opt->string_var[i] != NULL)
		cout<<" "<<opt->string_var[i];
	      break;
	    }
	  cout << endl;
	}
    }
}

/* add option OPT to option database ODB */
void OptionDB::add_option(Option *opt)	/* option variable */
{
  Option *elt, *prev;
  /* sanity checks on option name */
    
  //  if (opt->name[0] != '-')
  //    cout << "option "<<opt->name<<" must start with a `-'"<<endl;

  /* add to end of option list */
  for (prev=NULL, elt=options; elt != NULL; prev=elt, elt=elt->next)
    {
      /* sanity checks on option name */
      if (elt->name[0] == opt->name[0] && !strcmp(elt->name, opt->name))
	cout << "option "<<opt->name<<" is multiply defined"<<endl;
    }
  if (prev != NULL)
    prev->next = opt;
  else /* prev == NULL */
    options = opt;
  opt->next = NULL;
}

/* register an integer option variable */
void OptionDB::reg_int(char *name,			/* option name */
	    char *desc,			/* option description */
	    int *var,			/* target variable */
	    int def_val)		/* default variable value */
{
  Option *opt = new Option();
  opt->name=name;
  opt->desc=desc;
  opt->nvars=1;
  opt->nelt=NULL;
  opt->oc=oc_int;

  /* place on ODB's option list */
  opt->int_var=var;
  opt->next = NULL;
  add_option(opt);
  /* set default value */

  *var = def_val;
}

/* register an integer option list */
void OptionDB::reg_int_list(char *name,		/* option name */
		 char *desc,		/* option description */
		 int *vars,		/* pointer to option array */
		 int nvars,		/* total entries in option array */
		 int *nelt,		/* number of entries parsed */
		 int *def_val)		/* default value of option array */
{
  int i;
  Option *opt = new Option();
  opt->name=name;
  opt->desc=desc;
  opt->nvars=nvars;
  opt->nelt=nelt;
  opt->oc=oc_int;
  opt->int_var=vars;
   /* place on ODB's option list */
  opt->next = NULL;
  add_option(opt);
  /* set default value */
  for (i=0; i < *nelt; i++)
    vars[i] = def_val[i];
}

/* register an unsigned integer option variable */
void OptionDB::reg_uint(char *name,		/* option name */
	     char *desc,		/* option description */
	     unsigned int *var,		/* pointer to option variable */
	     unsigned int def_val)	/* default value of option variable */
{
    Option *opt = new Option();
   opt->name=name;
  opt->desc=desc;
  opt->nvars=1;
  opt->nelt=NULL;
  opt->oc=oc_uint;
  opt->uint_var = var;
   /* place on ODB's option list */
  opt->next = NULL;
  add_option(opt);
  /* set default value */
  *var = def_val;
}

/* register an unsigned integer option list */
void OptionDB::reg_uint_list(char *name,		/* option name */
		  char *desc,		/* option description */
		  unsigned int *vars,	/* pointer to option array */
		  int nvars,		/* total entries in option array */
		  int *nelt,		/* number of elements parsed */
		  unsigned int *def_val)/* default value of option array */
{
  int i;
    Option *opt = new Option();
   opt->name=name;
  opt->desc=desc;
  opt->nvars=nvars;
  opt->nelt=nelt;
  opt->oc=oc_uint;
   opt->uint_var = vars;
   /* place on ODB's option list */
  opt->next = NULL;
  add_option(opt);
  /* set default value */
  for (i=0; i < *nelt; i++)
    vars[i] = def_val[i];
}

/* register an unsigned long long option variable */
void OptionDB::reg_ulonglong(char *name,		/* option name */
	       char *desc,		/* option description */
	       unsigned long long *var,	/* target variable */
	       unsigned long long def_val)/* default variable value */
{
    Option *opt = new Option();
   opt->name=name;
  opt->desc=desc;
  opt->nvars=1;
  opt->nelt=NULL;
  opt->oc=oc_ulonglong;
   opt->ulint_var = var;
   /* place on ODB's option list */
  opt->next = NULL;
  add_option(opt);
  /* set default value */
  *var = def_val;
}


/* register a single-precision floating point option variable */
void OptionDB::reg_float(char *name,		/* option name */
	      char *desc,		/* option description */
	      float *var,		/* target option variable */
	      float def_val)		/* default variable value */
{
    Option *opt = new Option();
   opt->name=name;
  opt->desc=desc;
  opt->nvars=1;
  opt->nelt=NULL;
  opt->oc=oc_float;
  opt->float_var = var;
  /* place on ODB's option list */
  opt->next = NULL;
  add_option(opt);
  /* set default value */
  *var = def_val;
}

/* register a single-precision floating point option array */
void OptionDB::reg_float_list(char *name,		/* option name */
		   char *desc,		/* option description */
		   float *vars,		/* target array */
		   int nvars,		/* target array size */
		   int *nelt,		/* number of args parsed goes here */
		   float *def_val)	/* default variable value */
{
  int i;
    Option *opt = new Option();
   opt->name=name;
  opt->desc=desc;
  opt->nvars=nvars;
  opt->nelt=nelt;
  opt->oc=oc_float;
  opt->float_var = vars;
  
  /* place on ODB's option list */
  opt->next = NULL;
  add_option(opt);

  /* set default value */
  for (i=0; i < *nelt; i++)
    vars[i] = def_val[i];
}

/* register a double-precision floating point option variable */
void OptionDB::reg_double(char *name,		/* option name */
	       char *desc,		/* option description */
	       double *var,		/* target variable */
	       double def_val)		/* default variable value */
{
    Option *opt = new Option();
   opt->name=name;
  opt->desc=desc;
  opt->nvars=1;
  opt->nelt=NULL;
  opt->oc=oc_double;
  opt->double_var = var;

  /* place on ODB's option list */
  opt->next = NULL;
  add_option(opt);

  /* set default value */
  *var = def_val;
}

/* register a double-precision floating point option array */
void OptionDB::reg_double_list(char *name,		/* option name */
		    char *desc,		/* option description */
		    double *vars,	/* target array */
		    int nvars,		/* target array size */
		    int *nelt,		/* number of args parsed goes here */
		    double *def_val)	/* default variable value */
{
  int i;
  Option *opt;
  opt = new Option();
    opt->name=name;
  opt->desc=desc;
  opt->nvars=nvars;
  opt->nelt=nelt;
  opt->oc=oc_double;
  opt->double_var = vars;
 
  /* place on ODB's option list */
  opt->next = NULL;
  add_option(opt);

  /* set default value */
  for (i=0; i < *nelt; i++)
    vars[i] = def_val[i];
}

/* register a string option variable */
void OptionDB::reg_string(char *name,		/* option name */
	       char *desc,		/* option description */
	       char **var,		/* pointer to string option variable */
	       char *def_val)		/* default variable value */
{
  Option *opt;
  opt = new Option();
    opt->name=name;
  opt->desc=desc;
  opt->nvars=1;
  opt->nelt=NULL;
  opt->oc=oc_string;
  opt->string_var = var;

  /* place on ODB's option list */
  opt->next = NULL;
  add_option(opt);

  /* set default value */
  *var = def_val;
}

/* register a string option array */
void OptionDB::reg_string_list(char *name,		/* option name */
		    char *desc,		/* option description */
		    char **vars,	/* pointer to option string array */
		    int nvars,		/* target array size */
		    int *nelt,		/* number of args parsed goes here */
		    char **def_val)	/* default variable value */
{
  int i;
  Option *opt;
  opt = new Option();
   opt->name=name;
  opt->desc=desc;
  opt->nvars=nvars;
  opt->nelt=nelt;
  opt->oc=oc_string;

  opt->string_var = vars;

  /* place on ODB's option list */
  opt->next = NULL;
  add_option(opt);

  /* set default value */
  for (i=0; i < *nelt; i++)
    vars[i] = def_val[i];
}

/* process command line arguments, returns index of next argument to parse */
int OptionDB::process_option(int index,		/* index of the first arg to parse */
	       int argc,		/* total number of arguments */
	       char **argv)		/* argument string array */
{
  int cnt, ent, nvars;
  char *endp;
  double tmp;
  Option *opt;

  /* locate the option in the option database */
  for (opt=options; opt != NULL; opt=opt->next)
    {
      if (!strcmp(opt->name, argv[index]))
	break;
    }
  if (!opt)
    {
      /* no one registered this option */
      cout << "option "<<argv[index]<<" is undefined"<<endl;
    }
  index++;

  /* process option arguments */
  switch (opt->oc)
    {
    case oc_int:
      /* this option needs at least one argument */
      if (index >= argc
	  || (argv[index][0] == '-' && !isdigit((int)argv[index][1])))
	{
	  /* no arguments available */
	  cout<<"option "<<opt->name<<" requires an argument"<<endl;
	}
      cnt = 0;
      ent = 0;
      if (opt->nelt)
	*opt->nelt = 0;
      nvars = opt->nvars;
      /* parse all arguments */
      while (index < argc && cnt < nvars &&
	     (argv[index][0] != '-' || isdigit((int)argv[index][1])))
	{
	  opt->int_var[ent] = strtol(argv[index], &endp, 0);
	  if (*endp)
	    {
	      /* could not parse entire argument */
	      cout<<"could not parse argument "<<argv[index]<<" of option "<<opt->name<<endl;
	    }
	  /* else, argument converted correctly */
	  if (opt->nelt)
	    (*opt->nelt)++;
	  cnt++; index++; ent++;
	}
      break;
    case oc_uint:
      /* this option needs at least one argument */
      if (index >= argc || argv[index][0] == '-')
	{
	  /* no arguments available */
	  cout << "option "<<opt->name<<" requires an argument"<<endl;
	}
      cnt = 0;
      ent = 0;
      if (opt->nelt)
	*opt->nelt = 0;
      nvars = opt->nvars;
      /* parse all arguments */
      while (index < argc && cnt < nvars && argv[index][0] != '-')
	{
	  opt->uint_var[ent] = strtoul(argv[index], &endp, 0);
	  if (*endp)
	    {
	      /* could not parse entire argument */
	      cout<<"could not parse argument "<<argv[index]<<" of option "<< opt->name<<endl;
	    }
	  /* else, argument converted correctly */
	  if (opt->nelt)
	    (*opt->nelt)++;
	  cnt++; index++; ent++;
	}
      break;
    case oc_ulonglong:
      /* this option needs at least one argument */
      if (index >= argc || argv[index][0] == '-')
	{
	  /* no arguments available */
	  cout<< "option "<<opt->name<<" requires an argument"<<endl;
	}
      cnt = 0;
      ent = 0;
      if (opt->nelt)
	*opt->nelt = 0;
      nvars = opt->nvars;
      /* parse all arguments */
      while (index < argc && cnt < nvars && argv[index][0] != '-')
	{
	  opt->ulint_var[ent] = (long long unsigned int)strtod(argv[index], &endp);
	  if (*endp)
	    {
	      /* could not parse entire argument */
	      cout<< "could not parse argument "<<argv[index]<<" of option "<<opt->name<<endl;
	    }
	  /* else, argument converted correctly */
	  if (opt->nelt)
	    (*opt->nelt)++;
	  cnt++; index++; ent++;
	}
      break;
    case oc_float:
      /* this option needs at least one argument */
      if (index >= argc
	  || (argv[index][0] == '-' && !isdigit((int)argv[index][1])))
	{
	  /* no arguments available */
	  cout<<"option "<<opt->name<<" requires an argument"<<endl;
	}
      cnt = 0;
      ent = 0;
      if (opt->nelt)
	*opt->nelt = 0;
      nvars = opt->nvars;
      /* parse all arguments */
      while (index < argc && cnt < nvars &&
	     (argv[index][0] != '-' || isdigit((int)argv[index][1])))
	{
	  tmp = strtod(argv[index], &endp);
	  if (*endp)
	    {
	      /* could not parse entire argument */
	      cout<<"could not parse argument "<<argv[index]<<" of option "<<opt->name<<endl;
	    }
	  /* else, argument converted correctly */
	  opt->float_var[ent] = (float)tmp;
	  if (opt->nelt)
	    (*opt->nelt)++;
	  cnt++; index++; ent++;
	}
      break;
    case oc_double:
      /* this option needs at least one argument */
      if (index >= argc
	  || (argv[index][0] == '-' && !isdigit((int)argv[index][1])))
	{
	  /* no arguments available */
	  cout<<"option "<<opt->name<<" requires an argument"<<endl;
	}
      cnt = 0;
      ent = 0;
      if (opt->nelt)
	*opt->nelt = 0;
      nvars = opt->nvars;
      /* parse all arguments */
      while (index < argc && cnt < nvars &&
	     (argv[index][0] != '-' || isdigit((int)argv[index][1])))
	{
	  opt->double_var[ent] = strtod(argv[index], &endp);
	  if (*endp)
	    {
	      /* could not parse entire argument */
	      cout<<"could not parse argument "<<argv[index]<<" of option "<<opt->name<<endl;
	    }
	  /* else, argument converted correctly */
	  if (opt->nelt)
	    (*opt->nelt)++;
	  cnt++; index++; ent++;
	}
      break;
    case oc_string:
      /* this option needs at least one argument */
      if (index >= argc || argv[index][0] == '-')
	{
	  /* no arguments available */
	  cout<<"option "<<opt->name<<"` requires an argument"<<endl;
	}
      cnt = 0;
      ent = 0;
      if (opt->nelt)
	*opt->nelt = 0;
      nvars = opt->nvars;
      /* parse all arguments */
      while (index < argc && cnt < nvars && argv[index][0] != '-')
	{
	  opt->string_var[ent] = argv[index];
	  if (opt->nelt)
	    (*opt->nelt)++;
	  cnt++; index++; ent++;
	}
      break;
    default:
      cout<<"bogus option class"<<endl;
    }

  return index;
}

/* process a command line, internal version that tracks `-config' depth */
void OptionDB::__opt_process_options(int argc,			/* number of arguments */
		      char **argv,		/* argument array */
		      int depth)		/* `-config' option depth */
{
  int index;
  index = 0;
   /* visit all command line arguments */
  while (index < argc)
    {
     
      /* done with command line? */
      if (index == argc)
	{
	  /* done processing command line */
	  break;
	}
      /* when finished, argv[index] is an option to parse */

      /* process builtin options */
      if (!strcmp(argv[index], "-config"))
	{
	  /* handle `-config' builtin option */
	  index++;
	  if (index >= argc || argv[index][0] == '-')
	    {
	      /* no arguments available */
	      cout << "option `-config' requires an argument"<<endl;
	    }
	  process_file(argv[index], depth);
	  index++;
	}
      else
	{
	  /* process user-installed option */
	  index = process_option(index, argc, argv);
	}
    }
}

/* process command line arguments */
void OptionDB::opt_process_options(int argc,			/* number of arguments */
		    char **argv)		/* argument array */
{
  /* need at least two entries to have an option */
  if (argc < 2)
    return;

  /* process the command, starting at `-config' depth zero */
  __opt_process_options(argc-1, argv+1, /* top level */0);
}


void OptionDB::process_file(char *fname, int depth)
{
  int largc;
  char *largv[MAX_LINE_ARGS];
  char line[1024], *p, *q;
  ifstream in(fname);
  if (!in)
    cout<<"could not open configuration file "<<fname<<endl;

  while (in)
    {
      line[0] = '\n';
      /* read a line from the file and chop */
      in.getline(line, 1024);
      if (line[0] == '\0' || line[0] == '\n')
	continue;
      if (line[strlen(line)-1] == '\n')
	line[strlen(line)-1] = '\0';

      /* process one line from the file */
      largc = 0; p = line;
      while (*p)
	{
	  /* skip whitespace */
	  while (*p != '\0' && (*p == ' ' || *p == '\t'))
	    p++;

	  /* ignore empty lines and comments */
	  if (*p == '\0' || *p == '#')
	    break;

	  /* skip to the end of the argument */
	  q = p;
	  while (*q != '\0' && *q != ' ' && *q != '\t')
	    q++;
	  if (*q)
	    {
	      *q = '\0';
	      q++;
	    }

	  /* marshall an option array */
	  largv[largc++] = strdup(p);

	  if (largc == MAX_LINE_ARGS)
	    {
	      cout << "option line too complex in file "<< fname<<endl;
	    }
	  /* go to next argument */
	  p = q;
	}
      /* process the line */
      if (largc > 0)
	__opt_process_options(largc, largv, depth+1);
      /* else, empty line */
      int i=0;
      while(i<largc)
	free(largv[i++]);
    }

  in.close();

}

/* find an option by name in the option database, returns NULL if not found */
Option* OptionDB::find_option(char *opt_name)	/* option name */
{
  Option *opt;

  /* search builtin options */
  for (opt = builtin_options; opt != NULL; opt = opt->next)
    {
      if (!strcmp(opt->name, opt_name))
	{
	  /* located option */
	  return opt;
	}
    }

  /* search user-installed options */
  for (opt = options; opt != NULL; opt = opt->next)
    {
      if (!strcmp(opt->name, opt_name))
	{
	  /* located option */
	  return opt;
	}
    }
  /* not found */
  return NULL;
}


