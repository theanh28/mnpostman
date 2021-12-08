FROM gcc

WORKDIR /

RUN mkdir mnpostman
COPY . /mnpostman

RUN cd mnpostman/src && mkdir obj
RUN cd mnpostman && make

CMD tail -f /dev/null 
# keep docker container running
# if fail, use:
# docker run -d <image_name> tail -f /dev/null