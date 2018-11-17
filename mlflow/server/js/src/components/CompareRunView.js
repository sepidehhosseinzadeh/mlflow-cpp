import React, { Component } from 'react';
import PropTypes from 'prop-types';
import { getExperiment, getParams, getRunInfo } from '../reducers/Reducers';
import { connect } from 'react-redux';
import './CompareRunView.css';
import { Experiment, RunInfo } from '../sdk/MlflowMessages';
import CompareRunScatter from './CompareRunScatter';
import Routes from '../Routes';
import { Link } from 'react-router-dom';
import { getLatestMetrics } from '../reducers/MetricReducer';
import BreadcrumbTitle from "./BreadcrumbTitle";
import CompareRunUtil from './CompareRunUtil';
import Utils from '../utils/Utils';

class CompareRunView extends Component {
  static propTypes = {
    experiment: PropTypes.instanceOf(Experiment).isRequired,
    runInfos: PropTypes.arrayOf(RunInfo).isRequired,
    runUuids: PropTypes.arrayOf(String).isRequired,
    metricLists: PropTypes.arrayOf(Array).isRequired,
    paramLists: PropTypes.arrayOf(Array).isRequired,
  };

  render() {
    const experiment = this.props.experiment;
    const experimentId = experiment.getExperimentId();

    return (
      <div className="CompareRunView">
        <div className="header-container">
          <BreadcrumbTitle
            experiment={experiment}
            title={"Comparing " + this.props.runInfos.length + " Runs"}
          />
        </div>
        <div className="responsive-table-container">
          <table className="compare-table table">
            <thead>
              <tr>
                <th scope="row" className="row-header">Run ID:</th>
                {this.props.runInfos.map(r =>
                  <th scope="column" className="data-value" key={r.run_uuid}>
                    <Link to={Routes.getRunPageRoute(r.getExperimentId(), r.getRunUuid())}>
                      {r.getRunUuid()}
                    </Link>
                  </th>
                )}
              </tr>
            </thead>
            <tbody>
              <tr>
                <th scope="row" className="data-value">Start Time:</th>
                {this.props.runInfos.map((run) => {
                  const startTime =
                    run.getStartTime() ? Utils.formatTimestamp(run.getStartTime()) : '(unknown)';
                  return <td className="meta-info" key={run.run_uuid}>{startTime}</td>;
                }
                )}
              </tr>
              <tr>
                <th scope="rowgroup"
                    className="inter-title"
                    colSpan={this.props.runInfos.length + 1}>
                  <h2>Parameters</h2>
                </th>
              </tr>
              {this.renderDataRows(this.props.paramLists)}
              <tr>
                <th scope="rowgroup"
                    className="inter-title"
                    colSpan={this.props.runInfos.length + 1}>
                  <h2>Metrics</h2>
                </th>
              </tr>
              {this.renderDataRows(this.props.metricLists, (key, data) => {
                return <Link
                  to={Routes.getMetricPageRoute(
                      this.props.runInfos.map(info => info.run_uuid)
                                         .filter((uuid, idx) => data[idx] !== undefined),
                      key,
                      experimentId)}
                  title="Plot chart">
                  {key}
                  <i className="fas fa-chart-line" style={{paddingLeft: "6px"}}/>
                </Link>;
              }, Utils.formatMetric)}
            </tbody>
          </table>
        </div>

        <CompareRunScatter runUuids={this.props.runUuids}/>
      </div>
    );
  }

  // eslint-disable-next-line no-unused-vars
  renderDataRows(list, headerMap = (key, data) => key, formatter = (value) => value) {
    const keys = CompareRunUtil.getKeys(list);
    const data = {};
    keys.forEach(k => data[k] = []);
    list.forEach((records, i) => {
      keys.forEach(k => data[k].push(undefined));
      records.forEach(r => data[r.key][i] = r.value);
    });

    return keys.map(k => {
      return <tr key={k}>
        <th scope="row" className="rowHeader">{headerMap(k, data[k])}</th>
        {data[k].map((value, i) =>
          <td className="data-value" key={this.props.runInfos[i].run_uuid}>
            {value === undefined ? "" : formatter(value)}
          </td>
        )}
      </tr>;
    });
  }
}

const mapStateToProps = (state, ownProps) => {
  const runInfos = [];
  const metricLists = [];
  const paramLists = [];
  const { experimentId, runUuids } = ownProps;
  const experiment = getExperiment(experimentId, state);
  runUuids.forEach((runUuid) => {
    runInfos.push(getRunInfo(runUuid, state));
    metricLists.push(Object.values(getLatestMetrics(runUuid, state)));
    paramLists.push(Object.values(getParams(runUuid, state)));
  });
  return { experiment, runInfos, metricLists, paramLists };
};

export default connect(mapStateToProps)(CompareRunView);
