FROM gcc:latest
COPY ./code /tmp/code
WORKDIR /tmp/code
RUN apt-get update && apt-get install -y libgsl-dev
RUN make
