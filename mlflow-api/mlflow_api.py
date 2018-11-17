import subprocess
import argparse
import os
import sys
sys.path.append('/home/sepideh/mlflow-0.7/')
import mlflow

def run_command_safely(cmd):
    return_code = -1
    print '################', cmd
    while return_code != 0:
        p = subprocess.Popen(cmd, shell=True)
        p.communicate()
        return_code = p.returncode

def create_exp(name):
    args = ['mlflow experiments create', name]
    run_command_safely(' '.join(args))

#def create_exp(name):
#    return mlflow.create_experiment(name)

def list_exp():
    args = ['mlflow experiments list']
    run_command_safely(' '.join(args))

def delete_exp(exp_id):
    args = ['mlflow experiments delete', exp_id]
    run_command_safely(' '.join(args))

def restore_exp(exp_id):
    args = ['mlflow experiments restore', exp_id]
    run_command_safely(' '.join(args))

def run_package_exp(no_conda, uri, exp_id, param_vals): 
    pairs_str = ''
    for pair in param_vals:
      key, val = pair.split('=')
      pairs_str = pairs_str + key + '=' + val + ' '

    print param_vals
    args = ['mlflow run', '' if no_conda == 0 else '--no-conda', '--experiment-id', exp_id, uri,
            '' if len(param_vals) == 0 else '-P', pairs_str]
    print args
    run_command_safely(' '.join(args))

def exp_ui_local(port):
    args = ['mlflow ui', '-p', port]
    run_command_safely(' '.join(args))

def exp_ui_launch():
    args = ['cd /mlflow/mlflow/server/js', '&&', 'npm start']
    run_command_safely(' '.join(args))

def exp_ui_server(file_store_address, default_root_address, host):
    args = ['mlflow server', '--file-store', file_store_address, 
            '--default-artifact-root', default_root_address, 
            '--host', host]
    run_command_safely(' '.join(args))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--task', default="",
            help="create_exp, run, ...")

    parser.add_argument('--exp_name', default="",
            help="experiment name.")
    parser.add_argument('--artifact-location', default="",
            help="artifact location, to store outputs.")

    parser.add_argument('--exp_id', default=0,
            help="experiment id for deletion.")

    parser.add_argument('--no_conda', default=0,
            help="using conda env or not")

    parser.add_argument('--uri', default=".",
            help="address of the local dir/github address containing MLproject.")

    parser.add_argument('--param_val', nargs='*', default="",
            help="param=val input.")

    parser.add_argument('--port', default="1234")

    parser.add_argument('--file_store_address', default="./")

    parser.add_argument('--default_root_address', default="./") # s3://my-mlflow-bucket/

    parser.add_argument('--host', default="0.0.0.0")

    args = parser.parse_args()

    task = args.task

    if task == 'create_exp':
      if len(args.exp_name) == 0:
        print "no name is provided for experiment."
        sys.exit()
      create_exp(args.exp_name)

    if task == 'list_exp':
      list_exp()

    if task == 'delete_exp':
      delete_exp(args.exp_id)

    if task == 'restore_exp':
      delete_exp(args.exp_id)

    if task == 'run_package_exp':
      run_package_exp(args.no_conda, args.uri, args.exp_id, args.param_val)

    if task == 'exp_ui_local':
      exp_ui_local(args.port)

    if task == 'exp_ui_launch':
      exp_ui_launch()

    if task == 'exp_ui_server':
      exp_ui_server(args.file_store_address, args.default_root_address, args.host)

if __name__ == '__main__':
    main()

