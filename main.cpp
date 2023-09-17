#include <ncurses.h>
#include <string>
#include <future>

#define COMPLETED_COLOR 1
#define INPUTTING_COLOR 2
#define UNFILLED_COLOR 3
#define PROBLEMS_NUM 11

const std::string problems[PROBLEMS_NUM] = {
  "algorithm",
  "binary heap",
  "cgengo kanzennni rikaisita",
  "data structure",
  "english language",
  "function",
  "go language",
  "hot soup processor",
  "internet explorer",
  "javascript",
  "kokomade",
};

void init_screen() {
  initscr();
  noecho();
  curs_set(0);
  start_color();

  init_pair(COMPLETED_COLOR, COLOR_CYAN, COLOR_BLACK);
  init_pair(INPUTTING_COLOR, COLOR_WHITE, COLOR_BLACK);
  init_pair(UNFILLED_COLOR, COLOR_WHITE, COLOR_BLACK);
}

bool game_flag = true;
int problem_count = 0;
int input_count = 0;
int ch;

void update () {
  if (problem_count >= PROBLEMS_NUM) return;

  auto problem = problems[problem_count];
  if (ch == problem[input_count]) { 
    input_count++;
    ch = -1;
  }
  if (input_count == problem.length()) {
    problem_count++;
    input_count = 0;
  }
}

void print () {
  erase();
  // 四角で囲う
  mvhline(10, 28, ACS_HLINE, 32);
  mvhline(10, 60, ACS_URCORNER, 1);
  mvhline(10, 28, ACS_ULCORNER, 1);
  mvhline(14, 28, ACS_HLINE, 32);
  mvhline(14, 60, ACS_LRCORNER, 1);
  mvhline(14, 28, ACS_LLCORNER, 1);
  
  mvprintw(12, 28, ">");


  if (problem_count >= PROBLEMS_NUM) {
    mvprintw(12, 30, "Congratulations!");
    mvprintw(13, 31, "Press the Escape key to exit.");
    return;
  }

  std::string problem = problems[problem_count];
  int len = problem.length();
  int i = 0;

  // 入力済み文字列
  for (; i < input_count; i++) {
    attron(COLOR_PAIR(COMPLETED_COLOR));
    mvprintw(12, 30+i, "%c", problem[i]);
  }

  // 入力中文字
  if (i == input_count) {
    attron(COLOR_PAIR(INPUTTING_COLOR));
    mvprintw(12, 30+i, "%c", problem[i]);
    mvhline(13, 30+i, ACS_HLINE, 1);
    i++;
  }
  // 未入力文字列
  for (; i <= len; i++) {
    attron(COLOR_PAIR(UNFILLED_COLOR));
    mvprintw(12, 30+i, "%c", problem[i]);
  }
  refresh();
}

int main() {
  init_screen();
  print();

  auto game_thread = std::thread([] {
    while (game_flag) {
      update();
      print();
      std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }
  });

  while (true) {
    ch = getch();
    if (ch == 0x1b) break;
  }
  game_flag = false;
  game_thread.join();

  endwin();
}
