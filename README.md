# run

## local

```sh
# 编译
make

# 启动client
./bin/socket-server

# 启动client
./bin/socket-client
```
## docker

```sh
# 编译
docker build -t socket .

# 启动client
docker run -t --rm --name socket socket socket-server

# 启动client
docker exec -it socket socket-client
```