namespace go_zero {

enum Color {
  kNone,
  kBlack,
  kWhite,
};

Color OppositeColor(Color color) {
  switch (color) {
    case kNone:
      return kNone;
    case kBlack:
      return kWhite;
    case kWhite:
      return kBlack;
  }
}

}  // namespace go_zero
