% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/tracking-fluent.R
\name{mlflow_end_run}
\alias{mlflow_end_run}
\title{End a Run}
\usage{
mlflow_end_run(status = c("FINISHED", "SCHEDULED", "FAILED", "KILLED"))
}
\arguments{
\item{status}{Updated status of the run. Defaults to `FINISHED`.}
}
\description{
End an active MLflow run (if there is one).
}
\details{
The fluent API family of functions operate with an implied MLflow client
  determined by the service set by `mlflow_set_tracking_uri()`. For operations
  involving a run it adopts the current active run, or, if one does not exist,
  starts one through the implied service.
}
\seealso{
Other Fluent API functions: \code{\link{mlflow_create_experiment}},
  \code{\link{mlflow_log_artifact}},
  \code{\link{mlflow_log_metric}},
  \code{\link{mlflow_log_param}},
  \code{\link{mlflow_set_experiment}},
  \code{\link{mlflow_set_tag}},
  \code{\link{mlflow_start_run}}
}
\concept{Fluent API functions}
