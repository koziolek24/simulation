build_app:
	sudo apt install     libxrandr-dev     libxcursor-dev     libxi-dev     libudev-dev     libfreetype-dev     libflac-dev     libvorbis-dev     libgl1-mesa-dev     libegl1-mesa-dev     libfreetype-dev fonts-freefont-ttf libopenal-dev
	git submodule update --init --recursive
	mkdir -p include/nlohmann
	wget https://github.com/nlohmann/json/releases/download/v3.11.2/json.hpp -O include/nlohmann/json.hpp
	mkdir -p build && \
	cd build && \
	cmake .. && \
	make && \
	./zpr25l_sim
