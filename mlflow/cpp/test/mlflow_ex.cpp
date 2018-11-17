#include <org/mlflow/tracking/MlflowClient.h> 
#include <aws/core/Aws.h>
#include <string> 
#include <iostream>
#include <fstream>

void createRun(MlflowClient& client, long expId) {
  LOG(ERROR) << "====== createRun ======";

  // Create run
  std::string sourceFile = "sepideh_run";

  RunInfo runCreated = client.createRun(expId, sourceFile);
  std::string runId = runCreated.run_uuid();
  LOG(ERROR) << "### CreateRun with id=" << runId << ";";

  // Log parameters
  client.logParam(runId, "min_samples_leaf", "2");
  client.logParam(runId, "max_depth", "3");

  // Log metrics
  client.logMetric(runId, "auc", 2.12);
  client.logMetric(runId, "accuracy_score", 3.12);
  client.logMetric(runId, "zero_one_loss", 4.12);
  
  client.setTag(runId, "tag", "run");

  std::string localPath = "./README.txt";
  std::ofstream outfile (localPath);
  outfile << "Author : Sepideh ";
  outfile.close();

  client.logArtifact(runId, std::to_string(expId), localPath, "README.txt", "mlflow-runs", "");
  client.logMessage(runId, "Message here!");
  client.logAuthor(runId, "Sepideh");
  client.logDockerId(runId);
  client.logDockerImageName(runId);

  // Update finished run
  client.setTerminated(runId, RunStatus::FINISHED);

  // Get run details
  Run run = client.getRun(runId);
  LOG(ERROR) << "GetRun: " << run.info().run_uuid();
}

int main() {
  Aws::SDKOptions options;
  Aws::InitAPI(options);

  MlflowClient client; 
//  std::string expName = "run_cpp";
//  long expId = client.createExperiment(expName);
//  LOG(ERROR) << "exp id = " << expId;

  createRun(client, 0);

  Aws::ShutdownAPI(options); 
  return 0;
}


