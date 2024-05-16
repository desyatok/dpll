import subprocess
import os
import time


def get_test_file_header_line(test_file):
    while True:
        line = test_file.readline()
        if line[0] == 'p':
            break
    return line


def create_verifying_cnf(test_file_path, model):
    test_file = open(test_file_path, 'r')
    verifying_cnf = open('verifying.cnf', 'w')
    header_line_list = get_test_file_header_line(test_file).split(' ')

    # Add vars_num new unit clauses to cnf
    clauses_num = int(header_line_list[3])
    vars_num = int(header_line_list[2])
    clauses_num += vars_num

    header_line_list[3] = str(clauses_num)
    new_header = ' '.join(header_line_list) + '\n'
    verifying_cnf.write(new_header)

    # Fill file with new clauses and add old ones
    for literal in model:
        verifying_cnf.write(literal + ' ' + '0\n')
    verifying_cnf.writelines(test_file.readlines())

    test_file.close()
    verifying_cnf.close()

def verify_single_cnf(file):
    print(f"Trying {file.split('/')[-1]}")
    start = time.time()
    solver_run = subprocess.run([solver_location, file], stdout=subprocess.PIPE)
    end = time.time()

    my_solution = str(solver_run.stdout).split('\\n')[0]
    my_sat = 'UNSATISFIABLE' not in my_solution

    my_model = str(solver_run.stdout).split('\\n')[1].split(' ')[1:-1]
    create_verifying_cnf(file, my_model)

    if my_sat:
        picosat_run = subprocess.run(['picosat', 'verifying.cnf'], stdout=subprocess.PIPE)
        os.remove('verifying.cnf')
    else:
        picosat_run = subprocess.run(['picosat', file], stdout=subprocess.PIPE)

    picosat_solution = str(picosat_run.stdout).split('\\n')[0]
    picosat_sat = 'UNSATISFIABLE' not in picosat_solution

    if my_sat == picosat_sat:
        print(f'Verification is successful, problem is {"sat" if my_sat else "unsat"}')
        print(f'elapsed time: {end - start}\n')
        return True
    else:
        print(f'Error: verification is unsuccessful\n')
        return False


test_data = 'test_data'
solver_location = os.getcwd() + '/build/dpll'

for test in os.scandir(test_data):
    success = verify_single_cnf(test.path)
    if success:
        continue
    os.exit(-1)



