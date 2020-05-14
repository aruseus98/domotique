#!/bin/bash
user=Alex
pass=Admin327
hote=192.168.0.250
db=Projet

mysql -h "$hote" -D "$db" -u "$user" --password=$pass < signal.sql
