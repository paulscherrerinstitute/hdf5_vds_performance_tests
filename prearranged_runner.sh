#!/bin/bash
output_file_name="results/prearranged_results.csv"


echo "Creating temp/ and results/ folder. Delete temp/ folder when tests are completed."
mkdir -p temp/ results/

echo "num_files,source_x_size,source_y_size,source_z_size,tiles_x_count,tiles_y_count,tiles_z_count,buffer_z_size,separate_time_us,virtual_time_us" > $output_file_name

n_frames_per_file=1000;
tile_dimension_length=1024;
frame_dimension_length=2048;

echo "Executing tests for VDS read time."

echo "     Generating input files for VDS read time."

# Generate the input files.
./bin/hdf5_test_prepare temp/virtual.hdf5 temp/source_%d.hdf5 $tile_dimension_length $tile_dimension_length $n_frames_per_file 2 2 1 500

# Repeat the measurments 10 times to have an average time.
for repeat_measurment in {1..10}
do
    echo "     Executing test repetition "$repeat_measurment

    # Execute tests and dump result into CSV file.
    ./bin/hdf5_test_execute temp/virtual.hdf5 temp/source_%d.hdf5 $tile_dimension_length $tile_dimension_length $n_frames_per_file 2 2 1 500 >> $output_file_name
done

echo "Executing tests for prearranged read time."

echo "     Generating input files for prearranged read time."

# Generate the input files.
./bin/hdf5_test_prepare temp/virtual.hdf5 temp/source_%d.hdf5 $frame_dimension_length $frame_dimension_length $n_frames_per_file 1 1 1 1

# Repeat the measurments 10 times to have an average time.
for repeat_measurment in {1..10}
do
    echo "     Executing test repetition "$repeat_measurment

    # Execute tests and dump result into CSV file.
    ./bin/hdf5_test_execute temp/virtual.hdf5 temp/source_%d.hdf5 $frame_dimension_length $frame_dimension_length $n_frames_per_file 1 1 1 1 >> $output_file_name
done
