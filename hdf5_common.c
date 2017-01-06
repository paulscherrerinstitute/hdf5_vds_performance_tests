#include "hdf5_common.h"
#include <stdio.h>
#include <stdlib.h>


void generate_test_hdf5_files(int num_files, char filenames[][path_buffer_size], hsize_t dataset_size[], int n_dimensions, hsize_t chunk_size[]){
    for (int i=0; i<num_files; i++){
        generate_test_data_file(filenames[i], dataset_size, n_dimensions, i, chunk_size);
    }
}


void generate_test_data_file(const char* filename, hsize_t dataset_size[], int n_dimensions, int fill_value, hsize_t chunk_size[]){
#ifdef DEBUG
    printf("HDF5 File generator: %s, %d\n------------------\n", filename, fill_value);
#endif

    long buffer_length = 1;
    for (int i=0; i<n_dimensions; i++){
        buffer_length = buffer_length * dataset_size[i];
    }

    int* array_data = malloc(sizeof(int) * buffer_length);
    for (long i=0; i<buffer_length; i++){
        array_data[i] = fill_value;
    }

    // Write the data to a HDF5 file.
    write_to_file(filename, dataset_size, array_data, n_dimensions, chunk_size);

    free(array_data);
}


void write_to_file(const char* filename, hsize_t dataset_size[], int* array_data, int n_dimensions, hsize_t chunk_size[]){
    // Create a new empty file.
    hid_t file_identifier = H5Fcreate(filename,
                                      H5F_ACC_TRUNC, // Truncate existing file.
                                      H5P_DEFAULT,   // File creation property list.
                                      H5P_DEFAULT);  // File access property list.



    // Create a new data space.
    hid_t dataspace_indentifier = H5Screate_simple(n_dimensions,
                                                   dataset_size,
                                                   NULL);         // Maximum size of dimensions.

    // Set dataset chunking policy.
    hid_t dataset_creation_identifier = H5Pcreate(H5P_DATASET_CREATE);
    H5Pset_chunk(dataset_creation_identifier, n_dimensions, chunk_size);

    // Create a new dataset.
    hid_t dataset_identifier = H5Dcreate2(file_identifier,
                                          "image_data",     // Name of the dataset.
                                          H5T_NATIVE_INT,
                                          dataspace_indentifier,
                                          H5P_DEFAULT,      // Link creation properties.
                                          dataset_creation_identifier,
                                          H5P_DEFAULT);     // Dataset access properties.


    // Write data to the new dataset.
    H5Dwrite(dataset_identifier,
             H5T_NATIVE_INT,
             H5S_ALL,
             H5S_ALL,
             H5P_DEFAULT,
             array_data);

    H5Pclose(dataset_creation_identifier);
    H5Dclose(dataset_identifier);
    H5Sclose(dataspace_indentifier);
    H5Fclose(file_identifier);
}

