# run

## local

```sh
# 编译
make

# 启动server
./bin/socket-server

# 启动client
./bin/socket-client
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