# Run

```sh
docker build -t socket .

docker run -t --rm --name socket socket socket-server

docker exec -it socket socket-client
```