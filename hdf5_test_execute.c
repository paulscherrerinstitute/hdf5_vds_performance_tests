#include "hdf5_test_common.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    struct TestInputParameters input_parameters;
    if (parse_test_parameters(argc, argv, &input_parameters)){
        return -1;
    }

    // Generate file names.
    char filenames[input_parameters.num_files][path_buffer_size];
    for (int i=0; i<input_parameters.num_files; i++){
        sprintf(filenames[i], input_parameters.data_filename_template, i);
    }

    hsize_t source_buffer_size[] = {input_parameters.buffer_z_size, input_parameters.source_dataset_size[1], input_parameters.source_dataset_size[2]};

    // Measure the time needed to read from the separate files.
    struct timespec separate_start = timer_start();
        read_3d_dataset_file_from_multiple_sources(filenames, input_parameters.virtual_dataset_size, input_parameters.virtual_dataset_tiles_count,
                                                   input_parameters.source_dataset_size, source_buffer_size);
    long separate_micro_seconds = timer_end(separate_start);

    hsize_t virtual_buffer_size[] = {input_parameters.buffer_z_size, input_parameters.virtual_dataset_size[1], input_parameters.virtual_dataset_size[2]};

    // Measure the time needed to read the virtual dataset.
    struct timespec virtaul_start = timer_start();
        read_3d_dataset_file(input_parameters.virtual_dataset_filename, input_parameters.virtual_dataset_size, virtual_buffer_size);

    long virtual_micro_seconds = timer_end(virtaul_start);

#if defined(DEBUG) || defined(HUMAN)
    printf("Separate files read time: %ld us\n", separate_micro_seconds);
    printf("Virtual dataset read time: %ld us.\n", virtual_micro_seconds);
#else
    printf("%d,%d,%d,%d,%d,%d,%d,%d,%ld,%ld\n",
           input_parameters.num_files,
           input_parameters.source_x_size,
           input_parameters.source_y_size,
           input_parameters.source_z_size,
           input_parameters.tiles_x_count,
           input_parameters.tiles_y_count,
           input_parameters.tiles_z_count,
           input_parameters.buffer_z_size,
           separate_micro_seconds,
           virtual_micro_seconds);
#endif

    return 0;
}
