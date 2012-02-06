drop table if exists sensors_values, actuators_commands, system_logs ;

create table sensors_values (id int(11) not null auto_increment, date timestamp default current_timestamp, room int(11) not null, metric int(11) not null,  value int(11) not null, primary key id (id));

create table actuators_commands (id int(11) not null auto_increment, time datetime not null, room int(11) not null, metric int(11) not null,  value int(11) not null, primary key id (id));

create table system_logs (id int(11) not null auto_increment, time datetime not null, type int(11) not null, message varchar(512) not null, primary key id (id));

