cc_library(name = "game",
           srcs = ["game.cc"],
           hdrs = ["game.h"],
)

cc_test(name = "game_test",
        srcs = ["game_test.cc"],
        deps = [
          ":game",
          "@gtest//:gtest_main",
        ],
)
