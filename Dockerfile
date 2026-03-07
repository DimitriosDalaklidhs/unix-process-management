# ---------- Build stage ----------
FROM debian:stable-slim AS build

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        build-essential && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /src

# Copy the source file from your repo
COPY ask4.c .

# Compile and optimize
RUN gcc -O2 -Wall -o ask4 ask4.c && strip ask4


# ---------- Runtime stage ----------
FROM debian:stable-slim

WORKDIR /app

# Copy only the compiled binary from the build stage
COPY --from=build /src/ask4 .

# Run the application when the container starts
CMD ["./ask4"]
