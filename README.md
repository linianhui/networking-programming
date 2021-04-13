# Run

```sh
docker build -t socket-echo ./socket-echo

docker run -t --name socket-echo-server socket-echo

docker exec -it socket-echo-server ./client
```