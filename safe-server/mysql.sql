create database if not exists gosafe DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;
create user if not exists 'gosafe'@'localhost' identified by '12345678';
GRANT ALL PRIVILEGES ON `gosafe`.* TO 'gosafe'@'localhost';
flush privileges;