// To pass two arguments, we need a struct to hold them.
typedef struct bzeroparams {
    unsigned char *buffer;
    int length;
};

#define NTHREADS 10

// Zero a block of memory using multiple threads. 
void blockzero (unsigned char *p, int length){
    int i, j;
    thread_t threads[NTHREADS];
    struct bzeroparams params[NTHREADS];

    // For simplicity, assumes length is divisibly by NTHREADS.
    assert ((length%NTHREADS) == 0);
    for(i = 0, j=0; i < NTHREADS; i++, j +=length/NTHREADS){
        params[i].buffer = p + i *length/NTHREADS;
        params[i].length = length/NTHREADS;
        thread_create_p(&(threads[i]), &go, &params[i]);
    }
    for(i = 0; i < NTHREADS; i++){
        thread_join(threads[i]);
    }
}