FROM python:2.7

LABEL maintainer "Sepideh"

RUN pip install mlflow

#RUN curl -o- https://raw.githubusercontent.com/creationix/nvm/v0.33.11/install.sh | bash
#RUN nvm install node
#RUN nvm use node
#RUN nvm install --lts
#RUN nvm use --lts

CMD mlflow server \
    --default-artifact-root s3://mlflow-runs/ \
    --host 0.0.0.0 
#    --file-store /mllfow/mlruns
