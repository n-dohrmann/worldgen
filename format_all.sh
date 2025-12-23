#! /bin/zsh
# assumes ripgrep (rg), clang-format
rg --files | rg -e "c$" -e "\.h$" | xargs -I {} clang-format -i --style=webkit {}
