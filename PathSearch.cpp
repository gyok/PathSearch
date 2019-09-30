#include "PathSearch.h"
#include <algorithm>
#include <fstream>

PathSearch::PathSearch(const std::string &i_filename) {
  this->ParseFile(i_filename);
}

void PathSearch::ParseFile(const std::string &i_filename) {
  std::ifstream infile(i_filename);
  std::string beginning_town;
  std::string destination_town;
  size_t beginning_town_index, destination_town_index;
  int distance;
  while (infile >> beginning_town >> destination_town >> distance) {
    auto beginning_town_it = std::find_if(
        m_towns.begin(), m_towns.end(), [&beginning_town](Town *ip_town) {
          return ip_town->m_town_name == beginning_town;
        });

    beginning_town_index = std::distance(m_towns.begin(), beginning_town_it);
    if (beginning_town_it == m_towns.end()) {
      m_towns.emplace_back(new Town(beginning_town));
      beginning_town_index = m_towns.size() - 1;
    }

    auto destination_town_it = std::find_if(
        m_towns.begin(), m_towns.end(), [&destination_town](Town *ip_town) {
          return ip_town->m_town_name == destination_town;
        });
    destination_town_index =
        std::distance(m_towns.begin(), destination_town_it);
    if (destination_town_it == m_towns.end()) {
      m_towns.emplace_back(new Town(destination_town));
      destination_town_index = m_towns.size() - 1;
    }

    auto e = m_towns;
    m_towns[beginning_town_index]->mp_connected_towns->push_back(
        Connection(destination_town_index, distance));
    m_towns[destination_town_index]->mp_connected_towns->push_back(
        Connection(beginning_town_index, distance));
  }
}
