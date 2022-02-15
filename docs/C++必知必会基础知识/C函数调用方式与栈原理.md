## `C++`函数调用栈帧结构

![`C++`函数调用栈帧](./images/C++栈帧.png)

例如：

```cpp
int foo(int arg1, int arg2, int arg3);
```

`main`函数中调用`foo`:

- `main`为调用者(`caller`)，`foo`为被调用者(`callee`)；
- **`ESP`被`foo`用来指向栈顶，`EBP`类似基准指针。**
- **小于等于 4 字节的返回值直接保存在 EAX 中；其余方式可以通过额外参数来传递出返回值。**

### `C++ 17`结构化绑定

```cpp
#include <iostream>
#include <map>

int main(int argc, char **argv) {
  std::map<std::string, int> cities;

  cities["guangzhou"] = 0;
  cities["zhengzhou"] = 1;
  cities["beijing"]   = 2;
  cities["shanghai"]  = 3;

  for (const auto &[cityname, citynumber] : cities) {
    std::cout << cityname << " : " << citynumber << std::endl;
  }

  return 0;
}

```
