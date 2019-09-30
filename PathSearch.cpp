#include "PathSearch.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <utility>

PathSearch::PathSearch(const std::string &i_filename, std::string i_from,
                       std::string i_to) {
  this->ParseFile(i_filename);
  size_t from_index{}, to_index{};
  auto from_it =
      std::find_if(m_towns.begin(), m_towns.end(), [&i_from](Town i_town) {
        return i_town.m_town_name == i_from;
      });
  auto to_it =
      std::find_if(m_towns.begin(), m_towns.end(),
                   [&i_to](Town i_town) { return i_town.m_town_name == i_to; });
  if (from_it != m_towns.end())
    from_index = static_cast<size_t>(std::distance(m_towns.begin(), from_it));
  if (to_it != m_towns.end())
    to_index = static_cast<size_t>(std::distance(m_towns.begin(), to_it));
  PrintPath(WideSearch(from_index, to_index));
  std::cout << std::endl << std::endl;
  PrintPath(DepthSearch(from_index, to_index));
}

PathSearch::Path PathSearch::WideSearch(size_t i_from_index,
                                        size_t i_end_index) {
  std::vector<std::pair<size_t, std::vector<Connection>>> connections_list,
      next_connections_list;
  Path path;
  static auto copy_towns = m_towns;
  int i = 0;
  connections_list.push_back(std::make_pair(
      i_from_index, copy_towns[i_from_index].mp_connected_towns));
  while (i < 1000) {
    i++;
    for (const auto &connections : connections_list)
      for (const auto &connection : connections.second) {
        if (!copy_towns[connection.m_destination_town_id].m_learned) {
          copy_towns[connection.m_destination_town_id].m_learned = true;
          path.push_back(
              copy_towns[connections.first].m_town_name + " " +
              copy_towns[connection.m_destination_town_id].m_town_name);

          if (connection.m_destination_town_id == i_end_index)
            return path;

          next_connections_list.push_back(std::make_pair(
              connection.m_destination_town_id,
              copy_towns[connection.m_destination_town_id].mp_connected_towns));
        }
      }
    connections_list = next_connections_list;
    next_connections_list.empty();
  }
  return Path();
}

PathSearch::Path PathSearch::DepthSearch(size_t i_from_index,
                                         size_t i_end_index) {
  static auto copy_towns = m_towns;
  for (const auto &connection : copy_towns[i_from_index].mp_connected_towns) {
    if (!copy_towns[connection.m_destination_town_id].m_learned) {
      copy_towns[connection.m_destination_town_id].m_learned = true;
      auto path =
          Path{copy_towns[i_from_index].m_town_name + " " +
               copy_towns[connection.m_destination_town_id].m_town_name};
      if (connection.m_destination_town_id == i_end_index)
        return path;
      auto finded_path =
          DepthSearch(connection.m_destination_town_id, i_end_index);
      if (!finded_path.empty()) {
        path.insert(path.end(), std::make_move_iterator(finded_path.begin()),
                    std::make_move_iterator(finded_path.end()));
        return path;
      }
    }
  }
  return Path();
}

void PathSearch::PrintPath(const Path &i_path) {
  for (const auto &part_of_path : i_path) {
    std::cout << "[" << part_of_path << "], ";
  }
}

void PathSearch::ParseFile(const std::string &i_filename) {
  std::ifstream infile(i_filename);
  std::string beginning_town;
  std::string destination_town;
  size_t beginning_town_index, destination_town_index;
  int distance;
  while (infile >> beginning_town >> destination_town >> distance) {
    auto beginning_town_it = std::find_if(
        m_towns.begin(), m_towns.end(), [&beginning_town](Town i_town) {
          return i_town.m_town_name == beginning_town;
        });

    beginning_town_index =
        static_cast<size_t>(std::distance(m_towns.begin(), beginning_town_it));
    if (beginning_town_it == m_towns.end()) {
      m_towns.emplace_back(beginning_town);
      beginning_town_index = m_towns.size() - 1;
    }

    auto destination_town_it = std::find_if(
        m_towns.begin(), m_towns.end(), [&destination_town](Town i_town) {
          return i_town.m_town_name == destination_town;
        });
    destination_town_index = static_cast<size_t>(
        std::distance(m_towns.begin(), destination_town_it));
    if (destination_town_it == m_towns.end()) {
      m_towns.emplace_back(destination_town);
      destination_town_index = m_towns.size() - 1;
    }

    m_towns[beginning_town_index].mp_connected_towns.push_back(
        Connection(destination_town_index, distance));
    m_towns[destination_town_index].mp_connected_towns.push_back(
        Connection(beginning_town_index, distance));
  }
}
