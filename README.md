# mnpostman

A small "curl" project that aims to provide a template of C code and Makefile to make requests to servers.

# Download

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

# Manual
A request should have the form of 

    <GET/POST/PUT/DELETE> <URL>

for example:

    GET https://www.google.com
    
Options:
    -p      option port number, default to 80 when using HTTP and 443 when using HTTPS
    -c      show certificate, show SSL certificate of server
Complete command has the form

    ./mnpostman [-p <port number>, -c] <GET/POST/PUT/DELETE> <URL>
    
for example:

    ./mnpostman -p 443 -c GET https://www.google.com

# HTTP vs HTTPS
If the url does not start with https://, the program use the primitive non-SSL/TLS method to send request. Otherwise, it use version-flexible SSL/TLS method.
    
# Bug to fix
Currently, there will be a "stack smashing detected" when you use https url. A suspected reason is the packet received through SSL/TLS crypto protocol is too large for the max size of packet received throguh TCP transport protocol, leading to the error which is prominent for "buffer overflow" related problem.
