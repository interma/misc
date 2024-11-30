#pull base image
FROM alpine:3.6

ADD ./main /usr/local/bin/counter

ENTRYPOINT ["/bin/sh", "-c", "/usr/local/bin/counter --store_path=/usr/share/data/"]
#ENTRYPOINT ["/bin/sh", "-c", "sleep 36000"]
