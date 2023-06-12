///////////////////////////////////////////////////////////////////////////////
// disks.hpp
// 
// Definitions for two algorithms that each solve the alternating disks
// problem.
//
// As provided, this header has four functions marked with TODO comments.
// You need to write in your own implementation of these functions.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <sstream>
#include <string>
#include <vector>

// TODO
#include <functional>
#include <iostream>

enum disk_color { DISK_DARK, DISK_LIGHT };

class disk_state {
private:
  std::vector<disk_color> _colors;

public:
  disk_state(size_t light_count)
    : _colors(light_count * 2, DISK_DARK) {

    assert(light_count > 0);

    for (size_t i = 0; i < _colors.size(); i += 2) {
      _colors[i] = DISK_LIGHT;
    }
  }

  bool operator==(const disk_state& rhs) const {
    return std::equal(_colors.begin(), _colors.end(), rhs._colors.begin());
  }

  size_t total_count() const {
    return _colors.size();
  }

  size_t light_count() const {
    return total_count() / 2;
  }

  size_t dark_count() const {
    return light_count();
  }

  bool is_index(size_t i) const {
    return (i < total_count());
  }

  disk_color get(size_t index) const {
    assert(is_index(index));
    return _colors[index];
  }

  void swap(size_t left_index) {
    assert(is_index(left_index));
    auto right_index = left_index + 1;
    assert(is_index(right_index));
    std::swap(_colors[left_index], _colors[right_index]);
  }

  std::string to_string() const {
    std::stringstream Stringstream;
    bool first = true;
    for (auto color : _colors) {
      if (!first) {
        Stringstream << " ";
      }

      if (color == DISK_LIGHT) {
        Stringstream << "D";
      } else {
        Stringstream << "L";
      }
      first = false;
    }
    return Stringstream.str();
  }

    bool is_initialized() const {
      for(size_t i=0;i<_colors.size();i++){
      ///check if the index is even or odd and then check if the color is dark or light since they are alternalty oriented
      if(i%2==0){
        if(_colors[i]==DISK_DARK){
          return false;
        }
      }else{
        if(_colors[i]==DISK_LIGHT){
          return false;
      }
    }
    }
    return true;
  }
  

bool is_sorted() const {
    bool disk_light = false;
    for (size_t i = 0; i < total_count(); i++) {
        if (_colors[i] == DISK_LIGHT) {
            disk_light = true;
        } else if (disk_light && _colors[i] == DISK_DARK) {
            return false;
        }
    }
    return true;
  }
};

class sorted_disks {
private:
  disk_state _after;
  unsigned _swap_count;
  
public:
  sorted_disks(const disk_state& after, unsigned swap_count)
    : _after(after), _swap_count(swap_count) { }

  sorted_disks(disk_state&& after, unsigned swap_count)
    : _after(after), _swap_count(swap_count) { }

  const disk_state& after() const {
    return _after;
  }

  unsigned swap_count() const {
    return _swap_count;
  }
};

sorted_disks sort_alternate(const disk_state& before) {
  disk_state after(before);
  unsigned swap_count = 0;
  bool swap_prior = true;

  while(swap_prior) {
    swap_prior = false;

    for (size_t i = 0; i < after.total_count() - 1; i += 2) {
      if (after.get(i) == DISK_LIGHT && after.get(i + 1) == DISK_DARK) {
        after.swap(i);
        swap_count++;
        swap_prior = true;
        break;  //After a swap, exit the loop
      }
    }

    if (!swap_prior) {
      for (size_t i = 1; i < after.total_count() - 1; i += 2) {
        if (after.get(i) == DISK_LIGHT && after.get(i + 1) == DISK_DARK) {
          after.swap(i);
          swap_count++;
          swap_prior = true;
          break;  //After a swap, exit the loop
        }
      }
    }
  }

  return sorted_disks(after, swap_count);
}

sorted_disks sort_lawnmower(const disk_state& before) {
  disk_state after(before);
    size_t swap_count = 0;
    bool swap_prior;
    do {
        swap_prior = false;
        for (size_t i = 0; i < after.total_count() - 1; i++) {
            if (after.get(i) == DISK_LIGHT && after.get(i + 1) == DISK_DARK) {
                after.swap(i);
                swap_count++;
                swap_prior = true;
            }
        }
        for (size_t i = after.total_count() - 2; i > 0; i--) {
            if (after.get(i) == DISK_LIGHT && after.get(i + 1) == DISK_DARK) {
                after.swap(i);
                swap_count++;
                swap_prior = true;
            }
        }
    } while (swap_prior);
    return sorted_disks(after, swap_count);
}