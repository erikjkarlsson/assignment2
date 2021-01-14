
#include "option.h"


ioopm_option_t ioopm_option_success(elem_t v)
{
  return (ioopm_option_t) { .success = true, .value = v };
}
ioopm_option_t ioopm_option_failure()
{       
  return (ioopm_option_t) { .success = false };
}
bool ioopm_option_successful(ioopm_option_t o)
{   
  return (o.success == true);
}
bool ioopm_option_unsuccessful(ioopm_option_t o)
{ 
  return (o.success == false);
}