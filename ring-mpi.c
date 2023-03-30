#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv) {
  int numproc;
  int miproc;
  float counter[] = {0};
  int N;
  int cycle;
  MPI_Status status;
  

  N = 3; /* Numero de ciclos que se realizarán */
  cycle = 1;  

  

  MPI_Init (&argc, &argv); /* Inicializar MPI */
  MPI_Comm_rank(MPI_COMM_WORLD,&miproc); /* Determinar el rango del proceso invocado */
  MPI_Comm_size(MPI_COMM_WORLD,&numproc); /* Determinar el numero de procesos */

  MPI_Barrier (MPI_COMM_WORLD);

  if (miproc == 0) {
    MPI_Send(&counter, sizeof(counter), MPI_CHAR, 1, 98, MPI_COMM_WORLD);
  }

  int offset = miproc;
  while (cycle <= N) {
    if (miproc == 0) {
    	MPI_Recv(&counter, sizeof(counter), MPI_CHAR, numproc - 1, 98, MPI_COMM_WORLD, &status);
    }
    else{
	MPI_Recv(&counter, sizeof(counter), MPI_CHAR, miproc - 1, 98, MPI_COMM_WORLD, &status);     
    }
    counter[0] += offset;
    // printf("%f, %i\n", counter[0], miproc);
    offset += numproc;

    if (miproc == (numproc - 1)) {
        MPI_Send(&counter, sizeof(counter), MPI_CHAR, 0, 98, MPI_COMM_WORLD);
    }
    else{
	MPI_Send(&counter, sizeof(counter), MPI_CHAR, miproc + 1, 98, MPI_COMM_WORLD);
    }

    cycle += 1;

  }

  if (miproc == 0) {
	printf("R: %f\n", counter[0]);
  	MPI_Abort(MPI_COMM_WORLD,0);
  }


  MPI_Finalize ();

  return 0;

}
