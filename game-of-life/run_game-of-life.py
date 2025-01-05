import os
import argparse
import logging
import subprocess
import difflib
from collections.abc import Iterable
from typing import Union, LiteralString

FILENAME_CPU_TIMINGS = '_cpu_time.csv'
FILENAME_OPENMP_TIMINGS = '_openmp_time.csv'

def write_diff(file, diff):
    with open(file, 'w') as f:
        f.writelines(diff)

def compare_files(file1, file2):
    with open(file1, 'r') as f1, open(file2, 'r') as f2:
        diff = difflib.unified_diff(f1.readlines(), f2.readlines())
        if diff:
            return diff
        return None

def dir_path(path):
    """
    Validate the given path as a directory path. If the path doesn't exist, attempt to create it.

    Args:
        path (str): The path to validate.

    Returns:
        str: The valid directory path.

    Raises:
        argparse.ArgumentTypeError: If the path is not valid or cannot be created.
    """
    # Check if the path is valid for the current OS
    if not isinstance(path, str) or len(path.strip()) == 0:
        raise argparse.ArgumentTypeError(f"Invalid path: {path}")

    # If the path exists, ensure it is a directory
    if os.path.exists(path):
        if os.path.isdir(path):
            return path
        else:
            raise argparse.ArgumentTypeError(f"{path} exists but is not a directory.")

    # If the path does not exist, check if it can be created
    try:
        os.makedirs(path, exist_ok=True)
        return path
    except OSError as e:
        raise argparse.ArgumentTypeError(f"Could not create directory: {path}. Error: {e}")

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Run game of life application with all input files')
    parser.add_argument('executable', type=str, help='Executable file')
    parser.add_argument('basename', type=str, help='Basename of the generated result files', default='')
    parser.add_argument('-i', '--input_dir', type=dir_path, help='Input file')
    parser.add_argument('-o', '--output_dir', type=dir_path, help='Output file')
    parser.add_argument('-r', '--reference_dir', type=dir_path, help='Reference directory to compare output')
    parser.add_argument('-t', '--threads', type=int, help='Number of threads to use when running the application in parallel', default=4)
    parser.add_argument('-v', '--verbose', action='store_true', help='Print verbose output (DEBUG)')
    args = parser.parse_args()

    if args.verbose:
        logging.basicConfig(level=logging.DEBUG)
    else:
        logging.basicConfig(level=logging.INFO)

    log = logging.getLogger('Script')

    # Verify executable
    if not os.path.exists(args.executable):
        log.error('Executable file not found')
        exit(1)

    # Gather all input files
    input_files = os.listdir(args.input_dir)
    if input_files is None:
        log.error('No input files found in the input directory')
        exit(1)

    reference_files = os.listdir(args.reference_dir)
    if reference_files is None:
        log.error('No reference files found in the reference directory')
        exit(1)

    if len(input_files) > len(reference_files):
        log.warning('More input files than reference files.')

    if os.path.exists(args.output_dir):
        log.warning('Output directory already exists. Clearing files...')
        for file in os.listdir(args.output_dir):
            os.remove(os.path.join(args.output_dir, file))
    else:
        os.makedirs(args.output_dir)

    # Construct pairs for input and reference files based on the file name
    input_reference_pairs = []
    # Input files end with _in.gol and reference files end with _out.gol
    for input_file in input_files:
        ref_file = input_file.replace('_in', '_out')
        if ref_file in reference_files:
            input_reference_pairs.append((input_file, ref_file))
        else:
            log.warning('No reference file found for input file: {}'.format(input_file))

    # Run the application with all input files and verify their outputs
    for i, (input_file, reference_file) in enumerate(input_reference_pairs):
        log.info('Running {}/{}...'.format(i+1, len(input_reference_pairs)))
        input_file_path = os.path.join(args.input_dir, input_file)
        output_file_path = os.path.join(args.output_dir, reference_file) # Use matching names for output and reference
        reference_file_path = os.path.join(args.reference_dir, reference_file)

        # NOTE CLI Options for game_of_life.exe:
        # -l|--load <input_file>
        # -s|--save <output_file>
        # -g|--generations <number_of_generations>
        # -m|--measure
        # -p|--pretty
        # --mode seq|par|omp
        # --threads <number_of_threads>
        args_s = [args.executable, '-l', input_file_path, '-s', output_file_path, '--measure', '--mode', 'seq']
        args_p = [args.executable, '-l', input_file_path, '-s', output_file_path, '--measure', '--mode', 'par', '--threads', str(args.threads)]

        log.debug('Running application with input file: {}'.format(input_file))
        log.debug('(sequential) {}'.format(args_s))
        result_s = subprocess.run(args_s, capture_output=True, shell=True)
        log.debug('(sequential) Verifying output file: {}'.format(output_file_path))
        difference = compare_files(output_file_path, reference_file_path)
        if difference:
            log.error('(parallel) Output file does not match reference file: {}'.format(reference_file))
            difference_file = os.path.join(args.output_dir, reference_file.replace('_out.gol', '_seq_diff.txt'))
            write_diff(difference_file, difference)
            log.error('Diff wrote to file:\n{}'.format(difference_file))

        log.debug('(parallel) {}'.format(args_p))
        result_p = subprocess.run(args_p, capture_output=True, shell=True)
        log.debug('(parallel) Verifying output file: {}'.format(output_file_path))
        difference = compare_files(output_file_path, reference_file_path)
        if difference:
            log.error('(parallel) Output file does not match reference file: {}'.format(reference_file))
            difference_file = os.path.join(args.output_dir, reference_file.replace('_out.gol', '_omp_diff.txt'))
            write_diff(difference_file, difference)
            log.error('Diff wrote to file:{}'.format(difference_file))

        log.debug('Storing timings to CSV')
        with open(os.path.join(args.output_dir, args.basename + FILENAME_CPU_TIMINGS), 'a') as f:
            f.write(result_s.stdout.decode())
        with open(os.path.join(args.output_dir, args.basename + FILENAME_OPENMP_TIMINGS), 'a') as f:
            f.write(result_p.stdout.decode())