FROM ubuntu

RUN apt update && apt -y install gcc make bash

COPY . container-pipex

RUN ["/bin/bash"]