#ifndef AAU_P1_SOFTWARE_SRC_APPLICATION_H_
#define AAU_P1_SOFTWARE_SRC_APPLICATION_H_

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
const char* kRejseplanenApiBaseUrl =
    "https://xmlopen.rejseplanen.dk/bin/rest.exe/";

enum Constants { kBufferSize = 256 };

void Run();

#endif  // AAU_P1_SOFTWARE_SRC_APPLICATION_H_
