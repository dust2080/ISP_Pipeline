#include <iostream>
#include <omp.h>

int main() {
    std::cout << "Max threads: " << omp_get_max_threads() << std::endl;
    
    #pragma omp parallel
    {
        #pragma omp single
        std::cout << "Running with " << omp_get_num_threads() << " threads" << std::endl;
    }
    return 0;
}
