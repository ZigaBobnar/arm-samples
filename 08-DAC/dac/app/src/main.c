#include <asf.h>

#ifdef __cplusplus
extern "C" {
#endif


int main (void)
{

    /* sets the processor clock according to conf_clock.h definitions */
    sysclk_init();

    /* disable wathcdog */
    WDT->WDT_MR = WDT_MR_WDDIS;

    /********************* HW init     ***************************/

    /********************* Main loop     ***************************/
    while(1)
    {
    }
}


#ifdef __cplusplus
}
#endif
