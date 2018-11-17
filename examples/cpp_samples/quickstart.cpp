#include <org/mlflow/api/proto/service.pb.h> 
#include <org/mlflow/tracking/MlflowClient.h> 
#include <glog/logging.h>
#include <mlflow>

using namespace mlflow;

void createRun(MlflowClient* client, long expId) {
  LOG(ERROR) << "====== createRun ======";

  // Create run
  std::string sourceFile = "MyFile.cpp";

  RunInfo runCreated = client->createRun(expId, sourceFile);
  LOG(ERROR) << ("CreateRun: " + runCreated);
  std::string runId = runCreated.get_run_uuid();

  // Log parameters
  client->logParam(runId, "min_samples_leaf", "2");
  client->logParam(runId, "max_depth", "3");

  // Log metrics
  client->logMetric(runId, "auc", 2.12F);
  client->logMetric(runId, "accuracy_score", 3.12F);
  client->logMetric(runId, "zero_one_loss", 4.12F);

  // Update finished run
  client->setTerminated(runId, RunStatus::FINISHED);

  // Get run details
  Run run = client->getRun(runId);
  LOG(ERROR) << ("GetRun: " + run);
}

int main() {
  MlflowClient client; 
  std::string expName = "run_cpp";
  long expId = client.createExperiment(expName);
  LOG(ERROR) << "exp id = " << expId;

  createRun(&client, expId);

  return 0;
}


