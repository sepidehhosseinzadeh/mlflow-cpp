import { MlflowService } from './sdk/MlflowService';
import Utils from './utils/Utils';

export const isPendingApi = (action) => {
  return action.type.endsWith("_PENDING");
};

export const pending = (apiActionType) => {
  return `${apiActionType}_PENDING`;
};

export const isFulfilledApi = (action) => {
  return action.type.endsWith("_FULFILLED");
};

export const fulfilled = (apiActionType) => {
  return `${apiActionType}_FULFILLED`;
};

export const isRejectedApi = (action) => {
  return action.type.endsWith("_REJECTED");
};

export const rejected = (apiActionType) => {
  return `${apiActionType}_REJECTED`;
};

export const LIST_EXPERIMENTS_API = 'LIST_EXPERIMENTS_API';
export const listExperimentsApi = (id = getUUID()) => {
  return {
    type: LIST_EXPERIMENTS_API,
    payload: wrapDeferred(MlflowService.listExperiments, {}),
    meta: { id: id },
  };
};

export const GET_EXPERIMENT_API = 'GET_EXPERIMENT_API';
export const getExperimentApi = (experimentId, id = getUUID()) => {
  return {
    type: GET_EXPERIMENT_API,
    payload: wrapDeferred(MlflowService.getExperiment, { experiment_id: experimentId }),
    meta: { id: id },
  };
};

export const GET_RUN_API = 'GET_RUN_API';
export const getRunApi = (runUuid, id = getUUID()) => {
  return {
    type: GET_RUN_API,
    payload: wrapDeferred(MlflowService.getRun, { run_uuid: runUuid }),
    meta: { id: id },
  };
};

export const DELETE_RUN_API = 'DELETE_RUN_API';
export const deleteRunApi = (runUuid, id = getUUID()) => {
  return (dispatch) => {
    const deleteResponse = dispatch({
      type: DELETE_RUN_API,
      payload: wrapDeferred(MlflowService.deleteRun, { run_id: runUuid }),
      meta: { id: getUUID() },
    });
    return deleteResponse.then(() => dispatch(getRunApi(runUuid, id)));
  };
};

export const RESTORE_RUN_API = 'RESTORE_RUN_API';
export const restoreRunApi = (runUuid, id = getUUID()) => {
  return (dispatch) => {
    const restoreResponse = dispatch({
      type: RESTORE_RUN_API,
      payload: wrapDeferred(MlflowService.restoreRun, { run_id: runUuid }),
      meta: { id: getUUID() },
    });
    return restoreResponse.then(() => dispatch(getRunApi(runUuid, id)));
  };
};

export const SEARCH_RUNS_API = 'SEARCH_RUNS_API';
export const searchRunsApi = (experimentIds, andedExpressions, runViewType, id = getUUID()) => {
  return {
    type: SEARCH_RUNS_API,
    payload: wrapDeferred(MlflowService.searchRuns, {
      experiment_ids: experimentIds, anded_expressions: andedExpressions, run_view_type: runViewType
    }),
    meta: { id: id },
  };
};

export const LIST_ARTIFACTS_API = 'LIST_ARTIFACTS_API';
export const listArtifactsApi = (runUuid, path, id = getUUID()) => {
  return {
    type: LIST_ARTIFACTS_API,
    payload: wrapDeferred(MlflowService.listArtifacts, {
      run_uuid: runUuid, path: path
    }),
    meta: { id: id, runUuid: runUuid, path: path },
  };
};

export const GET_METRIC_HISTORY_API = 'GET_METRIC_HISTORY_API';
export const getMetricHistoryApi = (runUuid, metricKey, id = getUUID()) => {
  return {
    type: GET_METRIC_HISTORY_API,
    payload: wrapDeferred(MlflowService.getMetricHistory, {
      run_uuid: runUuid, metric_key: metricKey
    }),
    meta: { id: id, runUuid: runUuid, key: metricKey },
  };
};

export const SET_TAG_API = 'SET_TAG_API';
export const setTagApi = (runUuid, tagName, tagValue, id = getUUID()) => {
  return {
    type: SET_TAG_API,
    payload: wrapDeferred(MlflowService.setTag, {
      run_uuid: runUuid, key: tagName, value: tagValue
    }),
    meta: { id: id, runUuid: runUuid, key: tagName, value: tagValue },
  };
};

export const CLOSE_ERROR_MODAL = 'CLOSE_ERROR_MODAL';
export const closeErrorModal = () => {
  return {
    type: CLOSE_ERROR_MODAL,
  };
};

export const OPEN_ERROR_MODAL = 'OPEN_ERROR_MODAL';
export const openErrorModal = (text) => {
  return {
    type: OPEN_ERROR_MODAL,
    text,
  };
};

export const getUUID = () => {
  const randomPart = Math.random()
    .toString(36)
    .substring(2, 10);
  return new Date().getTime() + randomPart;
};

/**
 * Jquery's ajax promise is a bit weird so I chose to create a new Promise which resolves and
 * rejects using the ajax callbacks `success` and `error`.
 */
const wrapDeferred = (deferred, data) => {
  return new Promise((resolve, reject) => {
    deferred({
      data,
      success: response => resolve(response),
      error: xhr => {
        console.error("XHR failed", xhr);
        reject(new Error(Utils.getErrorMessageFromXhr(xhr)));
      }
    });
  });
};

