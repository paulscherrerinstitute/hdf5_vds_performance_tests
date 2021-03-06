#!/bin/bash
output_file_name="results/2d_tile_results.csv"


echo "Creating temp/ and results/ folder. Delete temp/ folder when tests are completed."
mkdir -p temp/ results/

echo "num_files,source_x_size,source_y_size,source_z_size,tiles_x_count,tiles_y_count,tiles_z_count,buffer_z_size,separate_time_us,virtual_time_us" > $output_file_name

n_frames_per_file=1;
frame_dimension_length=128;
frame_increase_step=128;
for i in {0..6}
do
    # Dimension of image (x and y)
    frame_dimension_length=$(($frame_dimension_length+$frame_increase_step));
    echo "Executing tests for image size "$frame_dimension_length"x"$frame_dimension_length

    for x_tile_count in {1..8}
    do
	for y_tile_count in {1..8}
    	do
		echo "     Generating input files for tile count "$x_tile_count"x"$y_tile_count

		# Generate the input files.
		./bin/hdf5_test_prepare temp/virtual.hdf5 temp/source_%d.hdf5 $frame_dimension_length $frame_dimension_length $n_frames_per_file $x_tile_count $y_tile_count 1 1

		# Repeat the measurments 10 times to have an average time.
		for repeat_measurment in {1..10}
		do
		    echo "     Executing tests for tile count "$x_tile_count"x"$y_tile_count

		    # Execute tests and dump result into CSV file.
		    ./bin/hdf5_test_execute temp/virtual.hdf5 temp/source_%d.hdf5 $frame_dimension_length $frame_dimension_length $n_frames_per_file $x_tile_count $y_tile_count 1 1 >> $output_file_name
		done
	done
    done
done
