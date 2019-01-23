pipeline {
  agent any
  stages {
    stage('Get testing environment') {
      steps {
        echo 'RTAlib Jenkins testing pipeline'
        sh 'singularity instance start --bind bind_dirs/lib:/var/lib --bind bind_dirs/lib/mysql:/var/lib/mysql --bind bind_dirs/log:/var/log --bind bind_dirs/run:/var/run ../Singularity_images/rta_lib_env_service.sif rta_lib_env_service'
        sh 'pwd'
      }
    }
    stage('Creating config file') {
      steps {
        sh '''cat >rtalibconfig_testing <<EOL
[General]
modelname=rtalib_dl_test_table
mjdref=5000
debug=yes
batchsize=1
numberofthreads=1

[Dtr]
active=no
debug=no
inputchannel=

[MySql]
host=127.0.0.1
port=60307
username=tester
password=SingMysqlTester2018
dbname=rtalib_db_test

[Redis]
host=127.0.0.1
port=63800
password=
dbname=1
indexon=rtalib_dl_test_table:timerealtt,rtalib_test_table:a
EOL

cat rtalibconfig_testing'''
      }
    }
    stage('Singularity Instance ') {
      steps {
        echo 'RTAlib testing'
        sh 'mkdir -p bind_dirs/lib/mysql && mkdir -p bind_dirs/log && mkdir -p bind_dirs/run/mysqld'
        sh '''set +e;
singularity instance start --bind bind_dirs/lib:/var/lib --bind bind_dirs/lib/mysql:/var/lib/mysql --bind bind_dirs/log:/var/log --bind bind_dirs/run:/var/run ../Singularity_images/rta_lib_env_service.sif rta_lib_env_service
set -e;'''
        sh '''singularity shell instance://rta_lib_env_service;
which python'''
        sh 'which python'
      }
    }
    stage('PyRTAlib Unit-testing') {
      parallel {
        stage('Unit-testing') {
          steps {
            sh 'pwd'
            sh 'export RTALIBCONFIG=/var/jenkins_home/workspace/RTAlib_master/rtalibconfig_testing'
            sh 'conda activate rtalib-env'
            sh 'python PyRTAlib/TestEnvironment/unit_tests/Config_unittest.py -v'
            sh 'python PyRTAlib/TestEnvironment/unit_tests/MySqlDBConnector_unittest.py -v'
            sh '''python PyRTAlib/TestEnvironment/unit_tests/RedisDBConnector_unittest.py -v
'''
            sh 'python PyRTAlib/TestEnvironment/unit_tests/RTA_DL_DB_unittest.py -v'
          }
        }
        stage('PyRTAlib Test coverage') {
          steps {
            echo 'Coverage test'
            sh 'export RTALIBCONFIG=/var/jenkins_home/workspace/RTAlib_master/rtalibconfig_testing'
            sh 'source activate rtalib-env'
          }
        }
      }
    }
  }
}