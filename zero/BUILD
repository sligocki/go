cc_library(name = "board",
           srcs = ["board.cc"],
           hdrs = ["board.h"],
)

cc_library(name = "board_io",
           srcs = ["board_io.cc"],
           hdrs = ["board_io.h"],
           deps = [
               ":board",
           ],
)

cc_test(name = "board_test",
        srcs = ["board_test.cc"],
        deps = [
          ":board",
          ":board_io",
          "@gtest//:gtest_main",
        ],
)

cc_library(name = "game",
           srcs = ["game.cc"],
           hdrs = ["game.h"],
           deps = [
               ":board",
           ],
)

cc_test(name = "game_test",
        srcs = ["game_test.cc"],
        deps = [
            ":game",
            "@gtest//:gtest_main",
        ],
)
