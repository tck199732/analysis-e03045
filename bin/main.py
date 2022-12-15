import pathlib
import glob
import subprocess
import inspect
import time

input_dir = pathlib.Path('/data/chajecki/EXP_DATA')
output_dir = pathlib.Path('/data/kin/e03045')
folders = ['40Ca', '40Ca_One', '48Ca', '48Ca_40t_One', '48Ca_One']


def main():
    files = dict()
    corrupted_files = dict()
    for folder in folders:
        sub_dir = pathlib.Path(input_dir, folder)

        files[folder] = glob.glob(f'{str(sub_dir)}/*.root')
        files[folder] = [pathlib.Path(p) for p in sorted(files[folder])]

    start_time = time.time()
    # run reader for all files
    for i, (folder, files) in enumerate(files.items()):
        corrupted_files[folder] = []
        for file in files:
            out = pathlib.Path(output_dir, folder)
            out.mkdir(parents=True, exist_ok=True)

            input = str(file)
            output = f'{str(out)}/{file.name}'
            script = inspect.cleandoc(f'''
                root -b -q load.C "read.cpp(\\"{input}\\", \\"{output}\\")"
            ''')

            print(script)
            result = subprocess.run(
                script, shell=True, text=True, capture_output=True)
            stdout = result.stdout.strip()
            stderr = result.stderr.strip()
            if stderr != '':
                print(stderr)
                corrupted_files[folder].append(input)
    end_time = time.time()
    print(f'ALL DONE : Time Taken = {end_time - start_time}s')

    for i, (folder, files) in enumerate(corrupted_files.items()):
        for file in files:
            print(file)


if __name__ == '__main__':
    main()
