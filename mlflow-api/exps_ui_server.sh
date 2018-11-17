# mlflow server \
#     --file-store /mnt/persistent-disk \
#     --default-artifact-root s3://my-mlflow-bucket/ \
#     --host 0.0.0.0

mlflow server \
    --file-store $1 \
    --default-artifact-root $2 \
    --host $3 

