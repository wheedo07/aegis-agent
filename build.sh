#!/bin/bash

dir=$(basename "$PWD")
if [[ "$dir" != "agent" && "$dir" !=  "aegis-agent" ]]; then
    echo "프로젝트 ROOT 경로에서 실행해주세요"
    exit 1
fi

if [ ! -d "build" ]; then
    mkdir build
fi

rm -f build/*;
cd module && zip ../build/aegis-mod.zip *;
cd ..;
docker build -f Dockerfile -t aegis-agent . && \
docker create --name tmp aegis-agent /aegis-agent_x86_64 && \
docker cp tmp:/aegis-agent_x86_64 ./conf/aegis-agent_x86_64 && \
docker rm tmp;
cd conf;
zip ../build/aegis-agent_x86_64.zip * && rm aegis-agent_x86_64;