#pull base image
FROM alpine:3.6

ARG tag
ENV IMAGE_TAG=$tag
RUN echo "image tag: $IMAGE_TAG"

ADD ./main /usr/local/bin/main

#ENTRYPOINT "echo ${IMAGE_TAG}"
#ENTRYPOINT ["/bin/sh", "-c", "echo $IMAGE_TAG"]
ENTRYPOINT ["/bin/sh", "-c", "/usr/local/bin/main --hawqimage=hawqbeijing/hawq_proxy:$IMAGE_TAG"]
#ENTRYPOINT ["/usr/local/bin/main", "--hawqimage=hawqbeijing/hawq_proxy:$IMAGE_TAG"]
#ENTRYPOINT ["/usr/local/bin/main"]

