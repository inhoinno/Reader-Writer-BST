#include "include/lab2_sync_types.h"


/*
TODO 
    Implement a fuction that creates rwstride object

    @param                  : void
    @return                 : a pointer of rwstride object 
*/
rwstride_t * rw_stride_create_stride()
{
    rwstride_t * t = (rwstride_t *) malloc(sizeof(rwstride_t));
    return t;
}

/*
TODO 
    Implement a fuction that initialize rwstride object with read&write percentage

    @param rwstride         : a rwstride object that contains stride & pass val of rw
    @param p_read           : a percentage of readers on Workload
    @param p_writer         : a percentage of writers on Workload
    @return                 : status(success , failed) 
*/
int rw_stride_init_stride(rwstride_t * rwstride, int p_read, int p_writer)
{
    rwstride->reader_stride = (p_read + p_writer) / p_read ;
    rwstride->writer_stride = (p_read + p_writer) / p_writer;
    printf("/*** STRIDE SET : (r:%d , w:%d) ***/\n",rwstride->reader_stride,rwstride->writer_stride);
    rwstride->reader_pass  = 0;
    rwstride->writer_pass = 0;
    return 1;
}

/*
TODO 
    Implement a fuction that schedule reader or wrtier
    base on pass and stride of rw that defined in object rwstride_t

    @param rwstride         : a rwstride object that contains stride & pass val of rw
    @return                 : (reader : 1 , writer  : 0) 
*/
int rw_stride_schedule(rwstride_t * rwstride)
{
    int ret = 0;
    if(rwstride->writer_pass == rwstride->reader_pass)
        rw_stride_reset(rwstride);
    if(rwstride->reader_pass < rwstride->writer_pass){
        rwstride->reader_pass += rwstride->reader_stride;
        return 1;
    }else
    {
        rwstride->writer_pass += rwstride->writer_stride;
        return 0;
    }
}
/*
TODO
    Implement a function that set pass value to 0 (reset + to avoid integer overflow)
    @param rwstride         : rwstride_t object that need to reset
    @return                 : status(success , failed)
*/
int rw_stride_reset(rwstride_t * rwstride)
{
    rwstride->reader_pass = 0;
    rwstride->writer_pass = 0;
    return 0;
}
/*
TODO
    Implement a function that de-allocate rwstride
    @param rwstride         : rwstride_t object that need to destroy
    @return                 : status(success , failed)
*/
int rw_stride_destroy(rwstride_t * rwstride)
{
    free(rwstride);
    rwstride = NULL;
    return 0;
}