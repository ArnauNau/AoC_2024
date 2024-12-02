FROM debian:stable-slim
LABEL authors="arnau.sf"
LABEL description="Docker image for the development and testing of the OS Labs."

# Update apt repositories and install Valgrind
RUN apt-get update && apt-get install -y \
    valgrind \
    make \
    gcc
#git \
#gdb
# Add Testing repository for GCC 13
RUN echo "deb http://deb.debian.org/debian testing main" >> /etc/apt/sources.list \
    && apt-get update \
    && apt-get install -y gcc-13 \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*
# Optional: Set GCC 13 as default
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-13 100

ADD ./ /workspace/
#COPY ./include /workspace/include

WORKDIR /workspace

RUN echo 'alias val="valgrind -s --read-var-info=yes --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes --track-fds=yes"' >> ~/.bashrc
#RUN make clean && make

# Keep the container running (adjust as needed)
CMD ["/bin/bash"]

#ENTRYPOINT ["top", "-b"]
