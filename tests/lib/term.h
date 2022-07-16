#ifndef LIB_TERM_H
#define LIB_TERM_H

#define CRST  "\x1B[0m"
#define CRED  "\x1B[31m"
#define CGRN  "\x1B[32m"
#define CYEL  "\x1B[33m"
#define CBLU  "\x1B[34m"
#define CMAG  "\x1B[35m"
#define CCYN  "\x1B[36m"
#define CWHT  "\x1B[37m"

#define psuccess(format, ...) \
  printf(CGRN format CRST __VA_OPT__(,) __VA_ARGS__)
#define pwarn(format, ...) \
  printf(CYEL format CRST __VA_OPT__(,) __VA_ARGS__)
#define pfail(format, ...) \
  printf(CRED format CRST __VA_OPT__(,) __VA_ARGS__)

#endif
