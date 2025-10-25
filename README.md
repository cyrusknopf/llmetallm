# Painful setup
1. Install the single header of metal (done already: `Metal/Metal.hpp`)

2. Install xcode metal. Try and run `xcrun metal` and if it errors, follow:
  a) Install XCode from the app store (????)
  b) `sudo xcode-select --switch /Applications/Xcode.app/Contents/Developer` (pls don't ask what this does)
  c) Try and run `xcrun metal` again; you'll have to accept a license agreement, then install a few other things along the way

3. `mkdir build && cd build/ && cmake .. && make`