void generate_3d_virtual_dataset_file(const char* filename,
                                      char sources[][path_buffer_size],
                                      hsize_t source_dataset_size[],
                                      hsize_t virtual_dataset_tiles_count[],
                                      hsize_t virtual_dataset_size[],
                                      int fill_value){
#ifdef DEBUG
    printf("HDF5 Virtual dataset generator: %s\n------------------\n", filename);
#endif
    hid_t virtual_file_identifier = H5Fcreate(filename,
                                              H5F_ACC_TRUNC, // Truncate existing file.
                                              H5P_DEFAULT,   // File creation property list.
                                              H5P_DEFAULT);  // File access property list.

    hid_t virtual_dataspace_indentifier = H5Screate_simple(3,
                                                           virtual_dataset_size,
                                                           NULL);

    // Create and fill empty parameters.
    hid_t virtaul_dataset_create_property_identifier = H5Pcreate(H5P_DATASET_CREATE);
    H5Pset_fill_value(virtaul_dataset_create_property_identifier, H5T_NATIVE_INT, &fill_value);

    // Common hyperslab parameters.
    hsize_t virtual_dataset_stride[] = {1, 1, 1};
    hsize_t virtual_dataset_blocks[] = {1, 1, 1};

    // Source files dataspace sizes.
    hid_t source_dataspace_indentifier = H5Screate_simple(3, source_dataset_size, NULL);

    // Create all hyperslabs.
    int current_file_index = 0;
    for (int z=0; z<virtual_dataset_tiles_count[0]; z++){
        for (int y=0; y<virtual_dataset_tiles_count[1]; y++){
            for (int x=0; x<virtual_dataset_tiles_count[2]; x++){
                hsize_t start_offset[] = {z*source_dataset_size[0], y*source_dataset_size[1], x*source_dataset_size[2]};

                H5Sselect_hyperslab(virtual_dataspace_indentifier,
                                    H5S_SELECT_SET,
                                    start_offset,
                                    virtual_dataset_stride,
                                    virtual_dataset_blocks,
                                    source_dataset_size);

                H5Pset_virtual(virtaul_dataset_create_property_identifier,
                               virtual_dataspace_indentifier,
                               sources[current_file_index],
                               "image_data",                        // Source dataset name.
                               source_dataspace_indentifier);

                current_file_index++;
            }
        }
    }

    hid_t virtual_data_set_identifier = H5Dcreate2(virtual_file_identifier,
                                                   "image_data",
                                                   H5T_NATIVE_INT,
                                                   virtual_dataspace_indentifier,
                                                   H5P_DEFAULT,                                 // Link properties.
                                                   virtaul_dataset_create_property_identifier,
                                                   H5P_DEFAULT);                                // Access properties.

    H5Dclose(virtual_data_set_identifier);
    H5Sclose(source_dataspace_indentifier);
    H5Pclose(virtaul_dataset_create_property_identifier);
    H5Sclose(virtual_dataspace_indentifier);
    H5Fclose(virtual_file_identifier);
}

void read_3d_dataset_file(const char* filename, hsize_t dataset_size[], hsize_t buffer_size[]){
    int buffer_length = buffer_size[0]*buffer_size[1]*buffer_size[2];
    int* read_buffer = malloc(sizeof(int)*buffer_length);

    hid_t file_identifier = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);
    hid_t dataset_identifier = H5Dopen2 (file_identifier, "image_data", H5P_DEFAULT);
    hid_t dataspace_identifier = H5Dget_space(dataset_identifier);
    hid_t memspace_identifier = H5Screate_simple (3, buffer_size, NULL);

    // Block read parameters.
    hsize_t stride[] = {1, 1, 1};
    hsize_t block[] = {1, 1, 1};
    hsize_t offset[] = {0, 0, 0};

    // Read the file buffer by buffer.
    int n_of_chunks = dataset_size[0] / buffer_size[0];
    for (int z=0; z<n_of_chunks; z++){
        offset[0] = z*buffer_size[0];
        H5Sselect_hyperslab (dataspace_identifier, H5S_SELECT_SET, offset, stride, buffer_size, block);
        H5Dread(dataset_identifier, H5T_NATIVE_INT, memspace_identifier, dataspace_identifier, H5P_DEFAULT, &read_buffer[0]);

        #ifdef DEBUG
            // Print frames to console.
            int n_frames_in_chunk = dataset_size[0] / n_of_chunks;
            for (int f=0; f<n_frames_in_chunk; f++){
                int current_frame = (z*n_frames_in_chunk) + f;
                printf("Frame %d, File %s\n", current_frame, filename);
                for (int y=0; y<dataset_size[1]; y++){
                    for (int x=0; x<dataset_size[2]; x++){
                        int offset = (f*dataset_size[1]*dataset_size[2]) + (y*dataset_size[2]) + x;
                        printf("%d ", read_buffer[offset]);
                    }
                    printf("\n");
                }
                printf("\n");
            }
        #endif
    }

    H5Sclose(memspace_identifier);
    H5Sclose(dataspace_identifier);
    H5Dclose(dataset_identifier);
    H5Fclose(file_identifier);
    free(read_buffer);
}
