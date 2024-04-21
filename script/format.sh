#!/bin/sh

if [ ! $CI ]; then
  export PATH=$PATH:/opt/homebrew/bin
  clang-format -i -style=file `find ./ae-core/Sources ./ae-core/Tests -type f \( -name *.h -o -name *.cpp -o -name *.hpp -o -name *.m -o -name *.mm \)`
  swift-format -r ./ae-core/Sources ./ae-core/Tests -i
fi
