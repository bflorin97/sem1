

#include <stdio.h>
#include <omp.h>

void f(int id) {
    int myid;
    #pragma omp parallel private(myid) num_threads(8)
    {
        myid = omp_get_thread_num();
        #pragma omp for
        for (int j = 0; j < 8; j++) {
            printf("hello %d from %d\n", id, myid);
        }
    }
}

int main(void) {

    int id;


    #pragma omp parallel private(id) num_threads(8)
	{
        id = omp_get_thread_num();
    //
        #pragma omp master
        {
    //         for (int i = 0; i < 8; i++) {
                f(id);
    //         }
        }
    }

    return 0;
}
