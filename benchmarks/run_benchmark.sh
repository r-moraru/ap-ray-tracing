#!/bin/bash

executable=./render.out
num_runs=5
output_file=./benchmarks/benchmark_results.csv

## ensure latest version of executable
make

echo "num_processors,run,topology,load_balanced,time" >$output_file

processors=(1 2 4 8 16 18 32)
configurations=("grid" "grid_nlb" "ring" "ring_nlb" "linear")

for np in "${processors[@]}"; do
	for config in "${configurations[@]}"; do
		for run in $(seq 1 $num_runs); do
			echo "Running with $np processors, configuration $config, run $run"
			output=$(mpirun --oversubscribe -np $np $executable $config no)
			time=$(echo "$output" | grep "Elapsed time" | awk '{print $3}')
			topology="grid"
			load_balanced="true"
			if [ "$config" == "grid_nlb" ]; then
				topology="grid"
				load_balanced="false"
			elif [ "$config" == "ring" ]; then
				topology="ring"
				load_balanced="true"
			elif [ "$config" == "ring_nlb" ]; then
				topology="ring"
				load_balanced="false"
			elif [ "$config" == "linear" ]; then
				topology="linear"
				load_balanced="false"
			fi
			echo "$np,$run,$topology,$load_balanced,$time" >>$output_file
		done
	done
done

echo "Benchmarking completed. Results saved to $output_file."

python ./benchmarks/graphing.py
