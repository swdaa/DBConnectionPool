
docker run -d \
  --name my-mysql \
  -e MYSQL_ROOT_PASSWORD=123456 \
  -e MYSQL_DATABASE=testdb \
  -e MYSQL_USER=testuser \
  -e MYSQL_PASSWORD=testpass \
  -p 3306:3306 \
  mysql:8.0


brew install mysql-client



/opt/homebrew/opt/mysql-client/bin/mysql -h 127.0.0.1 -P 3306 -u testuser -p
