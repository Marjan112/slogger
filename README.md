# Slogger (Simple Logger)

Slogger is a Simple Logger API.

## Usage
```c++
#include "Slogger.hpp"
#include <cmath>

int main() {
    if(!Slogger::EnableFileLogging("log_file.log")) {
        return 1;
    }

    Slogger::Debug("Hello");
    Slogger::Info("World %d", 420);
    Slogger::Warning("Hello, %s", "World");
    Slogger::Error("PI is approximately %f", 4 * atan2f(1, 1));
    return 0;
}
```

# License
Licensed under MIT License, see the [LICENSE](./LICENSE) file.