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

cc_library(name = "features",
           srcs = ["features.h"],
           deps = [
               ":board",
           ],
)

cc_library(name = "game",
           srcs = ["game.cc"],
           hdrs = ["game.h"],
           deps = [
               ":board",
               ":features",
           ],
)

cc_test(name = "game_test",
        srcs = ["game_test.cc"],
        deps = [
            ":game",
            "@gtest//:gtest_main",
        ],
)

cc_library(name = "player",
           srcs = ["player.cc"],
           hdrs = ["player.h"],
           deps = [
               ":features",
               ":game",
               "//util:random",
           ],
)

cc_library(name = "mcts",
           srcs = ["mcts.cc"],
           hdrs = ["mcts.h"],
           deps = [
               ":features",
               ":game",
               ":player",
           ],
)

cc_binary(name = "mcts_main",
          srcs = ["mcts_main.cc"],
          deps = [
              ":board_io",
              ":features",
              ":game",
              ":mcts",
              ":player",
              "//util:random",
              "//external:gflags",
          ],
)
