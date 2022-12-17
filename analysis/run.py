
import subprocess
import pathlib
import collections
import os
import time

data_directory = pathlib.Path('/data/kin/e03045')
project_directory = pathlib.Path('../').resolve()
executable_directory = pathlib.Path(project_directory, 'analysis')
output_directory = pathlib.Path(project_directory, 'result')
output_directory.mkdir(exist_ok=True, parents=True)

programs = dict()
programs['main'] = {
    'ca40': ['Ca40', 'Ca40', '80', str(data_directory), str(pathlib.Path(output_directory, 'ca40.root')), '40Ca 40Ca_One'],
    'ca48':  ['Ca48', 'Ca48', '80', str(data_directory), str(pathlib.Path(output_directory, 'ca48.root')), '48Ca 48Ca_One 48Ca_40t_One'],
}

program_outputs = collections.defaultdict(dict)
program_errors = collections.defaultdict(dict)


def main():
    for i, (program, args_dict) in enumerate(programs.items()):
        path = pathlib.Path(executable_directory, f'{program}.cpp')
        exe = compile(path)
        for j, (name, args) in enumerate(args_dict.items()):
            args = ' '.join(args)
            print(f'{exe} {args}')
            # out = subprocess.run(f'{program} {args}',
            #                      shell=True, text=True, capture_output=True)
            subprocess.run(f'{exe} {args}',
                           shell=True, text=True, capture_output=False)

            # program_outputs[program][name] = out.stdout.strip()

            # program_errors[program][name] = out.stdout.strip()

            # print(program_outputs[program][name])
            # print(program_errors[program][name])


def compile(cpp):
    '''
    exe : the pathlib.Path of the cpp main program
    '''
    cpp = pathlib.Path(cpp)
    exe = pathlib.Path(cpp.parent, f'{cpp.stem}.exe')
    root_inc = subprocess.run('root-config --cflags --libs --glibs',
                              capture_output=True, shell=True, text=True, encoding='utf-8')
    root_inc = root_inc.stdout.strip()
    subprocess.run(
        f'g++ {str(cpp)} -o {exe} -I{root_inc}', shell=True, text=True)
    print(f'compiling {str(exe)}')
    return str(pathlib.Path(exe.parent, f'{exe}'))


if __name__ == '__main__':
    main()
