// va_list 测试

#include <locale.h>
#include <stdarg.h>
#include <stdio.h>
#include <wchar.h>

#include <iostream>
#include <memory>

int ShowArg(const wchar_t *szFirstSentence, ...) {
  int num = 0;  // 统计可变参数数量

  va_list vl;  // 声明一个va_list对象，用于对参数遍历

  va_start(vl, szFirstSentence);  // 使用va_start对变量进行初始化

  // 使用va_arg返回实参列表中的下一个参数，并把va内部指针向下推一个参数
  std::wstring p = szFirstSentence;

  while (p != L"0") {
    std::wcout << p;
    num++;
    p = va_arg(vl, const wchar_t *);
  }

  // 所有参数读取完成之后，调用va_end对va和va_list进行清理
  va_end(vl);

  return num;
}

int main(int argc, char **argv) {
  setlocale(LC_ALL, "");
  int x = ShowArg(L"你好！\n", L"0");
  int y = ShowArg(L"很好！", L"你呢?\n", L"0");
  int z = ShowArg(L"我也好！", L"天气不错！", L"吃饭了么？\n", L"0");

  printf("x = %d, y = %d, z= %d\n", x, y, z);
  return 0;
}
