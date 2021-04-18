# run

## local

```sh
# 编译
make

# 启动server，随意其中的一个
./bin/socket-server
./bin/select-server
./bin/poll-server

# 启动client，可同时启动多个
./bin/socket-client
./bin/select-client
./bin/poll-client
```
## docker

```sh
# 编译
docker build -t socket .

# 启动server
docker run -t --rm --name socket-server socket socket-server

# 启动client
docker run -t --rm --name socket-client socket socket-client $(docker inspect -f "{{.NetworkSettings.IPAddress}}" socket-server)
```