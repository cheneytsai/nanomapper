/* option variable classes */
enum option_type {
  oc_int = 0,		/* integer option */
  oc_uint,		/* unsigned integer option */
  oc_ulonglong,		/* unsigned long long option */
  oc_float,		/* float option */
  oc_double,		/* double option */
  oc_string		/* string option */
};

/* user-specified option definition */
class Option{
 public:
  Option *next;	/* next option */
  char *name;			/* option name, e.g., "-foo:bar" */
  char *desc;			/* option description */
  int nvars;			/* > 1 if var for list options */
  int *nelt;			/* number of elements parsed */
  enum option_type oc;		/* class of this option */
  int* int_var;	 		/*pointer to integer variable*/		
  unsigned int* uint_var;		/*pointer to unsigned integer variable*/
 unsigned long long* ulint_var; 	/*pointer to integer variable*/
 float* float_var;		/*pointer to float variable*/		
 double* double_var;	/*pointer to double variable*/		
 char** string_var;		/*pointer to string variable*/		
 Option(){};

};

