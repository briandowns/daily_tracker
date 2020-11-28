FROM alpine as builder

RUN apk update && \
    apk add make git build-base
COPY . /daily_tracker
RUN cd /daily_tracker && \
    make

FROM scratch
COPY --from=builder /daily_tracker/bin/daily_tracker /daily_tracker
ENTRYPOINT ["/daily_tracker"]
