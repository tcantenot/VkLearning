cd external
git submodule add --depth 1 -- https://github.com/fmtlib/fmt.git
git submodule add --depth 1 -- https://github.com/gabime/spdlog.git
git submodule add --depth 1 -- https://github.com/onqtam/doctest.git
git submodule add --depth 1 -- https://github.com/Perlmint/glew-cmake.git glew
git submodule add --depth 1 -- https://github.com/glfw/glfw.git glfw
git submodule add --depth 1 -- https://github.com/g-truc/glm.git glm
git submodule add --depth 1 -- https://github.com/g-truc/gli.git gli
git submodule add --depth 1 -- https://github.com/nothings/stb.git stb
git submodule add --depth 1 -- https://github.com/ocornut/imgui.git imgui
git submodule add --depth 1 -- https://github.com/madler/zlib.git zlib (and add 'ignore = dirty' in .gitmodules below 'url = https://github.com/madler/zlib.git')
git submodule add --depth 1 -- https://github.com/assimp/assimp.git assimp
cd ..


git clone xxx.git
git submodule update --init --recursive --depth 1