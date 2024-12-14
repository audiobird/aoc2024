#include <array>
#include <cstdio>

constexpr auto data = []() {
  constexpr char t[] = {
#embed "input"
  };
  std::array<char, sizeof(t)> out{};
  for (auto i = 0u; i < sizeof(t); ++i) {
    out[i] = t[i];
  }
  return out;
}();

constexpr auto room_w = 101;
constexpr auto room_h = 103;

constexpr auto run_time_seconds = 100;

struct Coords {
  int x{};
  int y{};
};

using Velocity = Coords;

struct Robot {
  Coords coords{};
  Velocity vel{};
};

constexpr Coords calc_position(const Robot r, const int num_iterations) {
  const auto new_x = (r.coords.x + r.vel.x * num_iterations) % room_w;
  const auto actual_x = new_x < 0 ? new_x + room_w : new_x;
  const auto new_y = (r.coords.y + r.vel.y * num_iterations) % room_h;
  const auto actual_y = new_y < 0 ? new_y + room_h : new_y;
  return Coords{actual_x, actual_y};
}

constexpr int get_quadrant(const Coords c) {
  const auto mid_x = room_w / 2;
  const auto mid_y = room_h / 2;
  if (c.x == mid_x || c.y == mid_y) {
    return -1;
  }
  const auto on_right = c.x > mid_x;
  const auto on_bottom = c.y > mid_y;

  return on_right | (on_bottom << 1);
}

constexpr int ctoi(const char a) {
  const auto t = a - '0';
  if (t < 0 || t >= 10) {
    return -1;
  }
  return t;
}

constexpr int concat(int l, int r) { return l * 10 + r; }

constexpr int stoi(const char *s) {
  bool is_neg = false;
  if (*s == '-') {
    is_neg = true;
    s++;
  }
  auto out = 0;
  while (ctoi(*s) != -1) {
    out = concat(out, ctoi(*s));
    s++;
  }
  return out * (is_neg ? -1 : 1);
}

constexpr int find_next(int start_offset, const char c) {
  while (start_offset < data.size()) {
    if (data[start_offset] == c) {
      return start_offset;
    }
    start_offset++;
  }
  return data.size();
}

constexpr Robot get_robot(int offset) {
  Robot out{};
  offset = find_next(offset, '=');
  out.coords.x = stoi(&data[offset + 1]);
  offset = find_next(offset, ',');
  out.coords.y = stoi(&data[offset + 1]);
  offset = find_next(offset, '=');
  out.vel.x = stoi(&data[offset + 1]);
  offset = find_next(offset, ',');
  out.vel.y = stoi(&data[offset + 1]);
  return out;
}

constexpr auto answer = []() {
  std::array<int, 4> num_in_quad{};
  for (auto idx = 0; idx < data.size(); idx = find_next(idx + 1, 'p')) {
    auto r = get_robot(idx);
    auto o = calc_position(r, run_time_seconds);
    auto q = get_quadrant(o);
    if (q != -1) {
      num_in_quad[q]++;
    }
  }
  auto out{1};
  for (auto i : num_in_quad) {
    out *= i;
  }
  return out;
}();

int main() { printf("Safety Factor: %d\n", answer); }
