
#include <stdio.h>
#include "ss_internal.h"

void ss_set_prompt(int sci_idx, char *new_prompt)
{
     ss_info(sci_idx)->prompt = new_prompt;
}

char *ss_get_prompt(int sci_idx)
{
     return(ss_info(sci_idx)->prompt);
}