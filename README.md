# mnpostman

A small "curl" project that aims to provide a template of C code and Makefile to make requests to servers.

# How-to-use

## Docker
Clone docker image:
    docker pull conanastro2002/mnpostman
    docker run -d conanastro2002/mnpostman 

Now there should be a running container. If not, try:
    docker run -d conanastro2002/mnpostman tail -f /dev/null

After that, just get into a bash shell of the docker container
    docker exec -it <the container_id> bash

Then you can cd into /mnpostman and work with the executable by commands such as:
    ./mnpostman GET www.google.com

## Traditional
After cloning the repository, you will have to install the GNU make program (https://www.gnu.org/software/make/) and GCC to compile the code.
Compile:
    make
Then you can work with executable "mnpostman" by commands such as:
    ./mnpostman GET www.google.com

Have fun exploring the ancient C code.
