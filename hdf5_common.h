#ifndef HDF5_COMMON
#define HDF5_COMMON

#include "hdf5.h"

// File path buffer length
#define path_buffer_size 256
// Empty pixels values
#define empty_pixel_value -1


// Generate mulatiple HDF5 files of specific size for testing purposes.
void generate_test_hdf5_files(int num_files, char filenames[][path_buffer_size], hsize_t dataset_size[], int n_dimensions, hsize_t chunk_size[]);
// Generate a single HDF5 file of specific size for testing purposes.
void generate_test_data_file(const char* filename, hsize_t dataset_size[], int n_dimensions, int fill_value, hsize_t chunk_size[]);
// Write array data to HDF5 file.
void write_to_file(const char* filename, hsize_t dataset_size[], int* array_data, int n_dimensions, hsize_t chunk_size[]);

// Generate virtual 3d dataset file.
void generate_3d_virtual_dataset_file(const char* filename,
                                      char sources[][path_buffer_size],
                                      hsize_t source_dataset_size[],
                                      hsize_t virtual_dataset_tiles_count[],
                                      hsize_t virtual_dataset_size[],
                                      int fill_value);

// Read 3d dataset file.
void read_3d_dataset_chunk(hid_t dataset_identifier, hid_t dataspace_identifier, hid_t memspace_identifier, int z_offset, hsize_t buffer_size[], int* read_buffer);
void read_3d_dataset_file(const char* filename, hsize_t dataset_size[], hsize_t buffer_size[]);
void read_3d_dataset_file_from_multiple_sources(char filenames[][path_buffer_size], hsize_t virtual_dataset_size[], hsize_t virtual_dataset_tiles_count[],
                                                hsize_t source_dataset_size[], hsize_t buffer_size[]);

#endif
