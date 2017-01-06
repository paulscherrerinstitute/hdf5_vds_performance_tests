#include "hdf5_test_common.h"
#include <stdio.h>

int main(int argc, char *argv[]){
    struct TestInputParameters input_parameters;
    if (parse_test_parameters(argc, argv, &input_parameters)){
        return -1;
    }

    char filenames[input_parameters.num_files][path_buffer_size];

    // Generate file names.
    for (int i=0; i<input_parameters.num_files; i++){
        sprintf(filenames[i], input_parameters.data_filename_template, i);
    }

    hsize_t chunk_size[] = {input_parameters.buffer_z_size, input_parameters.source_dataset_size[1], input_parameters.source_dataset_size[2]};

    // Generate test files.
    generate_test_hdf5_files(input_parameters.num_files, filenames, input_parameters.source_dataset_size, 3, chunk_size);

    // Create a virtual HDF5 file from all partial files.
    generate_3d_virtual_dataset_file(input_parameters.virtual_dataset_filename,
                                     filenames,
                                     input_parameters.source_dataset_size,
                                     input_parameters.virtual_dataset_tiles_count,
                                     input_parameters.virtual_dataset_size,
                                     empty_pixel_value);

    return 0;
}
