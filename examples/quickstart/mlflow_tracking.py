import os
from random import random, randint

import sys 
sys.path.append('/home/sepideh/ecopia-zebra/mlflow-0.7/') 

from mlflow.entities import SourceType
from mlflow import log_metric, log_param, log_artifacts
from mlflow.tracking import MlflowClient

if __name__ == "__main__":
    print("Running mlflow_tracking.py")
    client = MlflowClient()
    exp_id = client.create_experiment("run_sepi2")

    run = client.create_run(exp_id, "sepideh", "run_sepi", SourceType.JOB, "example/quickstart", "mlflow_tracking.py")

    run_id = run.info.run_uuid
    client.log_param (run_id, "param1", randint(0, 100))
    client.log_metric(run_id, "foo", random())
    client.log_metric(run_id, "foo", random() + 1)
    client.log_metric(run_id, "foo", random() + 2)

    client.log_author(run_id, "Sepideh")
    client.log_message(run_id,"here is meesage!")
    client.log_docker_id(run_id)
    client.log_docker_image_name(run_id)

    if not os.path.exists("outputs"):
        os.makedirs("outputs")
    with open("outputs/README.txt", "w") as f:
        f.write("Author:" + "Sepideh" + "\n")
    f.close()

    client.log_artifacts(run_id, "outputs")
