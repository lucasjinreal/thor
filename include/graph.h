/**
 *
 *
 *
 * Code from apollo, simply do graph optimization
 * for automous car
 * 
 * 
 * 
 * */

#ifndef _APOLLO_PERCEPTION_STANDALONE_UTIL_GRAPH_UTIL_H_
#define _APOLLO_PERCEPTION_STANDALONE_UTIL_GRAPH_UTIL_H_

#include <vector>

namespace thor {
namespace graph {

// bfs based component analysis
void ConnectedComponentAnalysis(const std::vector<std::vector<int>>& graph,
                                std::vector<std::vector<int>>* components);

}  // namespace graph
}  // namespace thor
#endif  // MODULES_PERCEPTION_OBSTACLE_COMMON_GRAPH_UTIL_H_
