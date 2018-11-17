import React, { Component } from 'react';
import PropTypes from 'prop-types';
import { LineChart, BarChart, Bar, XAxis, Tooltip, CartesianGrid, Line, YAxis,
  ResponsiveContainer, Legend } from 'recharts';
import { connect } from 'react-redux';
import Utils from '../utils/Utils';
import { getMetricsByKey } from '../reducers/MetricReducer';
import './MetricView.css';
import { Experiment } from "../sdk/MlflowMessages";
import { getExperiment, getRunTags} from "../reducers/Reducers";
import BreadcrumbTitle from "./BreadcrumbTitle";

const COLORS = [
  "#993955",
  "#AE76A6",
  "#A3C3D9",
  "#364958",
  "#FF82A9",
  "#FFC0BE",
];

class MetricView extends Component {
  static propTypes = {
    experiment: PropTypes.instanceOf(Experiment).isRequired,
    title: PropTypes.element.isRequired,
    // Object with keys from Metric json and also
    metrics: PropTypes.arrayOf(Object).isRequired,
    runUuids: PropTypes.arrayOf(String).isRequired,
    runNames: PropTypes.arrayOf(String).isRequired,
  };

  render() {
    const { experiment, runUuids, title, metrics, runNames } = this.props;
    if (metrics.length === 1) {
      return (
        <div className="MetricView">
          <div className="header-container">
            <BreadcrumbTitle
              experiment={experiment}
              runNames={runNames}
              runUuids={runUuids}
              title={title}
            />
          </div>
          <ResponsiveContainer width="100%" aspect={1.55}>
            <BarChart
              data={metrics}
              margin={{top: 10, right: 10, left: 10, bottom: 10}}
            >
              <XAxis dataKey="index"/>
              <Tooltip isAnimationActive={false} labelStyle={{display: "none"}}/>
              <CartesianGrid strokeDasharray="3 3"/>
              <Legend verticalAlign="bottom"/>
              <YAxis/>
              {runUuids.map((uuid, idx) => (
                <Bar dataKey={uuid}
                     key={uuid}
                     isAnimationActive={false}
                     fill={COLORS[idx % COLORS.length]}/>
              ))}
            </BarChart>
          </ResponsiveContainer>
        </div>
      );
    } else {
      return (
        <div className="MetricView">
          <div className="header-container">
            <BreadcrumbTitle
              experiment={experiment}
              runNames={runNames}
              runUuids={runUuids}
              title={title}
            />
          </div>
          <ResponsiveContainer width="100%" aspect={1.55}>
            <LineChart
              data={Utils.convertTimestampToInt(metrics)}
              margin={{top: 10, right: 10, left: 10, bottom: 10}}
            >
              <XAxis dataKey="index" type="number"/>
              <Tooltip isAnimationActive={false} labelStyle={{display: "none"}}/>
              <CartesianGrid strokeDasharray="3 3"/>
              <Legend verticalAlign="bottom"/>
              <YAxis/>
              {runUuids.map((uuid, idx) => (
                <Line type="linear"
                      dataKey={uuid}
                      key={uuid}
                      isAnimationActive={false}
                      connectNulls
                      stroke={COLORS[idx % COLORS.length]}/>
              ))}
            </LineChart>
          </ResponsiveContainer>
        </div>
      );
    }
  }
}

const mapStateToProps = (state, ownProps) => {
  const { metricKey, runUuids, experimentId } = ownProps;
  const experiment = experimentId !== null ? getExperiment(experimentId, state) : null;
  let maxLength = 0;
  runUuids.forEach(runUuid => {
    maxLength = Math.max(maxLength, getMetricsByKey(runUuid, metricKey, state).length);
  });
  const metrics = new Array(maxLength);
  for (let i = 0; i < metrics.length; i++) {
    metrics[i] = {index: i};
  }
  runUuids.forEach(runUuid => {
    const entries = getMetricsByKey(runUuid, metricKey, state);
    for (let i = 0; i < entries.length; i++) {
      metrics[i][runUuid] = entries[i].value;
    }
  });
  const runNames = runUuids.map((runUuid) => {
    const tags = getRunTags(runUuid, state);
    return Utils.getRunDisplayName(tags, runUuid);
  });
  return {
    experiment,
    metrics,
    title: <span>{metricKey}</span>,
    runUuids: runUuids,
    runNames: runNames
  };
};

export default connect(mapStateToProps)(MetricView);
