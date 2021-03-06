# dev 

```sh
# ubuntu
sudo apt install build-essential

# centos
sudo yum groupinstall 'Development Tools'
```

# run

## local

```sh
# 编译
make

# 启动server，随意其中的一个
./bin/socket-server
./bin/select-server
./bin/poll-server
./bin/epoll-server

# 启动client，可同时启动多个
./bin/socket-client
./bin/select-client
./bin/poll-client
./bin/epoll-client
```
## docker

```sh
# 编译镜像
docker build -t socket .
```

```sh
# 启动所有的
docker-compose up
```

```
# 启动server
docker run -t --rm --name socket-server socket socket-server

# 启动client
docker run -t --rm --name socket-client socket socket-client $(docker inspect -f "{{.NetworkSettings.IPAddress}}" socket-server)
```