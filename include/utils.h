#pragma once


#include <time.h>
#include <cstdlib>
#include <string>
#include <fstream>
#include <filesystem>

inline unsigned long millis() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts); // Utilisation de CLOCK_MONOTONIC pour éviter les changements d'horloge
    return (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000); // Conversion en millisecondes
}

inline std::string currentTimeFormatted() {
    time_t temps;
    struct tm date;
    char buffer[32];
    time(&temps);
    date = *localtime(&temps);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &date);
    return std::string(buffer);
}

inline bool isWifiConnected()
{
    std::ifstream file("/proc/net/wireless");
    std::string line;

    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            if (line.find("wlan0") != std::string::npos)
            {
                return true;
            }
        }
        file.close();
    }
    return false;
}

inline void setProgramPriority(int val = 99) // Max priority
{
    struct sched_param param;
    param.sched_priority = 99;  // Highest priority for SCHED_FIFO

    if (sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
        perror("sched_setscheduler failed");
        throw std::runtime_error("sched_setscheduler failed. Make sure the programs runs in SUDO");
    }
    printf("Program priority set to %d\n", val);
}

template<typename T>
inline T clip(T x, T lo, T hi) {
    return x < lo ? lo : (x > hi ? hi : x);
}

inline std::filesystem::path getExecutableDir(char* argv0) {
    return std::filesystem::canonical(argv0).parent_path();
}


#define DECLARE_ENUM_CLASS(NAME, ...)                                \
    enum class NAME { __VA_ARGS__ };                                 \
                                                                     \
    inline const char* NAME##_to_string(NAME value) {                \
        switch (value) {                                             \
            FOR_EACH_ENUM(ENUM_CASE, NAME, __VA_ARGS__)              \
        default: return "inconnu";                                   \
        }                                                            \
    }

#define ENUM_CASE(name, enumName) case enumName::name: return #name;

#define FE_1(WHAT, NAME, X) WHAT(X, NAME)
#define FE_2(WHAT, NAME, X, ...)  WHAT(X, NAME) FE_1(WHAT, NAME, __VA_ARGS__)
#define FE_3(WHAT, NAME, X, ...)  WHAT(X, NAME) FE_2(WHAT, NAME, __VA_ARGS__)
#define FE_4(WHAT, NAME, X, ...)  WHAT(X, NAME) FE_3(WHAT, NAME, __VA_ARGS__)
#define FE_5(WHAT, NAME, X, ...)  WHAT(X, NAME) FE_4(WHAT, NAME, __VA_ARGS__)
#define FE_6(WHAT, NAME, X, ...)  WHAT(X, NAME) FE_5(WHAT, NAME, __VA_ARGS__)
#define FE_7(WHAT, NAME, X, ...)  WHAT(X, NAME) FE_6(WHAT, NAME, __VA_ARGS__)
#define FE_8(WHAT, NAME, X, ...)  WHAT(X, NAME) FE_7(WHAT, NAME, __VA_ARGS__)
#define FE_9(WHAT, NAME, X, ...)  WHAT(X, NAME) FE_8(WHAT, NAME, __VA_ARGS__)
#define FE_10(WHAT, NAME, X, ...) WHAT(X, NAME) FE_9(WHAT, NAME, __VA_ARGS__)
#define FE_11(WHAT, NAME, X, ...) WHAT(X, NAME) FE_10(WHAT, NAME, __VA_ARGS__)
#define FE_12(WHAT, NAME, X, ...) WHAT(X, NAME) FE_11(WHAT, NAME, __VA_ARGS__)
#define FE_13(WHAT, NAME, X, ...) WHAT(X, NAME) FE_12(WHAT, NAME, __VA_ARGS__)
#define FE_14(WHAT, NAME, X, ...) WHAT(X, NAME) FE_13(WHAT, NAME, __VA_ARGS__)
#define FE_15(WHAT, NAME, X, ...) WHAT(X, NAME) FE_14(WHAT, NAME, __VA_ARGS__)
#define FE_16(WHAT, NAME, X, ...) WHAT(X, NAME) FE_15(WHAT, NAME, __VA_ARGS__)
#define FE_17(WHAT, NAME, X, ...) WHAT(X, NAME) FE_16(WHAT, NAME, __VA_ARGS__)
#define FE_18(WHAT, NAME, X, ...) WHAT(X, NAME) FE_17(WHAT, NAME, __VA_ARGS__)
#define FE_19(WHAT, NAME, X, ...) WHAT(X, NAME) FE_18(WHAT, NAME, __VA_ARGS__)
#define FE_20(WHAT, NAME, X, ...) WHAT(X, NAME) FE_19(WHAT, NAME, __VA_ARGS__)
#define FE_21(WHAT, NAME, X, ...) WHAT(X, NAME) FE_20(WHAT, NAME, __VA_ARGS__)
#define FE_22(WHAT, NAME, X, ...) WHAT(X, NAME) FE_21(WHAT, NAME, __VA_ARGS__)
#define FE_23(WHAT, NAME, X, ...) WHAT(X, NAME) FE_22(WHAT, NAME, __VA_ARGS__)
#define FE_24(WHAT, NAME, X, ...) WHAT(X, NAME) FE_23(WHAT, NAME, __VA_ARGS__)
#define FE_25(WHAT, NAME, X, ...) WHAT(X, NAME) FE_24(WHAT, NAME, __VA_ARGS__)
#define FE_26(WHAT, NAME, X, ...) WHAT(X, NAME) FE_25(WHAT, NAME, __VA_ARGS__)
#define FE_27(WHAT, NAME, X, ...) WHAT(X, NAME) FE_26(WHAT, NAME, __VA_ARGS__)
#define FE_28(WHAT, NAME, X, ...) WHAT(X, NAME) FE_27(WHAT, NAME, __VA_ARGS__)
#define FE_29(WHAT, NAME, X, ...) WHAT(X, NAME) FE_28(WHAT, NAME, __VA_ARGS__)
#define FE_30(WHAT, NAME, X, ...) WHAT(X, NAME) FE_29(WHAT, NAME, __VA_ARGS__)
#define FE_31(WHAT, NAME, X, ...) WHAT(X, NAME) FE_30(WHAT, NAME, __VA_ARGS__)
#define FE_32(WHAT, NAME, X, ...) WHAT(X, NAME) FE_31(WHAT, NAME, __VA_ARGS__)

#define GET_FE_MACRO( \
    _1,  _2,  _3,  _4,  _5,  _6,  _7,  _8,  \
    _9,  _10, _11, _12, _13, _14, _15, _16, \
    _17, _18, _19, _20, _21, _22, _23, _24, \
    _25, _26, _27, _28, _29, _30, _31, _32, \
    NAME, ...) NAME

#define FOR_EACH_ENUM(action, NAME, ...) \
    GET_FE_MACRO(__VA_ARGS__,            \
        FE_32, FE_31, FE_30, FE_29, FE_28, FE_27, FE_26, FE_25, \
        FE_24, FE_23, FE_22, FE_21, FE_20, FE_19, FE_18, FE_17, \
        FE_16, FE_15, FE_14, FE_13, FE_12, FE_11, FE_10, FE_9,  \
        FE_8,  FE_7,  FE_6,  FE_5,  FE_4,  FE_3,  FE_2,  FE_1  \
    )(action, NAME, __VA_ARGS__)
