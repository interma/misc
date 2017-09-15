
export HADOOP_HOME=/opt/hadoop-2.7.3
export HIVE_CONF_DIR=/opt/apache-hive-2.2.0/conf


export HADOOP_USER_CLASSPATH_FIRST=true
export TEZ_HOME=/opt/apache-tez-0.8.4
export HADOOP_CLASSPATH=$HADOOP_CLASSPATH:$TEZ_HOME/*:$TEZ_HOME/lib/*
