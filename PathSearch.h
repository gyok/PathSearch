#pragma once

#include <list>
#include <memory>
#include <string>
#include <vector>

class PathSearch {
  typedef std::vector<std::string> Path;
  struct Town;
  struct Connection {
    size_t m_destination_town_id;
    int m_distance;
    Connection(size_t i_destination_town_id = 0, int i_distance = 0)
        : m_destination_town_id{i_destination_town_id}, m_distance{i_distance} {
    }
  };

  struct Town {
    bool m_learned;
    std::string m_town_name;
    std::vector<Connection> mp_connected_towns;
    Town(std::string i_town_name)
        : m_town_name{i_town_name},
          mp_connected_towns(std::vector<Connection>()) {}
  };

public:
  PathSearch(const std::string &i_filename, std::string i_from,
             std::string i_to);

private:
  Path WideSearch(size_t i_from_index, size_t i_end_index);
  Path DepthSearch(size_t i_from_index, size_t i_end_index);
  void PrintPath(const Path &i_path);
  void ParseFile(const std::string &i_filename);

private:
  std::vector<Town> m_towns;
  Town *mp_root;
};
