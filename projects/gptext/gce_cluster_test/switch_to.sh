#!/bin/bash
set -x
set -e

if [ $1 == "5" ];then
	rm /usr/local/greenplum-db && ln -s /usr/local/greenplum-db-5.0.0 /usr/local/greenplum-db
	pssh -h hostfile_gpinitsystem "rm /usr/local/greenplum-db && ln -s /usr/local/greenplum-db-5.0.0 /usr/local/greenplum-db"
	pssh -h hostfile_gpinitsystem "cp ~/gpdb5.env ~/.bashrc"
	source ~/gpdb5.env
	source /usr/local/greenplum-text-3.2.0/greenplum-text_path.sh
elif [ $1 == "6" ];then
	rm /usr/local/greenplum-db && ln -s /usr/local/greenplum-db-6.0.0-beta.1 /usr/local/greenplum-db
	pssh -h hostfile_gpinitsystem "rm /usr/local/greenplum-db && ln -s /usr/local/greenplum-db-6.0.0-beta.1 /usr/local/greenplum-db"
	pssh -h hostfile_gpinitsystem "cp ~/gpdb6.env ~/.bashrc"
	source ~/gpdb6.env
	source /usr/local/greenplum-text-3.3.0/greenplum-text_path.sh
else
	echo "unsupport ${1}"	
fi

gpstart -a
zkManager start
gptext-start
echo "switched to gpdb${1}"
exit 0
